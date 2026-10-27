#ifndef BACKEND_LIBINPUT_H
#define BACKEND_LIBINPUT_H

#include <libinput.h>
#include <wayland-server-core.h>
#include <wlr/backend/interface.h>
#include <wlr/backend/libinput.h>
#include <wlr/types/wlr_keyboard.h>
#include <wlr/types/wlr_pointer.h>
#include <wlr/types/wlr_switch.h>
#include <wlr/types/wlr_touch.h>

struct wlr_libinput_backend {
	struct wlr_backend backend;

	struct wlr_session *session;

	struct libinput *libinput_context;
	struct wl_event_source *input_event;

	struct wl_listener session_destroy;
	struct wl_listener session_signal;

	struct wl_list devices; // wlr_libinput_device.link
};

struct wlr_libinput_input_device {
	struct libinput_device *handle;

	struct wlr_keyboard keyboard;
	struct wlr_pointer pointer;
	struct wlr_switch switch_device;
	struct wlr_touch touch;

	struct wl_list link;
};

uint32_t usec_to_msec(uint64_t usec);

void handle_libinput_event(struct wlr_libinput_backend *state,
		struct libinput_event *event);

void destroy_libinput_input_device(struct wlr_libinput_input_device *dev);
const char *get_libinput_device_name(struct libinput_device *device);

extern const struct wlr_keyboard_impl libinput_keyboard_impl;
extern const struct wlr_pointer_impl libinput_pointer_impl;
extern const struct wlr_switch_impl libinput_switch_impl;
extern const struct wlr_touch_impl libinput_touch_impl;

void init_device_keyboard(struct wlr_libinput_input_device *dev);
struct wlr_libinput_input_device *device_from_keyboard(struct wlr_keyboard *kb);
void handle_keyboard_key(struct libinput_event *event, struct wlr_keyboard *kb);

void init_device_pointer(struct wlr_libinput_input_device *dev);
struct wlr_libinput_input_device *device_from_pointer(struct wlr_pointer *kb);
void handle_pointer_motion(struct libinput_event *event,
	struct wlr_pointer *pointer);
void handle_pointer_motion_abs(struct libinput_event *event,
	struct wlr_pointer *pointer);
void handle_pointer_button(struct libinput_event *event,
	struct wlr_pointer *pointer);
void handle_pointer_axis(struct libinput_event *event,
	struct wlr_pointer *pointer);
void handle_pointer_axis_value120(struct libinput_event *event,
	struct wlr_pointer *pointer, enum wl_pointer_axis_source source);
void handle_pointer_swipe_begin(struct libinput_event *event,
	struct wlr_pointer *pointer);
void handle_pointer_swipe_update(struct libinput_event *event,
	struct wlr_pointer *pointer);
void handle_pointer_swipe_end(struct libinput_event *event,
	struct wlr_pointer *pointer);
void handle_pointer_pinch_begin(struct libinput_event *event,
	struct wlr_pointer *pointer);
void handle_pointer_pinch_update(struct libinput_event *event,
	struct wlr_pointer *pointer);
void handle_pointer_pinch_end(struct libinput_event *event,
	struct wlr_pointer *pointer);
void handle_pointer_hold_begin(struct libinput_event *event,
	struct wlr_pointer *pointer);
void handle_pointer_hold_end(struct libinput_event *event,
	struct wlr_pointer *pointer);

void init_device_switch(struct wlr_libinput_input_device *dev);
struct wlr_libinput_input_device *device_from_switch(
	struct wlr_switch *switch_device);
void handle_switch_toggle(struct libinput_event *event,
	struct wlr_switch *switch_device);

void init_device_touch(struct wlr_libinput_input_device *dev);
struct wlr_libinput_input_device *device_from_touch(
	struct wlr_touch *touch);
void handle_touch_down(struct libinput_event *event,
	struct wlr_touch *touch);
void handle_touch_up(struct libinput_event *event,
	struct wlr_touch *touch);
void handle_touch_motion(struct libinput_event *event,
	struct wlr_touch *touch);
void handle_touch_cancel(struct libinput_event *event,
	struct wlr_touch *touch);
void handle_touch_frame(struct libinput_event *event,
	struct wlr_touch *touch);

bool button_state_from_libinput(enum libinput_button_state state, enum wlr_button_state *out);

#endif
