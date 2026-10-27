CFLAGS = -O2 -march=native -mtune=native -pipe \
         -fno-plt -fomit-frame-pointer \
         -fstack-protector-strong -D_FORTIFY_SOURCE=2 \
         -ffunction-sections -fdata-sections -flto -DWLR_USE_UNSTABLE \
         -g0

# Uncomment to build XWayland support
#XWAYLAND = -DXWAYLAND

# Uncomment to enable fullscreen tearing support.  This creates the
# wp_tearing_control_manager_v1 protocol and allows tearing for fullscreen
# windows.  The ToggleTearing action can override it per window.
#FULLSCREEN_TEARING = -DFULLSCREEN_TEARING

# ---------------------------------------------------------------------------
# Wlroots feature toggles.  These control what goes INTO the amalgamated
# wlroots.c (which wlroots sources are pulled in and the matching WLR_HAS_*
# macros in the generated config.h).  Toggling one re-runs amalgamate.py on
# the next make because config.mk is a prerequisite of wlroots.c.
# ---------------------------------------------------------------------------

# Uncomment to build the X11 backend into wlroots
#X11_BACKEND = -DX11_BACKEND

# Uncomment to build the Vulkan renderer into wlroots
#VULKAN = -DVULKAN

CC = cc
# Strict warnings applied to the compositor code (wlroots.c suppresses them)
STRICT ?= -Werror -Wshadow -Wconversion
STRIP ?= strip
PKG_CONFIG ?= pkg-config
PKG_CONFIG_PATH := /home/lynch/Downloads:/usr/local/lib64/pkgconfig$(if $(PKG_CONFIG_PATH),:$(PKG_CONFIG_PATH))
export PKG_CONFIG_PATH
PREFIX ?= /usr/local
MANDIR ?= $(PREFIX)/share/man
DATADIR ?= $(PREFIX)/share
