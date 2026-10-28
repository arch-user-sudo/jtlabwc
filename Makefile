.POSIX:
.SUFFIXES:

all: jtlabwc jtlabwcctl


include config.mk

AMALG = amalgamate.py
SCAN  = scan.py

# Every vendored wlroots input that affects the generated amalgamation.
WLR_SRCS = $(shell find wlroots -type f \( -name '*.c' -o -name '*.h' -o -name '*.xml' -o -name '*.sh' \) 2>/dev/null)

PKGS = wayland-server wayland-client xkbcommon libinput pixman-1 libdrm egl gbm glesv2 \
       lcms2 libudev libseat libdisplay-info libliftoff \
       cairo pangocairo pango harfbuzz glib-2.0

# XCB is only needed by the XWayland and X11 backend paths.
ifneq ($(XWAYLAND)$(X11_BACKEND),)
PKGS += xcb xcb-dri3 xcb-present xcb-render xcb-renderutil xcb-shm xcb-xfixes \
        xcb-xinput xcb-composite xcb-ewmh xcb-icccm xcb-res xcb-shape
endif

CFLAGS += $(XWAYLAND) $(FULLSCREEN_TEARING) $(shell $(PKG_CONFIG) --cflags $(PKGS)) -I. -Iamalgamate-build/include -Iwlroots/include -Ibuild/proto
LDFLAGS += $(shell $(PKG_CONFIG) --libs $(PKGS)) -lm

WAYLAND_SCANNER ?= wayland-scanner
PROTO_DIR = protocols
PROTO_GEN_DIR = build/proto

PROTO_XMLS = $(PROTO_DIR)/wlr-layer-shell-unstable-v1.xml $(PROTO_DIR)/wlr-output-power-management-unstable-v1.xml
PROTO_SRCS = $(PROTO_GEN_DIR)/wlr-layer-shell-unstable-v1-protocol.c $(PROTO_GEN_DIR)/wlr-output-power-management-unstable-v1-protocol.c
PROTO_HDRS = $(PROTO_GEN_DIR)/wlr-layer-shell-unstable-v1-protocol.h $(PROTO_GEN_DIR)/wlr-output-power-management-unstable-v1-protocol.h

# All jtlabwc code lives in the generated monolith jtlabwc.c.
LABWC_OBJS = build/jtlabwc.o

amalgamate:
	python3 $(AMALG)
	$(tidy_wlroots)

# clang-tidy auto-fix pass run on wlroots.c right after it is regenerated and
# before anything compiles it (it is included by jtlabwc.c).
# Only checks with safe, semantics-preserving fix-its are enabled; the big
# conversion checks (-Wconversion etc.) have no auto-fixes and are deliberately
# absent.  Skipped automatically if clang-tidy is not installed.
# Set TIDY= (empty) to disable.
TIDY        = clang-tidy
TIDY_CHECKS = -*,bugprone-macro-parentheses,bugprone-macro-repeated-side-effects,performance-type-promotion-in-math-fn,readability-inconsistent-declaration-parameter-name

# Canned recipe: the clang-tidy auto-fix pass (used by both wlroots.c rules).
# header-filter matches nothing on purpose: wlroots.c is self-contained (all
# project headers are inlined into it), so fixes must land in wlroots.c itself
# and never in system headers like pixman.h or drm_fourcc.h.
define tidy_wlroots
	@if [ -n "$(TIDY)" ] && command -v $(TIDY) >/dev/null 2>&1; then \
		echo "clang-tidy: auto-fixing wlroots.c"; \
		$(TIDY) -quiet -fix -checks='$(TIDY_CHECKS)' \
			--header-filter='a^' wlroots.c -- \
			$(CFLAGS) || exit 1; \
	else \
		echo "clang-tidy not found; skipping wlroots.c auto-fix pass"; \
	fi
endef

# Re-run the tidy auto-fix pass on the existing wlroots.c without regenerating.
tidy-wlroots: wlroots.c
	$(tidy_wlroots)

# Regenerate the amalgamation when its inputs change.  jtlabwc.c is a
# prerequisite because it seeds the wlroots pruning pass.
wlroots.c: jtlabwc.c config.mk $(AMALG) $(SCAN) $(WLR_SRCS)
	python3 $(AMALG)
	$(tidy_wlroots)

$(PROTO_GEN_DIR)/%-protocol.c: $(PROTO_DIR)/%.xml
	@mkdir -p $(PROTO_GEN_DIR)
	$(WAYLAND_SCANNER) private-code $< $@

$(PROTO_GEN_DIR)/%-protocol.h: $(PROTO_DIR)/%.xml
	@mkdir -p $(PROTO_GEN_DIR)
	$(WAYLAND_SCANNER) server-header $< $@

build/proto/%.o: $(PROTO_GEN_DIR)/%.c $(PROTO_GEN_DIR)/%.h
	@mkdir -p build/proto
	$(CC) $(CFLAGS) -Wall -Wextra -Wno-unused-parameter -c -o $@ $<

build/jtlabwc.o: jtlabwc.c wlroots.c config.mk $(PROTO_HDRS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -Wall -Wextra -Wno-unused-parameter $(STRICT) -c -o $@ $<

jtlabwc: wlroots.c $(PROTO_SRCS) $(PROTO_HDRS) jtlabwc.c $(LABWC_OBJS)
	$(CC) $(CFLAGS) -Wall -Wextra -Wno-unused-parameter -flto -o $@ $(LABWC_OBJS) $(LDFLAGS)
	$(STRIP) $@

jtlabwcctl: jtlabwcctl.c
	$(CC) $(CFLAGS) $(STRICT) -o $@ $<

clean:
	rm -rf build jtlabwc jtlabwcctl

.PHONY: amalgamate clean tidy-wlroots

all: jtlabwc jtlabwcctl

install: jtlabwc jtlabwcctl
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f jtlabwc $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/jtlabwc
	cp -f jtlabwcctl $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/jtlabwcctl
	mkdir -p $(DESTDIR)$(DATADIR)/wayland-sessions
	cp -f data/jtlabwc.desktop $(DESTDIR)$(DATADIR)/wayland-sessions
	mkdir -p $(DESTDIR)$(DATADIR)/xdg-desktop-portal
	cp -f data/labwc-portals.conf $(DESTDIR)$(DATADIR)/xdg-desktop-portal
	mkdir -p $(DESTDIR)$(DATADIR)/icons/hicolor/scalable/apps
	cp -f data/jtlabwc.svg data/jtlabwc-symbolic.svg \
		$(DESTDIR)$(DATADIR)/icons/hicolor/scalable/apps
	mkdir -p $(DESTDIR)$(PREFIX)/lib/systemd/user
	cp -f data/labwc-session.target $(DESTDIR)$(PREFIX)/lib/systemd/user

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/jtlabwc
	rm -f $(DESTDIR)$(PREFIX)/bin/jtlabwcctl
	rm -f $(DESTDIR)$(DATADIR)/wayland-sessions/jtlabwc.desktop
	rm -f $(DESTDIR)$(DATADIR)/xdg-desktop-portal/labwc-portals.conf
	rm -f $(DESTDIR)$(DATADIR)/icons/hicolor/scalable/apps/jtlabwc.svg
	rm -f $(DESTDIR)$(DATADIR)/icons/hicolor/scalable/apps/jtlabwc-symbolic.svg
	rm -f $(DESTDIR)$(PREFIX)/lib/systemd/user/labwc-session.target

.PHONY: install uninstall
