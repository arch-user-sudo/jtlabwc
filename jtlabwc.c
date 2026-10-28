#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 1
#endif
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif
#ifndef WLR_USE_UNSTABLE
#define WLR_USE_UNSTABLE 1
#endif
#include "wlroots.c"
static void jtlabwc_control_init(void);
static void jtlabwc_control_finish(void);
#ifndef LABWC_ACTION_H
#define LABWC_ACTION_H
#include <stdbool.h>
#include <sys/types.h>
#include <wayland-util.h>
struct view;
struct server;
struct cursor_context;
struct action {
	struct wl_list link;
	uint32_t type;
	struct wl_list args;
};
struct action *action_create(const char *action_name);
const char *action_get_str(struct action *action, const char *key,
	const char *default_value);
bool action_is_valid(struct action *action);
bool action_is_show_menu(struct action *action);
void action_arg_add_str(struct action *action, const char *key, const char *value);
void action_arg_add_actionlist(struct action *action, const char *key);
void action_arg_add_querylist(struct action *action, const char *key);
struct wl_list *action_get_actionlist(struct action *action, const char *key);
struct wl_list *action_get_querylist(struct action *action, const char *key);
void action_arg_from_xml_node(struct action *action, const char *nodename, const char *content);
bool actions_contain_toggle_keybinds(struct wl_list *action_list);
void actions_run(struct view *activator, struct wl_list *actions, struct cursor_context *ctx);
void action_free(struct action *action);
void action_list_free(struct wl_list *action_list);
#endif
#ifndef LABWC_BUF_H
#define LABWC_BUF_H
struct buf {
	char *data;
	int alloc;
	int len;
};
#define BUF_INIT ((struct buf){.data = ""})
void buf_expand_tilde(struct buf *s);
void buf_expand_shell_variables(struct buf *s);
void buf_add_fmt(struct buf *s, const char *fmt, ...);
void buf_add_hex_color(struct buf *s, float color[4]);
void buf_add(struct buf *s, const char *data);
void buf_add_char(struct buf *s, char ch);
void buf_clear(struct buf *s);
void buf_reset(struct buf *s);
void buf_move(struct buf *dst, struct buf *src);
struct buf buf_from_file(const char *filename);
#endif
#ifndef LABWC_MACROS_H
#define LABWC_MACROS_H
#include <limits.h>
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define CONNECT_SIGNAL(src, dest, name) \
	(dest)->name.notify = handle_##name; \
	wl_signal_add(&(src)->events.name, &(dest)->name)
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef BOUNDED_INT
#define BOUNDED_INT(a) ((a) < INT_MAX && (a) > INT_MIN)
#endif
#define _LAB_CALC_WLR_VERSION_NUM(major, minor, micro) (((uint32_t)(major) << 16) | ((uint32_t)(minor) << 8) | (uint32_t)(micro))
#define LAB_WLR_VERSION_AT_LEAST(major, minor, micro) ( \
	server.wlr_version >= _LAB_CALC_WLR_VERSION_NUM(major, minor, micro))
#define LAB_WLR_VERSION_LOWER(major, minor, micro) (!LAB_WLR_VERSION_AT_LEAST(major, minor, micro))
#endif
#ifndef LABWC_LIST_H
#define LABWC_LIST_H
#include <wayland-server-core.h>
static inline void
wl_list_append(struct wl_list *list, struct wl_list *elm)
{
	wl_list_insert(list->prev, elm);
}
#define WL_LIST_INIT(head) {.prev = (head), .next = (head)}
#endif
#ifndef LABWC_MEM_H
#define LABWC_MEM_H
#include <stdlib.h>
void die_if_null(void *ptr);
void *xzalloc(size_t size);
#define znew(expr)       ((__typeof__(expr) *)xzalloc(sizeof(expr)))
#define znew_n(expr, n)  ((__typeof__(expr) *)xzalloc((size_t)(n) * sizeof(expr)))
void *xrealloc(void *ptr, size_t size);
#define xmalloc(size) xrealloc(NULL, (size_t)(size))
char *xstrdup(const char *str);
#define xstrdup_replace(ptr, str) do { \
	free(ptr); (ptr) = xstrdup(str); \
} while (0)
#define zfree(ptr) do { \
	free(ptr); (ptr) = NULL; \
} while (0)
#endif
#ifndef LABWC_CONFIG_TYPES_H
#define LABWC_CONFIG_TYPES_H
enum lab_placement_policy {
	LAB_PLACE_INVALID = 0,
	LAB_PLACE_CENTER,
	LAB_PLACE_CURSOR,
	LAB_PLACE_AUTOMATIC,
	LAB_PLACE_CASCADE,
};
enum lab_ssd_mode {
	LAB_SSD_MODE_NONE = 0,
	LAB_SSD_MODE_BORDER,
	LAB_SSD_MODE_FULL,
	LAB_SSD_MODE_INVALID,
};
enum lab_tristate {
	LAB_STATE_UNSPECIFIED = 0,
	LAB_STATE_ENABLED,
	LAB_STATE_DISABLED
};
enum lab_view_criteria {
	LAB_VIEW_CRITERIA_NONE = 0,
	LAB_VIEW_CRITERIA_CURRENT_WORKSPACE       = 1 << 0,
	LAB_VIEW_CRITERIA_FULLSCREEN              = 1 << 1,
	LAB_VIEW_CRITERIA_ALWAYS_ON_TOP           = 1 << 2,
	LAB_VIEW_CRITERIA_NO_DIALOG               = 1 << 5,
	LAB_VIEW_CRITERIA_NO_ALWAYS_ON_TOP        = 1 << 6,
	LAB_VIEW_CRITERIA_NO_SKIP_WINDOW_SWITCHER = 1 << 7,
	LAB_VIEW_CRITERIA_NO_OMNIPRESENT          = 1 << 8,
};
enum lab_window_type {
	LAB_WINDOW_TYPE_INVALID = -1,
	LAB_WINDOW_TYPE_DESKTOP = 0,
	LAB_WINDOW_TYPE_DOCK,
	LAB_WINDOW_TYPE_TOOLBAR,
	LAB_WINDOW_TYPE_MENU,
	LAB_WINDOW_TYPE_UTILITY,
	LAB_WINDOW_TYPE_SPLASH,
	LAB_WINDOW_TYPE_DIALOG,
	LAB_WINDOW_TYPE_DROPDOWN_MENU,
	LAB_WINDOW_TYPE_POPUP_MENU,
	LAB_WINDOW_TYPE_TOOLTIP,
	LAB_WINDOW_TYPE_NOTIFICATION,
	LAB_WINDOW_TYPE_COMBO,
	LAB_WINDOW_TYPE_DND,
	LAB_WINDOW_TYPE_NORMAL,
	LAB_WINDOW_TYPE_LEN
};
#endif
#ifndef LABWC_PARSE_BOOL_H
#define LABWC_PARSE_BOOL_H
#include <stdbool.h>
enum lab_tristate parse_tristate(const char *str);
int parse_bool(const char *str, int default_value);
void set_bool(const char *str, bool *variable);
void set_bool_as_int(const char *str, int *variable);
#endif
#ifndef LABWC_SPAWN_H
#define LABWC_SPAWN_H
#include <sys/types.h>
pid_t spawn_primary_client(const char *command);
void spawn_async_no_shell(char const *command);
void spawn_sync_no_shell(char const *command);
pid_t spawn_piped(const char *command, int *pipe_fd);
void spawn_piped_close(pid_t pid, int pipe_fd);
#endif
#ifndef LABWC_STRING_HELPERS_H
#define LABWC_STRING_HELPERS_H
#include <stdbool.h>
bool string_null_or_empty(const char *s);
bool str_space_only(const char *s);
char *string_strip(char *s);
void string_truncate_at_pattern(char *buf, const char *pattern);
char *strdup_printf(const char *fmt, ...);
char *str_join(const char *const parts[], const char *fmt, const char *sep);
bool str_endswith(const char *const string, const char *const suffix);
bool str_endswith_ignore_case(const char *const string, const char *const suffix);
bool str_starts_with(const char *s, char needle, const char *ignore_chars);
bool str_equal(const char *a, const char *b);
#endif
#ifndef LABWC_BORDER_H
#define LABWC_BORDER_H
struct border {
	int top;
	int right;
	int bottom;
	int left;
};
#endif
#ifndef LABWC_FONT_H
#define LABWC_FONT_H
#include <cairo.h>
#include <pango/pango-font.h>
#include <stdbool.h>
struct lab_data_buffer;
struct font {
	char *name;
	int size;
	PangoStyle slant;
	PangoWeight weight;
};
struct _PangoFontDescription *font_to_pango_desc(struct font *font);
int font_height(struct font *font);
int font_width(struct font *font, const char *string);
void font_get_buffer_size(int max_width, const char *text, struct font *font,
	int *width, int *height);
void font_buffer_create(struct lab_data_buffer **buffer, int max_width,
	int height, const char *text, struct font *font, const float *color,
	cairo_pattern_t *bg_pattern, double scale, bool use_markup);
void font_finish(void);
#endif
#ifndef LABWC_EDGE_H
#define LABWC_EDGE_H
#include <stdbool.h>
enum lab_edge {
	LAB_EDGE_NONE = 0,
	LAB_EDGE_TOP = (1 << 0),
	LAB_EDGE_BOTTOM = (1 << 1),
	LAB_EDGE_LEFT = (1 << 2),
	LAB_EDGE_RIGHT = (1 << 3),
	LAB_EDGE_CENTER = (1 << 4),
	LAB_EDGE_ANY = (1 << 5),
	LAB_EDGES_TOP_LEFT = (LAB_EDGE_TOP | LAB_EDGE_LEFT),
	LAB_EDGES_TOP_RIGHT = (LAB_EDGE_TOP | LAB_EDGE_RIGHT),
	LAB_EDGES_BOTTOM_LEFT = (LAB_EDGE_BOTTOM | LAB_EDGE_LEFT),
	LAB_EDGES_BOTTOM_RIGHT = (LAB_EDGE_BOTTOM | LAB_EDGE_RIGHT),
	LAB_EDGES_TOP_BOTTOM = (LAB_EDGE_TOP | LAB_EDGE_BOTTOM),
	LAB_EDGES_LEFT_RIGHT = (LAB_EDGE_LEFT | LAB_EDGE_RIGHT),
	LAB_EDGES_ALL = (LAB_EDGE_TOP | LAB_EDGE_BOTTOM |
			LAB_EDGE_LEFT | LAB_EDGE_RIGHT),
	LAB_EDGES_EXCEPT_TOP = (LAB_EDGES_ALL ^ LAB_EDGE_TOP),
	LAB_EDGES_EXCEPT_BOTTOM = (LAB_EDGES_ALL ^ LAB_EDGE_BOTTOM),
	LAB_EDGES_EXCEPT_LEFT = (LAB_EDGES_ALL ^ LAB_EDGE_LEFT),
	LAB_EDGES_EXCEPT_RIGHT = (LAB_EDGES_ALL ^ LAB_EDGE_RIGHT),
};
enum lab_edge lab_edge_parse(const char *direction, bool tiled, bool any);
bool lab_edge_is_cardinal(enum lab_edge edge);
enum lab_edge lab_edge_invert(enum lab_edge edge);
#endif
#ifndef LABWC_NODE_TYPE_H
#define LABWC_NODE_TYPE_H
enum lab_node_type {
	LAB_NODE_NONE = 0,
	LAB_NODE_BUTTON_CLOSE = 1,
	LAB_NODE_BUTTON_MAXIMIZE,
	LAB_NODE_BUTTON_ICONIFY,
	LAB_NODE_BUTTON_WINDOW_MENU,
	LAB_NODE_BUTTON_SHADE,
	LAB_NODE_BUTTON_OMNIPRESENT,
	LAB_NODE_BUTTON_FIRST = LAB_NODE_BUTTON_CLOSE,
	LAB_NODE_BUTTON_LAST = LAB_NODE_BUTTON_OMNIPRESENT,
	LAB_NODE_BUTTON,
	LAB_NODE_TITLEBAR,
	LAB_NODE_CORNER_TOP_LEFT,
	LAB_NODE_CORNER_TOP_RIGHT,
	LAB_NODE_CORNER_BOTTOM_RIGHT,
	LAB_NODE_CORNER_BOTTOM_LEFT,
	LAB_NODE_BORDER_TOP,
	LAB_NODE_BORDER_RIGHT,
	LAB_NODE_BORDER_BOTTOM,
	LAB_NODE_BORDER_LEFT,
	LAB_NODE_BORDER,
	LAB_NODE_CLIENT,
	LAB_NODE_FRAME,
	LAB_NODE_ROOT,
	LAB_NODE_MENUITEM,
	LAB_NODE_LAYER_SURFACE,
	LAB_NODE_UNMANAGED,
	LAB_NODE_ALL,
	LAB_NODE_VIEW,
	LAB_NODE_XDG_POPUP,
	LAB_NODE_LAYER_POPUP,
	LAB_NODE_SESSION_LOCK_SURFACE,
	LAB_NODE_IME_POPUP,
	LAB_NODE_SSD_ROOT,
};
enum lab_node_type node_type_parse(const char *context);
bool node_type_contains(enum lab_node_type whole, enum lab_node_type part);
enum lab_edge node_type_to_edges(enum lab_node_type type);
#endif
#ifndef LABWC_RCXML_H
#define LABWC_RCXML_H
#include <stdbool.h>
#include <wayland-server-core.h>
#define TITLE_BUTTONS_MAX ((LAB_NODE_BUTTON_LAST + 1) - LAB_NODE_BUTTON_FIRST)
enum resize_indicator_mode {
	LAB_RESIZE_INDICATOR_NEVER = 0,
	LAB_RESIZE_INDICATOR_ALWAYS,
	LAB_RESIZE_INDICATOR_NON_PIXEL
};
enum tearing_mode {
	LAB_TEARING_DISABLED = 0,
	LAB_TEARING_ENABLED,
	LAB_TEARING_FULLSCREEN,
	LAB_TEARING_FULLSCREEN_FORCED,
};
enum tiling_events_mode {
	LAB_TILING_EVENTS_NEVER = 0,
	LAB_TILING_EVENTS_REGION = 1 << 0,
	LAB_TILING_EVENTS_EDGE = 1 << 1,
	LAB_TILING_EVENTS_ALWAYS =
		(LAB_TILING_EVENTS_REGION | LAB_TILING_EVENTS_EDGE),
};
struct buf;
struct usable_area_override {
	struct border margin;
	char *output;
	struct wl_list link;
};
struct workspace_config {
	struct wl_list link;
	char *name;
};
struct rcxml {
	char *config_dir;
	bool xdg_shell_server_side_deco;
	bool hide_maximized_window_titlebar;
	int gap;
	enum tearing_mode allow_tearing;
	bool auto_enable_outputs;
	bool reuse_output_mode;
	uint32_t allowed_interfaces;
	bool xwayland_persistence;
	bool primary_selection;
	enum lab_placement_policy placement_policy;
	int placement_cascade_offset_x;
	int placement_cascade_offset_y;
	bool focus_follow_mouse;
	bool focus_follow_mouse_requires_movement;
	bool raise_on_focus;
	uint32_t raise_on_focus_delay_ms;
	enum lab_node_type title_buttons_left[TITLE_BUTTONS_MAX];
	int nr_title_buttons_left;
	enum lab_node_type title_buttons_right[TITLE_BUTTONS_MAX];
	int nr_title_buttons_right;
	int corner_radius;
	bool ssd_keep_border;
	bool shadows_enabled;
	bool shadows_on_tiled;
	struct font font_activewindow;
	struct font font_inactivewindow;
	struct font font_menuheader;
	struct font font_menuitem;
	struct font font_osd;
	struct theme *theme;
	struct wl_list usable_area_overrides;
	int repeat_rate;
	int repeat_delay;
	enum lab_tristate kb_numlock_enable;
	bool kb_layout_per_window;
	struct wl_list keybinds;
	long doubleclick_time;
	struct wl_list mousebinds;
	struct wl_list libinput_categories;
	int screen_edge_strength;
	int window_edge_strength;
	int unsnap_threshold;
	int unmaximize_threshold;
	int snap_edge_range_inner;
	int snap_edge_range_outer;
	int snap_edge_corner_range;
	bool snap_overlay_enabled;
	int snap_overlay_delay_inner;
	int snap_overlay_delay_outer;
	bool snap_top_maximize;
	enum tiling_events_mode snap_tiling_events_mode;
	enum resize_indicator_mode resize_indicator;
	bool resize_draw_contents;
	int resize_corner_range;
	int resize_minimum_area;
	struct {
		int popuptime;
		char *initial_workspace_name;
		struct wl_list workspaces;
	} workspace_config;
	struct wl_list regions;
	struct wl_list window_rules;
	unsigned int menu_ignore_button_release_period;
};
extern struct rcxml rc;
void rcxml_init(void);
void rcxml_finish(void);
uint32_t parse_privileged_interface(const char *name);
#endif
#ifndef LABWC_INPUT_H
#define LABWC_INPUT_H
#include <wayland-server-core.h>
struct input {
	struct wlr_input_device *wlr_input_device;
	struct seat *seat;
	double scroll_factor;
	struct wl_listener destroy;
	struct wl_list link;
};
void input_handlers_init(struct seat *seat);
void input_handlers_finish(struct seat *seat);
#endif
#ifndef LABWC_KEYBOARD_H
#define LABWC_KEYBOARD_H
#include <stdbool.h>
#include <xkbcommon/xkbcommon.h>
struct keyboard {
	struct input base;
	struct wlr_keyboard *wlr_keyboard;
	bool is_virtual;
	struct wl_listener modifiers;
	struct wl_listener key;
	uint32_t keybind_repeat_keycode;
	int32_t keybind_repeat_rate;
	struct wl_event_source *keybind_repeat;
};
void keyboard_reset_current_keybind(void);
void keyboard_configure(struct seat *seat, struct wlr_keyboard *kb,
	bool is_virtual);
void keyboard_group_init(struct seat *seat);
void keyboard_group_finish(struct seat *seat);
void keyboard_setup_handlers(struct keyboard *keyboard);
void keyboard_set_numlock(struct wlr_keyboard *keyboard);
void keyboard_update_layout(struct seat *seat, xkb_layout_index_t layout);
void keyboard_cancel_keybind_repeat(struct keyboard *keyboard);
uint32_t keyboard_get_all_modifiers(struct seat *seat);
#endif
#ifndef LABWC_KEY_STATE_H
#define LABWC_KEY_STATE_H
#include <stdbool.h>
#include <stdint.h>
struct seat;
void key_state_indicator_update(struct seat *seat);
void key_state_indicator_toggle(void);
uint32_t *key_state_pressed_sent_keycodes(void);
int key_state_nr_pressed_sent_keycodes(void);
void key_state_set_pressed(uint32_t keycode, bool is_pressed);
void key_state_store_pressed_key_as_bound(uint32_t keycode);
bool key_state_corresponding_press_event_was_bound(uint32_t keycode);
void key_state_bound_key_remove(uint32_t keycode);
int key_state_nr_bound_keys(void);
#endif
#define PACKAGE_NAME "jtlabwc"
#define PACKAGE_VERSION "0.20.2"
#define LABWC_VERSION "0.20.2"
#define SYSCONFDIR "/etc"
#define XDG_CONFIG_DIR "/etc/xdg"
#define XDG_DATA_DIR "/usr/local/share"
#define XDG_CONFIG_HOME "/home/lynch/.config"
#define XDG_DATA_HOME "/home/lynch/.local/share"
#define XDG_STATE_HOME "/home/lynch/.local/state"
#define XDG_CACHE_HOME "/home/lynch/.cache"
#define XDG_RUNTIME_DIR "/run/user/1000"
#define XDG_CONFIG_DIRS "/etc/xdg"
#define XDG_DATA_DIRS "/usr/local/share:/usr/share"
#ifdef XWAYLAND
#define HAVE_XWAYLAND 1
#else
#define HAVE_XWAYLAND 0
#endif
#define HAVE_LIBINPUT_CONFIG_QUIRKS 1
#define HAVE_LIBINPUT_CONFIG_ACCEL_PROFILE_CUSTOM_SET_ENABLED 1
#ifndef LABWC_SET_H
#define LABWC_SET_H
#include <stdbool.h>
#include <stdint.h>
#define LAB_SET_MAX_SIZE 16
struct lab_set {
	uint32_t values[LAB_SET_MAX_SIZE];
	int size;
};
bool lab_set_contains(struct lab_set *set, uint32_t value);
void lab_set_add(struct lab_set *set, uint32_t value);
void lab_set_remove(struct lab_set *set, uint32_t value);
#endif
#ifndef LABWC_CURSOR_H
#define LABWC_CURSOR_H
#include <wayland-server-protocol.h>
struct view;
struct seat;
struct server;
struct wlr_input_device;
struct wlr_cursor;
struct wlr_surface;
struct wlr_scene_node;
enum lab_cursors {
	LAB_CURSOR_CLIENT = 0,
	LAB_CURSOR_DEFAULT,
	LAB_CURSOR_GRAB,
	LAB_CURSOR_RESIZE_NW,
	LAB_CURSOR_RESIZE_N,
	LAB_CURSOR_RESIZE_NE,
	LAB_CURSOR_RESIZE_E,
	LAB_CURSOR_RESIZE_SE,
	LAB_CURSOR_RESIZE_S,
	LAB_CURSOR_RESIZE_SW,
	LAB_CURSOR_RESIZE_W,
	LAB_CURSOR_COUNT
};
struct cursor_context {
	struct view *view;
	struct wlr_scene_node *node;
	struct wlr_surface *surface;
	enum lab_node_type type;
	double sx, sy;
};
struct cursor_context_saved {
	struct cursor_context ctx;
	struct wl_listener view_destroy;
	struct wl_listener node_destroy;
	struct wl_listener surface_destroy;
};
struct cursor_context get_cursor_context(void);
void cursor_set(struct seat *seat, enum lab_cursors cursor);
void cursor_set_visible(struct seat *seat, bool visible);
void cursor_context_save(struct cursor_context_saved *saved_ctx,
	const struct cursor_context *ctx);
enum lab_edge cursor_get_resize_edges(struct wlr_cursor *cursor,
	const struct cursor_context *ctx);
enum lab_cursors cursor_get_from_edge(enum lab_edge resize_edges);
void cursor_update_focus(void);
void cursor_update_image(struct seat *seat);
bool cursor_process_motion(uint32_t time, double *sx, double *sy);
bool cursor_process_button_press(struct seat *seat, uint32_t button, uint32_t time_msec);
bool cursor_process_button_release(struct seat *seat, uint32_t button, uint32_t time_msec);
bool cursor_finish_button_release(struct seat *seat, uint32_t button);
void cursor_init(struct seat *seat);
void cursor_reload(struct seat *seat);
void cursor_emulate_move(struct seat *seat,
		struct wlr_input_device *device,
		double dx, double dy, uint32_t time_msec);
void cursor_emulate_move_absolute(struct seat *seat,
		struct wlr_input_device *device,
		double x, double y, uint32_t time_msec);
void cursor_emulate_button(struct seat *seat,
		uint32_t button, enum wl_pointer_button_state state, uint32_t time_msec);
void cursor_emulate_axis(struct seat *seat,
		struct wlr_input_device *device,
		enum wl_pointer_axis orientation, double delta, double delta_discrete,
		enum wl_pointer_axis_source source, uint32_t time_msec);
void cursor_finish(struct seat *seat);
#endif
#ifndef LABWC_OVERLAY_H
#define LABWC_OVERLAY_H
struct seat;
struct overlay {
	struct lab_scene_rect *rect;
	struct {
		struct region *region;
		enum lab_edge edge;
		struct output *output;
	} active;
	struct wl_event_source *timer;
};
void overlay_update(struct seat *seat);
void overlay_finish(struct seat *seat);
#endif
#ifndef LABWC_H
#define LABWC_H
#define XCURSOR_DEFAULT "left_ptr"
#define XCURSOR_SIZE 24
struct wlr_xdg_popup;
enum input_mode {
	LAB_INPUT_STATE_PASSTHROUGH = 0,
	LAB_INPUT_STATE_MOVE,
	LAB_INPUT_STATE_RESIZE,
	LAB_INPUT_STATE_MENU,
};
struct seat {
	struct wlr_seat *wlr_seat;
	struct wlr_keyboard_group *keyboard_group;
	enum lab_cursors server_cursor;
	bool cursor_visible;
	struct wlr_cursor *cursor;
	struct wlr_xcursor_manager *xcursor_manager;
	struct accumulated_scroll {
		double delta;
		double delta_discrete;
	} accumulated_scrolls[2];
	bool cursor_scroll_wheel_emulation;
	struct {
		struct wlr_surface *surface;
		struct wl_listener surface_destroy;
	} focus_override;
	struct wlr_pointer_constraint_v1 *current_constraint;
	struct wl_event_source *workspace_osd_timer;
	bool workspace_osd_shown_by_modifier;
	struct wlr_layer_surface_v1 *focused_layer;
	struct input_method_relay *input_method_relay;
	struct cursor_context_saved pressed;
	struct cursor_context_saved last_cursor_ctx;
	struct lab_set bound_buttons;
	struct {
		bool active;
		struct {
			struct wl_listener request;
			struct wl_listener start;
			struct wl_listener destroy;
		} events;
		struct wlr_scene_tree *icons;
	} drag;
	struct overlay overlay;
	bool region_prevent_snap;
	struct wl_list inputs;
	struct wl_listener new_input;
	struct wl_listener focus_change;
	struct {
		struct wl_listener motion;
		struct wl_listener motion_absolute;
		struct wl_listener button;
		struct wl_listener axis;
		struct wl_listener frame;
	} on_cursor;
	struct wlr_pointer_gestures_v1 *pointer_gestures;
	struct wl_listener pinch_begin;
	struct wl_listener pinch_update;
	struct wl_listener pinch_end;
	struct wl_listener swipe_begin;
	struct wl_listener swipe_update;
	struct wl_listener swipe_end;
	struct wl_listener hold_begin;
	struct wl_listener hold_end;
	struct wl_listener request_set_cursor;
	struct wl_listener request_set_shape;
	struct wl_listener request_set_selection;
	struct wl_listener request_set_primary_selection;
	struct wl_listener constraint_commit;
	struct wlr_virtual_pointer_manager_v1 *virtual_pointer;
	struct wl_listener new_virtual_pointer;
	struct wlr_virtual_keyboard_manager_v1 *virtual_keyboard;
	struct wl_listener new_virtual_keyboard;
};
struct server {
	uint32_t wlr_version;
	struct wl_display *wl_display;
	struct wl_event_loop *wl_event_loop;
	struct view *pending_auto_raise_view;
	struct wl_event_source *pending_auto_raise_timer;
	struct wlr_renderer *renderer;
	struct wlr_allocator *allocator;
	struct wlr_backend *backend;
	struct headless {
		struct wlr_backend *backend;
	} headless;
	struct wlr_session *session;
	struct wlr_linux_dmabuf_v1 *linux_dmabuf;
	struct wlr_compositor *compositor;
	struct wl_event_source *sigint_source;
	struct wl_event_source *sigterm_source;
	struct wl_event_source *sigchld_source;
	struct wlr_xdg_shell *xdg_shell;
	struct wlr_layer_shell_v1 *layer_shell;
	struct wl_listener new_xdg_toplevel;
	struct wl_listener new_layer_surface;
	struct wl_listener kde_server_decoration;
	struct wl_listener xdg_toplevel_decoration;
#if HAVE_XWAYLAND
	struct wlr_xwayland *xwayland;
	struct wl_listener xwayland_xwm_ready;
	struct wl_listener xwayland_new_surface;
#endif
	struct wlr_xdg_activation_v1 *xdg_activation;
	struct wl_listener xdg_activation_request;
	struct wl_listener xdg_activation_new_token;
	struct {
		struct wlr_ext_foreign_toplevel_image_capture_source_manager_v1 *manager;
		struct {
			struct wl_listener new_request;
		} on;
	} toplevel_capture;
	struct wl_list views;
	uint64_t next_view_creation_id;
	struct wl_list unmanaged_surfaces;
	struct seat seat;
	struct wlr_scene *scene;
	struct wlr_scene_output_layout *scene_layout;
	bool direct_scanout_enabled;
	enum input_mode input_mode;
	struct view *grabbed_view;
	double grab_x, grab_y;
	struct wlr_box grab_box;
	enum lab_edge resize_edges;
	struct view *active_view;
	struct ssd_button *hovered_button;
	struct wlr_scene_tree *workspace_tree;
	struct wlr_scene_tree *xdg_popup_tree;
#if HAVE_XWAYLAND
	struct wlr_scene_tree *unmanaged_tree;
#endif
	struct wlr_scene_tree *menu_tree;
	struct {
		struct wl_list all;
		struct workspace *current;
		struct workspace *last;
		struct wlr_ext_workspace_manager_v1 *ext_manager;
		struct wlr_ext_workspace_group_handle_v1 *ext_group;
		struct {
			struct wl_listener layout_output_added;
		} on;
		struct {
			struct wl_listener commit;
		} on_ext_manager;
	} workspaces;
	struct wl_list outputs;
	struct wl_listener new_output;
	struct wlr_output_layout *output_layout;
	uint64_t next_output_id_bit;
	struct wl_listener output_layout_change;
	struct wlr_output_manager_v1 *output_manager;
	struct wl_listener output_manager_test;
	struct wl_listener output_manager_apply;
	int pending_output_layout_change;
	struct wl_listener renderer_lost;
	struct wlr_gamma_control_manager_v1 *gamma_control_manager_v1;
	struct session_lock_manager *session_lock_manager;
	struct wlr_foreign_toplevel_manager_v1 *foreign_toplevel_manager;
	struct wlr_ext_foreign_toplevel_list_v1 *foreign_toplevel_list;
	struct wlr_drm_lease_v1_manager *drm_lease_manager;
	struct wl_listener drm_lease_request;
	struct wlr_output_power_manager_v1 *output_power_manager_v1;
	struct wl_listener output_power_manager_set_mode;
	struct wlr_relative_pointer_manager_v1 *relative_pointer_manager;
	struct wlr_pointer_constraints_v1 *constraints;
	struct wl_listener new_constraint;
#ifdef FULLSCREEN_TEARING
	struct wlr_tearing_control_manager_v1 *tearing_control;
	struct wl_listener tearing_new_object;
#endif
	struct wlr_input_method_manager_v2 *input_method_manager;
	struct wlr_text_input_manager_v3 *text_input_manager;
	struct wlr_security_context_manager_v1 *security_context_manager_v1;
	struct menu *menu_current;
	struct wl_list menus;
	pid_t primary_client_pid;
	char *title_fmt;
};
extern struct server server;
void xdg_popup_create(struct view *view, struct wlr_xdg_popup *wlr_popup);
void xdg_shell_init(void);
void xdg_shell_finish(void);
void desktop_focus_view(struct view *view, bool raise);
void desktop_cancel_pending_auto_raise(void);
void desktop_focus_view_or_surface(struct seat *seat, struct view *view,
	struct wlr_surface *surface, bool raise);
void desktop_arrange_all_views(void);
void desktop_focus_output(struct output *output);
void desktop_update_top_layer_visibility(void);
void desktop_focus_topmost_view(void);
void seat_init(void);
void seat_finish(void);
void seat_reconfigure(void);
void seat_force_focus_surface(struct seat *seat, struct wlr_surface *surface);
void seat_focus_surface(struct seat *seat, struct wlr_surface *surface);
void seat_pointer_end_grab(struct seat *seat, struct wlr_surface *surface);
void seat_focus_lock_surface(struct seat *seat, struct wlr_surface *surface);
void seat_set_focus_layer(struct seat *seat, struct wlr_layer_surface_v1 *layer);
void seat_output_layout_changed(struct seat *seat);
void seat_focus_override_begin(struct seat *seat, enum input_mode input_mode,
	enum lab_cursors cursor_shape);
void seat_focus_override_end(struct seat *seat, bool restore_focus);
void interactive_anchor_to_cursor(struct wlr_box *geo);
void interactive_set_grab_context(const struct cursor_context *ctx);
void interactive_begin(struct view *view, enum input_mode mode,
	enum lab_edge edges);
void interactive_finish(struct view *view);
void interactive_cancel(struct view *view);
bool edge_from_cursor(struct seat *seat, struct output **dest_output,
	enum lab_edge *edge1, enum lab_edge *edge2);
#ifdef FULLSCREEN_TEARING
void handle_tearing_new_object(struct wl_listener *listener, void *data);
#endif
void server_init(void);
void server_start(void);
void server_finish(void);
void create_constraint(struct wl_listener *listener, void *data);
void constrain_cursor(struct wlr_pointer_constraint_v1
	*constraint);
#endif
#ifndef LABWC_MENU_H
#define LABWC_MENU_H
#include <wayland-server.h>
struct view;
struct server;
struct wl_list;
struct wlr_scene_tree;
struct wlr_scene_node;
struct scaled_font_buffer;
enum menuitem_type {
	LAB_MENU_ITEM = 0,
	LAB_MENU_SEPARATOR_LINE,
	LAB_MENU_TITLE,
};
struct menuitem {
	struct wl_list actions;
	char *text;
	const char *arrow;
	uint32_t accelerator;
	struct menu *parent;
	struct menu *submenu;
	bool selectable;
	bool use_markup;
	enum menuitem_type type;
	int native_width;
	struct wlr_scene_tree *tree;
	struct wlr_scene_tree *normal_tree;
	struct wlr_scene_tree *selected_tree;
	struct view *client_list_view;
	struct wl_list link;
};
struct menu {
	char *id;
	char *label;
	struct menu *parent;
	struct {
		int width;
		int height;
	} size;
	struct wl_list menuitems;
	struct {
		struct menu *menu;
		struct menuitem *item;
	} selection;
	struct wlr_scene_tree *scene_tree;
	bool align_left;
	struct view *triggered_by_view;
	struct wl_list link;
};
void menu_item_select_next(void);
void menu_item_select_previous(void);
bool menu_item_select_by_accelerator(uint32_t accelerator);
void menu_submenu_enter(void);
void menu_submenu_leave(void);
bool menu_call_selected_actions(void);
void menu_init(void);
void menu_finish(void);
void menu_on_view_destroy(struct view *view);
struct menu *menu_get_by_id(const char *id);
void menu_open(struct menu *menu, int x, int y);
void menu_process_cursor_motion(struct wlr_scene_node *node);
void menu_close(void);
void menu_reconfigure(void);
#endif
#ifndef LABWC_OUTPUT_H
#define LABWC_OUTPUT_H
#define LAB_NR_LAYERS (4)
struct output {
	struct wl_list link;
	struct wlr_output *wlr_output;
	struct wlr_output_state pending;
	struct wlr_scene_output *scene_output;
	struct wlr_scene_tree *layer_tree[LAB_NR_LAYERS];
	struct wlr_scene_tree *layer_popup_tree;
	struct wlr_scene_tree *session_lock_tree;
	struct wlr_scene_buffer *workspace_osd;
	struct wlr_box usable_area;
	struct wl_list regions;
	struct wl_listener destroy;
	struct wl_listener frame;
	struct wl_listener request_state;
	uint64_t id_bit;
};
#undef LAB_NR_LAYERS
void output_init(void);
void output_finish(void);
struct output *output_from_wlr_output(struct wlr_output *wlr_output);
struct output *output_from_name(const char *name);
struct output *output_nearest_to(int lx, int ly);
struct output *output_nearest_to_cursor(void);
struct output *output_get_adjacent(struct output *output,
	enum lab_edge edge, bool wrap);
bool output_is_usable(struct output *output);
void output_update_usable_area(struct output *output);
void output_update_all_usable_areas(bool layout_changed);
bool output_get_tearing_allowance(struct output *output);
struct wlr_box output_usable_area_in_layout_coords(struct output *output);
void handle_output_power_manager_set_mode(struct wl_listener *listener,
	void *data);
#endif
#ifndef LABWC_OUTPUT_VIRTUAL_H
#define LABWC_OUTPUT_VIRTUAL_H
struct server;
struct wlr_output;
void output_virtual_add(const char *output_name,
		struct wlr_output **store_wlr_output);
void output_virtual_remove(const char *output_name);
void output_virtual_update_fallback(void);
#endif
#ifndef LABWC_REGIONS_H
#define LABWC_REGIONS_H
struct seat;
struct view;
struct server;
struct output;
struct wl_list;
struct wlr_box;
struct multi_rect;
struct region {
	struct wl_list link;
	struct output *output;
	char *name;
	struct wlr_box geo;
	struct wlr_box percentage;
	struct {
		int x;
		int y;
	} center;
};
bool regions_should_snap(void);
void regions_reconfigure(void);
void regions_reconfigure_output(struct output *output);
void regions_update_geometry(struct output *output);
void regions_evacuate_output(struct output *output);
void regions_destroy(struct seat *seat, struct wl_list *regions);
struct region *regions_from_cursor(void);
struct region *regions_from_name(const char *region_name, struct output *output);
#endif
#ifndef LABWC_SHOW_DESKTOP_H
#define LABWC_SHOW_DESKTOP_H
void show_desktop_toggle(void);
void show_desktop_reset(void);
#endif
#ifndef LABWC_SSD_H
#define LABWC_SSD_H
enum ssd_active_state {
	SSD_INACTIVE = 0,
	SSD_ACTIVE = 1,
};
#define FOR_EACH_ACTIVE_STATE(active) for (active = SSD_INACTIVE; active <= SSD_ACTIVE; active++)
struct wlr_cursor;
#define SSD_SHADOW_INSET 0.3
struct server;
struct ssd;
struct ssd_button;
struct view;
struct wlr_scene;
struct wlr_scene_node;
struct ssd *ssd_create(struct view *view, bool active);
struct border ssd_get_margin(const struct ssd *ssd);
int ssd_get_corner_width(void);
void ssd_update_margin(struct ssd *ssd);
void ssd_set_active(struct ssd *ssd, bool active);
void ssd_update_geometry(struct ssd *ssd);
void ssd_destroy(struct ssd *ssd);
void ssd_set_titlebar(struct ssd *ssd, bool enabled);
void ssd_enable_keybind_inhibit_indicator(struct ssd *ssd, bool enable);
void ssd_enable_shade(struct ssd *ssd, bool enable);
void ssd_update_hovered_button(struct wlr_scene_node *node);
void ssd_button_free(struct ssd_button *button);
enum lab_node_type ssd_get_resizing_type(const struct ssd *ssd,
	struct wlr_cursor *cursor);
enum lab_ssd_mode ssd_mode_parse(const char *mode);
struct border ssd_thickness(struct view *view);
struct wlr_box ssd_max_extents(struct view *view);
#endif
#ifndef LABWC_THEME_H
#define LABWC_THEME_H
#include <cairo.h>
#include <stdbool.h>
struct lab_img;
enum lab_gradient {
	LAB_GRADIENT_NONE,
	LAB_GRADIENT_VERTICAL,
	LAB_GRADIENT_SPLITVERTICAL,
};
enum lab_justification {
	LAB_JUSTIFY_LEFT,
	LAB_JUSTIFY_CENTER,
	LAB_JUSTIFY_RIGHT,
};
struct theme_snapping_overlay {
	bool bg_enabled;
	bool border_enabled;
	float bg_color[4];
	int border_width;
	float border_color[3][4];
};
enum lab_button_state {
	LAB_BS_DEFAULT = 0,
	LAB_BS_HOVERED = 1 << 0,
	LAB_BS_TOGGLED = 1 << 1,
	LAB_BS_ROUNDED = 1 << 2,
	LAB_BS_ALL = LAB_BS_HOVERED | LAB_BS_TOGGLED | LAB_BS_ROUNDED,
};
struct theme_background {
	enum lab_gradient gradient;
	float color[4];
	float color_split_to[4];
	float color_to[4];
	float color_to_split_to[4];
};
struct theme {
	int border_width;
	int window_titlebar_padding_width;
	int window_titlebar_padding_height;
	int titlebar_height;
	float window_toggled_keybinds_color[4];
	enum lab_justification window_label_text_justify;
	int window_button_width;
	int window_button_height;
	int window_button_spacing;
	float window_button_hover_bg_color[4];
	int window_button_hover_bg_corner_radius;
	struct {
		struct theme_background title_bg;
		float button_colors[LAB_NODE_BUTTON_LAST + 1][4];
		float border_color[4];
		float toggled_keybinds_color[4];
		float label_text_color[4];
		int shadow_size;
		float shadow_color[4];
		struct lab_img *button_imgs
			[LAB_NODE_BUTTON_LAST + 1][LAB_BS_ALL + 1];
		cairo_pattern_t *titlebar_pattern;
		struct lab_data_buffer *titlebar_fill;
		struct lab_data_buffer *corner_top_left_normal;
		struct lab_data_buffer *corner_top_right_normal;
		struct lab_data_buffer *shadow_corner_top;
		struct lab_data_buffer *shadow_corner_bottom;
		struct lab_data_buffer *shadow_edge;
	} window[2];
	int menu_item_height;
	int menu_header_height;
	int menu_overlap_x;
	int menu_overlap_y;
	int menu_min_width;
	int menu_max_width;
	int menu_border_width;
	float menu_border_color[4];
	int menu_items_padding_x;
	int menu_items_padding_y;
	float menu_items_bg_color[4];
	float menu_items_text_color[4];
	float menu_items_active_bg_color[4];
	float menu_items_active_text_color[4];
	int menu_separator_line_thickness;
	int menu_separator_padding_width;
	int menu_separator_padding_height;
	float menu_separator_color[4];
	float menu_title_bg_color[4];
	enum lab_justification menu_title_text_justify;
	float menu_title_text_color[4];
	int osd_border_width;
	float osd_bg_color[4];
	float osd_border_color[4];
	float osd_label_text_color[4];
	int osd_workspace_switcher_boxes_width;
	int osd_workspace_switcher_boxes_height;
	int osd_workspace_switcher_boxes_border_width;
	struct theme_snapping_overlay
		snapping_overlay_region, snapping_overlay_edge;
};
void theme_init(struct theme *theme);
void theme_finish(struct theme *theme);
#endif
#ifndef LABWC_VIEW_H
#define LABWC_VIEW_H
#include <stdbool.h>
#include <stdint.h>
#include <wayland-util.h>
#include <xkbcommon/xkbcommon.h>
#define LAB_MIN_VIEW_WIDTH 100
#define LAB_MIN_VIEW_HEIGHT 60
#define VIEW_FALLBACK_X 100
#define VIEW_FALLBACK_Y 100
#define VIEW_FALLBACK_WIDTH  640
#define VIEW_FALLBACK_HEIGHT 480
enum view_type {
	LAB_XDG_SHELL_VIEW,
#if HAVE_XWAYLAND
	LAB_XWAYLAND_VIEW,
#endif
};
enum ssd_preference {
	LAB_SSD_PREF_UNSPEC = 0,
	LAB_SSD_PREF_CLIENT,
	LAB_SSD_PREF_SERVER,
};
enum view_axis {
	VIEW_AXIS_NONE = 0,
	VIEW_AXIS_HORIZONTAL = (1 << 0),
	VIEW_AXIS_VERTICAL = (1 << 1),
	VIEW_AXIS_BOTH = (VIEW_AXIS_HORIZONTAL | VIEW_AXIS_VERTICAL),
	VIEW_AXIS_INVALID = (1 << 2),
};
enum view_wants_focus {
	VIEW_WANTS_FOCUS_NEVER = 0,
	VIEW_WANTS_FOCUS_ALWAYS,
	VIEW_WANTS_FOCUS_LIKELY,
	VIEW_WANTS_FOCUS_UNLIKELY,
};
enum view_layer {
	VIEW_LAYER_NORMAL = 0,
	VIEW_LAYER_ALWAYS_ON_TOP,
	VIEW_LAYER_ALWAYS_ON_BOTTOM,
};
struct view;
struct wlr_surface;
struct foreign_toplevel;
struct mappable {
	bool connected;
	struct wl_listener map;
	struct wl_listener unmap;
};
struct view_size_hints {
	int min_width;
	int min_height;
	int width_inc;
	int height_inc;
	int base_width;
	int base_height;
};
struct view_impl {
	void (*configure)(struct view *view, struct wlr_box geo);
	void (*close)(struct view *view);
	void (*set_activated)(struct view *view, bool activated);
	void (*set_fullscreen)(struct view *view, bool fullscreen);
	void (*notify_tiled)(struct view *view);
	void (*maximize)(struct view *view, enum view_axis maximized);
	void (*minimize)(struct view *view, bool minimize);
	struct view *(*get_parent)(struct view *self);
	struct view *(*get_root)(struct view *self);
	void (*append_children)(struct view *self, struct wl_array *children);
	bool (*is_modal_dialog)(struct view *self);
	struct view_size_hints (*get_size_hints)(struct view *self);
	enum view_wants_focus (*wants_focus)(struct view *self);
	void (*offer_focus)(struct view *self);
	bool (*has_strut_partial)(struct view *self);
	bool (*contains_window_type)(struct view *view,
		enum lab_window_type window_type);
	pid_t (*get_pid)(struct view *view);
};
struct view {
	enum view_type type;
	const struct view_impl *impl;
	struct wl_list link;
	struct wl_list cycle_link;
	struct output *output;
	uint64_t outputs;
	struct workspace *workspace;
	struct wlr_surface *surface;
	struct wlr_scene_tree *scene_tree;
	struct wlr_scene_tree *content_tree;
	char *title;
	char *app_id;
	struct {
		struct wlr_scene *scene;
		struct wlr_ext_image_capture_source_v1 *source;
		struct wl_listener on_capture_source_destroy;
	} capture;
	bool mapped;
	bool been_mapped;
	uint64_t creation_id;
	enum lab_ssd_mode ssd_mode;
	enum ssd_preference ssd_preference;
	bool shaded;
	bool minimized;
	bool was_minimized_by_show_desktop_action;
	enum view_axis maximized;
	bool fullscreen;
	bool tearing_hint;
	enum lab_tristate force_tearing;
	enum view_layer layer;
	bool visible_on_all_workspaces;
	enum lab_edge tiled;
	enum lab_edge edges_visible;
	bool inhibits_keybinds;
	xkb_layout_index_t keyboard_layout;
	struct region *tiled_region;
	char *tiled_region_evacuate;
	struct wlr_box current;
	struct wlr_box pending;
	struct wlr_box natural_geometry;
	struct {
		char *output_name;
		struct wlr_box relative_geo;
		struct wlr_box layout_geo;
	} last_placement;
	bool adjusting_for_layout_change;
	uint32_t pending_configure_serial;
	struct wl_event_source *pending_configure_timeout;
	struct ssd *ssd;
	struct resize_indicator {
		int width, height;
		struct wlr_scene_tree *tree;
		struct wlr_scene_rect *border;
		struct wlr_scene_rect *background;
		struct scaled_font_buffer *text;
	} resize_indicator;
	struct resize_outlines {
		struct wlr_box view_geo;
		struct lab_scene_rect *rect;
	} resize_outlines;
	struct mappable mappable;
	struct wl_listener destroy;
	struct wl_listener commit;
	struct wl_listener request_move;
	struct wl_listener request_resize;
	struct wl_listener request_minimize;
	struct wl_listener request_maximize;
	struct wl_listener request_fullscreen;
	struct wl_listener set_title;
	struct foreign_toplevel *foreign_toplevel;
	struct {
		struct wl_signal new_app_id;
		struct wl_signal new_title;
		struct wl_signal new_outputs;
		struct wl_signal maximized;
		struct wl_signal minimized;
		struct wl_signal fullscreened;
		struct wl_signal activated;
		struct wl_signal always_on_top;
		struct wl_signal destroy;
	} events;
};
struct view_query {
	struct wl_list link;
	char *identifier;
	char *title;
	enum lab_window_type window_type;
	char *sandbox_engine;
	char *sandbox_app_id;
	enum lab_tristate shaded;
	enum view_axis maximized;
	enum lab_tristate iconified;
	enum lab_tristate focused;
	enum lab_tristate omnipresent;
	enum lab_edge tiled;
	char *tiled_region;
	char *desktop;
	enum lab_ssd_mode decoration;
	char *monitor;
};
struct xdg_toplevel_view {
	struct view base;
	struct wlr_xdg_surface *xdg_surface;
	struct wlr_scene_rect *fullscreen_bg;
	struct wl_listener set_app_id;
	struct wl_listener request_show_window_menu;
	struct wl_listener set_parent;
	struct wl_listener new_popup;
};
struct view *view_from_wlr_surface(struct wlr_surface *surface);
struct view_query *view_query_create(void);
void view_query_free(struct view_query *view);
bool view_matches_query(struct view *view, struct view_query *query);
#define for_each_view(view, head, criteria)           \
	for (view = view_next(head, NULL, criteria);  \
	     view;                                    \
	     view = view_next(head, view, criteria))
#define for_each_view_reverse(view, head, criteria)   \
	for (view = view_prev(head, NULL, criteria);  \
	     view;                                    \
	     view = view_prev(head, view, criteria))
struct view *view_next(struct wl_list *head, struct view *view,
	enum lab_view_criteria criteria);
struct view *view_prev(struct wl_list *head, struct view *view,
	enum lab_view_criteria criteria);
void view_array_append(struct wl_array *views,
	enum lab_view_criteria criteria);
enum view_wants_focus view_wants_focus(struct view *view);
struct wlr_box view_get_edge_snap_box(struct view *view, struct output *output,
	enum lab_edge edge);
struct wlr_box view_get_region_snap_box(struct view *view, struct region *region);
bool view_is_focusable(struct view *view);
void view_offer_focus(struct view *view);
struct wlr_box view_get_edge_snap_box(struct view *view, struct output *output,
	enum lab_edge edge);
void mappable_connect(struct mappable *mappable, struct wlr_surface *surface,
	wl_notify_func_t notify_map, wl_notify_func_t notify_unmap);
void mappable_disconnect(struct mappable *mappable);
void view_toggle_keybinds(struct view *view);
bool view_inhibits_actions(struct view *view, struct wl_list *actions);
void view_set_activated(struct view *view, bool activated);
void view_set_output(struct view *view, struct output *output);
void view_close(struct view *view);
void view_move_resize(struct view *view, struct wlr_box geo);
void view_resize_relative(struct view *view,
	int left, int right, int top, int bottom);
void view_move_relative(struct view *view, int x, int y);
void view_move(struct view *view, int x, int y);
void view_moved(struct view *view);
void view_minimize(struct view *view, bool minimized);
bool view_compute_centered_position(struct view *view,
	const struct wlr_box *ref, int w, int h, int *x, int *y);
struct wlr_box view_get_fallback_natural_geometry(struct view *view);
void view_store_natural_geometry(struct view *view);
void view_apply_natural_geometry(struct view *view);
int view_effective_height(struct view *view, bool use_pending);
void view_center(struct view *view, const struct wlr_box *ref);
bool view_compute_position_by_policy(struct view *view, struct wlr_box *geom,
	bool allow_cursor, enum lab_placement_policy policy);
void view_place_by_policy(struct view *view, bool allow_cursor,
	enum lab_placement_policy policy);
void view_constrain_size_to_that_of_usable_area(struct view *view);
void view_set_maximized(struct view *view, enum view_axis maximized);
void view_set_untiled(struct view *view);
void view_maximize(struct view *view, enum view_axis axis);
void view_set_fullscreen(struct view *view, bool fullscreen);
void view_toggle_maximize(struct view *view, enum view_axis axis);
bool view_wants_decorations(struct view *view);
void view_toggle_decorations(struct view *view);
void view_set_layer(struct view *view, enum view_layer layer);
void view_toggle_always_on_top(struct view *view);
void view_toggle_always_on_bottom(struct view *view);
void view_toggle_visible_on_all_workspaces(struct view *view);
bool view_is_tiled(struct view *view);
bool view_is_tiled_and_notify_tiled(struct view *view);
bool view_is_floating(struct view *view);
void view_move_to_workspace(struct view *view, struct workspace *workspace);
bool view_titlebar_visible(struct view *view);
void view_set_ssd_mode(struct view *view, enum lab_ssd_mode mode);
void view_set_decorations(struct view *view, enum lab_ssd_mode mode, bool force_ssd);
void view_toggle_fullscreen(struct view *view);
void view_save_last_placement(struct view *view);
void view_adjust_for_layout_change(struct view *view);
void view_move_to_edge(struct view *view, enum lab_edge direction, bool snap_to_windows);
void view_grow_to_edge(struct view *view, enum lab_edge direction);
void view_shrink_to_edge(struct view *view, enum lab_edge direction);
void view_snap_to_edge(struct view *view, enum lab_edge direction,
	bool across_outputs, bool combine);
void view_snap_to_region(struct view *view, struct region *region);
void view_move_to_output(struct view *view, struct output *output);
void view_move_to_front(struct view *view);
void view_move_to_back(struct view *view);
bool view_is_modal_dialog(struct view *view);
struct view *view_get_modal_dialog(struct view *view);
bool view_on_output(struct view *view, struct output *output);
bool view_has_strut_partial(struct view *view);
void view_set_title(struct view *view, const char *title);
void view_set_app_id(struct view *view, const char *app_id);
void view_reload_ssd(struct view *view);
void view_set_shade(struct view *view, bool shaded);
struct view_size_hints view_get_size_hints(struct view *view);
void view_adjust_size(struct view *view, int *w, int *h);
void view_evacuate_region(struct view *view);
void view_on_output_destroy(struct view *view);
void view_update_visibility(struct view *view);
void view_init(struct view *view);
void view_destroy(struct view *view);
enum view_axis view_axis_parse(const char *direction);
enum lab_placement_policy view_placement_parse(const char *policy);
struct wlr_xdg_surface *xdg_surface_from_view(struct view *view);
bool view_matches_criteria(struct view *view, enum lab_view_criteria criteria);
#endif
#ifndef LABWC_WORKSPACES_H
#define LABWC_WORKSPACES_H
#include <stdbool.h>
#include <wayland-util.h>
#include <wayland-server-core.h>
struct seat;
struct server;
struct wlr_scene_tree;
struct workspace {
	struct wl_list link;
	char *name;
	struct wlr_scene_tree *tree;
	struct wlr_scene_tree *view_trees[3];
	struct wlr_ext_workspace_handle_v1 *ext_workspace;
};
void workspaces_init(void);
void workspaces_switch_to(struct workspace *target, bool update_focus);
void workspaces_destroy(void);
void workspaces_osd_hide(struct seat *seat);
struct workspace *workspaces_find(struct workspace *anchor, const char *name,
	bool wrap);
#endif
#ifndef LABWC_BUFFER_H
#define LABWC_BUFFER_H
#include <cairo.h>
struct lab_data_buffer {
	struct wlr_buffer base;
	bool surface_owns_data;
	cairo_surface_t *surface;
	void *data;
	uint32_t format;
	size_t stride;
	uint32_t logical_width;
	uint32_t logical_height;
};
struct lab_data_buffer *buffer_adopt_cairo_surface(cairo_surface_t *surface);
struct lab_data_buffer *buffer_create_cairo(uint32_t logical_width,
	uint32_t logical_height, float scale);
struct lab_data_buffer *buffer_create_from_data(void *pixel_data, uint32_t width,
	uint32_t height, uint32_t stride);
struct lab_data_buffer *buffer_resize(struct lab_data_buffer *src_buffer,
	int width, int height, double scale);
#endif
#ifndef LABWC_BOX_H
#define LABWC_BOX_H
bool box_intersects(struct wlr_box *box_a, struct wlr_box *box_b);
void box_union(struct wlr_box *box_dest, struct wlr_box *box_a,
	struct wlr_box *box_b);
void box_center(int width, int height, const struct wlr_box *ref,
	const struct wlr_box *bound, int *x, int *y);
struct wlr_box box_fit_within(int width, int height, struct wlr_box *bound);
struct wlr_fbox box_to_fbox(struct wlr_box *box);
#endif
#ifndef LABWC_IMG_H
#define LABWC_IMG_H
#include <cairo.h>
#include <stdbool.h>
#include <wayland-util.h>
struct lab_img {
	struct wl_array modifiers;
	struct lab_img_data *data;
};
struct lab_img *lab_img_load_from_bitmap(const char *bitmap, float *rgba);
typedef void (*lab_img_modifier_func_t)(cairo_t *cairo, int w, int h);
struct lab_img *lab_img_copy(struct lab_img *img);
void lab_img_add_modifier(struct lab_img *img, lab_img_modifier_func_t modifier);
struct lab_data_buffer *lab_img_render(struct lab_img *img,
	int width, int height, double scale);
void lab_img_destroy(struct lab_img *img);
bool lab_img_equal(struct lab_img *img_a, struct lab_img *img_b);
#endif
#ifndef LABWC_FD_UTIL_H
#define LABWC_FD_UTIL_H
void increase_nofile_limit(void);
void restore_nofile_limit(void);
#endif
#ifndef LABWC_DIR_H
#define LABWC_DIR_H
#include <wayland-server-core.h>
struct path {
	char *string;
	struct wl_list link;
};
void paths_config_create(struct wl_list *paths, const char *filename);
void paths_destroy(struct wl_list *paths);
#endif
#ifndef LABWC_LAB_SCENE_RECT_H
#define LABWC_LAB_SCENE_RECT_H
#include <wayland-server-core.h>
struct wlr_scene_tree;
struct lab_scene_rect_options {
	float **border_colors;
	int nr_borders;
	int border_width;
	float *bg_color;
	int width;
	int height;
};
struct lab_scene_rect {
	struct wlr_scene_tree *tree;
	int border_width;
	int nr_borders;
	struct border_scene *borders;
	struct wlr_scene_rect *fill;
	struct wl_listener node_destroy;
};
struct lab_scene_rect *lab_scene_rect_create(struct wlr_scene_tree *parent,
	struct lab_scene_rect_options *opts);
void lab_scene_rect_set_size(struct lab_scene_rect *rect, int width, int height);
#endif
#ifndef LABWC_SCENE_HELPERS_H
#define LABWC_SCENE_HELPERS_H
#include <stdbool.h>
struct wlr_buffer;
struct wlr_scene_node;
struct wlr_surface;
struct wlr_scene_output;
struct wlr_output_state;
struct wlr_surface *lab_wlr_surface_from_node(struct wlr_scene_node *node);
struct wlr_scene_tree *lab_wlr_scene_tree_create(struct wlr_scene_tree *parent);
struct wlr_scene_rect *lab_wlr_scene_rect_create(struct wlr_scene_tree *parent,
	int width, int height, const float color[static 4]);
struct wlr_scene_buffer *lab_wlr_scene_buffer_create(
	struct wlr_scene_tree *parent, struct wlr_buffer *buffer);
struct wlr_scene_node *lab_wlr_scene_get_prev_node(struct wlr_scene_node *node);
bool lab_wlr_scene_output_commit(struct wlr_scene_output *scene_output,
	struct wlr_output_state *state);
#endif
#ifndef LABWC_NODE_DESCRIPTOR_H
#define LABWC_NODE_DESCRIPTOR_H
#include <wayland-server-core.h>
struct wlr_scene_node;
struct node_descriptor {
	enum lab_node_type type;
	struct view *view;
	void *data;
	struct wl_listener destroy;
};
void node_descriptor_create(struct wlr_scene_node *scene_node,
	enum lab_node_type type, struct view *view, void *data);
struct view *node_view_from_node(struct wlr_scene_node *wlr_scene_node);
struct lab_layer_surface *node_layer_surface_from_node(
	struct wlr_scene_node *wlr_scene_node);
struct menuitem *node_menuitem_from_node(
	struct wlr_scene_node *wlr_scene_node);
struct ssd_button *node_try_ssd_button_from_node(
	struct wlr_scene_node *wlr_scene_node);
#endif
#ifndef LABWC_SCALED_FONT_BUFFER_H
#define LABWC_SCALED_FONT_BUFFER_H
struct wlr_scene_tree;
struct wlr_scene_buffer;
struct scaled_buffer;
struct scaled_font_buffer {
	struct wlr_scene_buffer *scene_buffer;
	int width;
	int height;
	char *text;
	bool use_markup;
	int max_width;
	float color[4];
	float bg_color[4];
	struct font font;
	struct scaled_buffer *scaled_buffer;
};
struct scaled_font_buffer *scaled_font_buffer_create(struct wlr_scene_tree *parent);
void scaled_font_buffer_update(struct scaled_font_buffer *self,
	const char *text, int max_width, struct font *font, const float *color,
	const float *bg_color);
void scaled_font_buffer_update_markup(struct scaled_font_buffer *self,
	const char *text, int max_width, struct font *font, const float *color,
	const float *bg_color, bool use_markup);
#endif
#ifndef LABWC_PARSE_DOUBLE_H
#define LABWC_PARSE_DOUBLE_H
#include <assert.h>
#include <stdbool.h>
bool set_double(const char *str, double *val);
static inline bool
set_float(const char *str, float *val)
{
	assert(val);
	double d;
	if (set_double(str, &d)) {
		*val = (float)d;
		return true;
	}
	return false;
}
#endif
#ifndef LABWC_GRAPHIC_HELPERS_H
#define LABWC_GRAPHIC_HELPERS_H
#include <stdbool.h>
#include <stdint.h>
#include <cairo.h>
struct wlr_fbox;
void set_cairo_color(cairo_t *cairo, const float *color);
cairo_pattern_t *color_to_pattern(const float *color);
bool is_pattern_opaque(cairo_pattern_t *pattern);
#define zfree_pattern(ptr) do { \
	if (ptr) { \
		cairo_pattern_destroy(ptr); \
		(ptr) = NULL; \
	} \
} while (0)
void draw_cairo_border(cairo_t *cairo, struct wlr_fbox fbox, double line_width);
bool lookup_named_color(const char *name, uint32_t *argb);
#endif
#ifndef LABWC_SCALED_BUFFER_H
#define LABWC_SCALED_BUFFER_H
#include <wayland-server-core.h>
#define LAB_SCALED_BUFFER_MAX_CACHE 2
struct wlr_buffer;
struct wlr_scene_tree;
struct lab_data_buffer;
struct scaled_buffer;
struct scaled_buffer_impl {
	struct lab_data_buffer *(*create_buffer)
		(struct scaled_buffer *scaled_buffer, double scale);
	void (*destroy)(struct scaled_buffer *scaled_buffer);
	bool (*equal)(struct scaled_buffer *scaled_buffer_a,
		struct scaled_buffer *scaled_buffer_b);
};
struct scaled_buffer {
	struct wlr_scene_buffer *scene_buffer;
	int width;
	int height;
	void *data;
	bool drop_buffer;
	double active_scale;
	struct wl_list cache;
	struct wl_listener destroy;
	struct wl_listener outputs_update;
	const struct scaled_buffer_impl *impl;
	struct wl_list link;
};
struct scaled_buffer *scaled_buffer_create(
	struct wlr_scene_tree *parent,
	const struct scaled_buffer_impl *implementation,
	bool drop_buffer);
void scaled_buffer_request_update(struct scaled_buffer *self,
	int width, int height);
void scaled_buffer_invalidate_sharing(void);
struct scaled_buffer_cache_entry {
	struct wl_list link;
	struct wlr_buffer *buffer;
	double scale;
};
#endif
#ifndef LABWC_IMG_XBM_H
#define LABWC_IMG_XBM_H
struct lab_data_buffer;
struct lab_data_buffer *img_xbm_load_from_bitmap(const char *bitmap, float *rgba);
#endif
#ifndef LABWC_WINDOW_RULES_H
#define LABWC_WINDOW_RULES_H
#include <stdbool.h>
#include <wayland-util.h>
enum window_rule_event {
	LAB_WINDOW_RULE_EVENT_ON_FIRST_MAP = 0,
};
enum property {
	LAB_PROP_UNSPECIFIED = 0,
	LAB_PROP_UNSET,
	LAB_PROP_FALSE,
	LAB_PROP_TRUE,
};
struct window_rule {
	char *identifier;
	char *title;
	enum lab_window_type window_type;
	char *sandbox_engine;
	char *sandbox_app_id;
	bool match_once;
	enum window_rule_event event;
	struct wl_list actions;
	enum property server_decoration;
	enum property skip_taskbar;
	enum property skip_window_switcher;
	enum property ignore_focus_request;
	enum property ignore_configure_request;
	enum property fixed_position;
	enum property allow_always_on_top;
	struct wl_list link;
};
struct view;
void window_rules_apply(struct view *view, enum window_rule_event event);
enum property window_rules_get_property(struct view *view, const char *property);
#endif
#ifndef LABWC_SCALED_IMG_BUFFER_H
#define LABWC_SCALED_IMG_BUFFER_H
struct wlr_scene_tree;
struct wlr_scene_node;
struct wlr_scene_buffer;
struct lab_img;
struct scaled_img_buffer {
	struct scaled_buffer *scaled_buffer;
	struct wlr_scene_buffer *scene_buffer;
	struct lab_img *img;
	int width;
	int height;
};
struct scaled_img_buffer *scaled_img_buffer_create(struct wlr_scene_tree *parent,
	struct lab_img *img, int width, int height);
#endif
#ifndef LABWC_MATCH_H
#define LABWC_MATCH_H
#include <stdbool.h>
bool match_glob(const char *pattern, const char *string);
#endif
static const char color_names[] =
	"alice blue\0"
	"AliceBlue\0"
	"antique white\0"
	"AntiqueWhite\0"
	"AntiqueWhite1\0"
	"AntiqueWhite2\0"
	"AntiqueWhite3\0"
	"AntiqueWhite4\0"
	"aqua\0"
	"aquamarine\0"
	"aquamarine1\0"
	"aquamarine2\0"
	"aquamarine3\0"
	"aquamarine4\0"
	"azure\0"
	"azure1\0"
	"azure2\0"
	"azure3\0"
	"azure4\0"
	"beige\0"
	"bisque\0"
	"bisque1\0"
	"bisque2\0"
	"bisque3\0"
	"bisque4\0"
	"black\0"
	"blanched almond\0"
	"BlanchedAlmond\0"
	"blue\0"
	"blue violet\0"
	"blue1\0"
	"blue2\0"
	"blue3\0"
	"blue4\0"
	"BlueViolet\0"
	"brown\0"
	"brown1\0"
	"brown2\0"
	"brown3\0"
	"brown4\0"
	"burlywood\0"
	"burlywood1\0"
	"burlywood2\0"
	"burlywood3\0"
	"burlywood4\0"
	"cadet blue\0"
	"CadetBlue\0"
	"CadetBlue1\0"
	"CadetBlue2\0"
	"CadetBlue3\0"
	"CadetBlue4\0"
	"chartreuse\0"
	"chartreuse1\0"
	"chartreuse2\0"
	"chartreuse3\0"
	"chartreuse4\0"
	"chocolate\0"
	"chocolate1\0"
	"chocolate2\0"
	"chocolate3\0"
	"chocolate4\0"
	"coral\0"
	"coral1\0"
	"coral2\0"
	"coral3\0"
	"coral4\0"
	"cornflower blue\0"
	"CornflowerBlue\0"
	"cornsilk\0"
	"cornsilk1\0"
	"cornsilk2\0"
	"cornsilk3\0"
	"cornsilk4\0"
	"crimson\0"
	"cyan\0"
	"cyan1\0"
	"cyan2\0"
	"cyan3\0"
	"cyan4\0"
	"dark blue\0"
	"dark cyan\0"
	"dark goldenrod\0"
	"dark gray\0"
	"dark green\0"
	"dark grey\0"
	"dark khaki\0"
	"dark magenta\0"
	"dark olive green\0"
	"dark orange\0"
	"dark orchid\0"
	"dark red\0"
	"dark salmon\0"
	"dark sea green\0"
	"dark slate blue\0"
	"dark slate gray\0"
	"dark slate grey\0"
	"dark turquoise\0"
	"dark violet\0"
	"DarkBlue\0"
	"DarkCyan\0"
	"DarkGoldenrod\0"
	"DarkGoldenrod1\0"
	"DarkGoldenrod2\0"
	"DarkGoldenrod3\0"
	"DarkGoldenrod4\0"
	"DarkGray\0"
	"DarkGreen\0"
	"DarkGrey\0"
	"DarkKhaki\0"
	"DarkMagenta\0"
	"DarkOliveGreen\0"
	"DarkOliveGreen1\0"
	"DarkOliveGreen2\0"
	"DarkOliveGreen3\0"
	"DarkOliveGreen4\0"
	"DarkOrange\0"
	"DarkOrange1\0"
	"DarkOrange2\0"
	"DarkOrange3\0"
	"DarkOrange4\0"
	"DarkOrchid\0"
	"DarkOrchid1\0"
	"DarkOrchid2\0"
	"DarkOrchid3\0"
	"DarkOrchid4\0"
	"DarkRed\0"
	"DarkSalmon\0"
	"DarkSeaGreen\0"
	"DarkSeaGreen1\0"
	"DarkSeaGreen2\0"
	"DarkSeaGreen3\0"
	"DarkSeaGreen4\0"
	"DarkSlateBlue\0"
	"DarkSlateGray\0"
	"DarkSlateGray1\0"
	"DarkSlateGray2\0"
	"DarkSlateGray3\0"
	"DarkSlateGray4\0"
	"DarkSlateGrey\0"
	"DarkTurquoise\0"
	"DarkViolet\0"
	"deep pink\0"
	"deep sky blue\0"
	"DeepPink\0"
	"DeepPink1\0"
	"DeepPink2\0"
	"DeepPink3\0"
	"DeepPink4\0"
	"DeepSkyBlue\0"
	"DeepSkyBlue1\0"
	"DeepSkyBlue2\0"
	"DeepSkyBlue3\0"
	"DeepSkyBlue4\0"
	"dim gray\0"
	"dim grey\0"
	"DimGray\0"
	"DimGrey\0"
	"dodger blue\0"
	"DodgerBlue\0"
	"DodgerBlue1\0"
	"DodgerBlue2\0"
	"DodgerBlue3\0"
	"DodgerBlue4\0"
	"firebrick\0"
	"firebrick1\0"
	"firebrick2\0"
	"firebrick3\0"
	"firebrick4\0"
	"floral white\0"
	"FloralWhite\0"
	"forest green\0"
	"ForestGreen\0"
	"fuchsia\0"
	"gainsboro\0"
	"ghost white\0"
	"GhostWhite\0"
	"gold\0"
	"gold1\0"
	"gold2\0"
	"gold3\0"
	"gold4\0"
	"goldenrod\0"
	"goldenrod1\0"
	"goldenrod2\0"
	"goldenrod3\0"
	"goldenrod4\0"
	"gray\0"
	"gray0\0"
	"gray1\0"
	"gray10\0"
	"gray100\0"
	"gray11\0"
	"gray12\0"
	"gray13\0"
	"gray14\0"
	"gray15\0"
	"gray16\0"
	"gray17\0"
	"gray18\0"
	"gray19\0"
	"gray2\0"
	"gray20\0"
	"gray21\0"
	"gray22\0"
	"gray23\0"
	"gray24\0"
	"gray25\0"
	"gray26\0"
	"gray27\0"
	"gray28\0"
	"gray29\0"
	"gray3\0"
	"gray30\0"
	"gray31\0"
	"gray32\0"
	"gray33\0"
	"gray34\0"
	"gray35\0"
	"gray36\0"
	"gray37\0"
	"gray38\0"
	"gray39\0"
	"gray4\0"
	"gray40\0"
	"gray41\0"
	"gray42\0"
	"gray43\0"
	"gray44\0"
	"gray45\0"
	"gray46\0"
	"gray47\0"
	"gray48\0"
	"gray49\0"
	"gray5\0"
	"gray50\0"
	"gray51\0"
	"gray52\0"
	"gray53\0"
	"gray54\0"
	"gray55\0"
	"gray56\0"
	"gray57\0"
	"gray58\0"
	"gray59\0"
	"gray6\0"
	"gray60\0"
	"gray61\0"
	"gray62\0"
	"gray63\0"
	"gray64\0"
	"gray65\0"
	"gray66\0"
	"gray67\0"
	"gray68\0"
	"gray69\0"
	"gray7\0"
	"gray70\0"
	"gray71\0"
	"gray72\0"
	"gray73\0"
	"gray74\0"
	"gray75\0"
	"gray76\0"
	"gray77\0"
	"gray78\0"
	"gray79\0"
	"gray8\0"
	"gray80\0"
	"gray81\0"
	"gray82\0"
	"gray83\0"
	"gray84\0"
	"gray85\0"
	"gray86\0"
	"gray87\0"
	"gray88\0"
	"gray89\0"
	"gray9\0"
	"gray90\0"
	"gray91\0"
	"gray92\0"
	"gray93\0"
	"gray94\0"
	"gray95\0"
	"gray96\0"
	"gray97\0"
	"gray98\0"
	"gray99\0"
	"green\0"
	"green yellow\0"
	"green1\0"
	"green2\0"
	"green3\0"
	"green4\0"
	"GreenYellow\0"
	"grey\0"
	"grey0\0"
	"grey1\0"
	"grey10\0"
	"grey100\0"
	"grey11\0"
	"grey12\0"
	"grey13\0"
	"grey14\0"
	"grey15\0"
	"grey16\0"
	"grey17\0"
	"grey18\0"
	"grey19\0"
	"grey2\0"
	"grey20\0"
	"grey21\0"
	"grey22\0"
	"grey23\0"
	"grey24\0"
	"grey25\0"
	"grey26\0"
	"grey27\0"
	"grey28\0"
	"grey29\0"
	"grey3\0"
	"grey30\0"
	"grey31\0"
	"grey32\0"
	"grey33\0"
	"grey34\0"
	"grey35\0"
	"grey36\0"
	"grey37\0"
	"grey38\0"
	"grey39\0"
	"grey4\0"
	"grey40\0"
	"grey41\0"
	"grey42\0"
	"grey43\0"
	"grey44\0"
	"grey45\0"
	"grey46\0"
	"grey47\0"
	"grey48\0"
	"grey49\0"
	"grey5\0"
	"grey50\0"
	"grey51\0"
	"grey52\0"
	"grey53\0"
	"grey54\0"
	"grey55\0"
	"grey56\0"
	"grey57\0"
	"grey58\0"
	"grey59\0"
	"grey6\0"
	"grey60\0"
	"grey61\0"
	"grey62\0"
	"grey63\0"
	"grey64\0"
	"grey65\0"
	"grey66\0"
	"grey67\0"
	"grey68\0"
	"grey69\0"
	"grey7\0"
	"grey70\0"
	"grey71\0"
	"grey72\0"
	"grey73\0"
	"grey74\0"
	"grey75\0"
	"grey76\0"
	"grey77\0"
	"grey78\0"
	"grey79\0"
	"grey8\0"
	"grey80\0"
	"grey81\0"
	"grey82\0"
	"grey83\0"
	"grey84\0"
	"grey85\0"
	"grey86\0"
	"grey87\0"
	"grey88\0"
	"grey89\0"
	"grey9\0"
	"grey90\0"
	"grey91\0"
	"grey92\0"
	"grey93\0"
	"grey94\0"
	"grey95\0"
	"grey96\0"
	"grey97\0"
	"grey98\0"
	"grey99\0"
	"honeydew\0"
	"honeydew1\0"
	"honeydew2\0"
	"honeydew3\0"
	"honeydew4\0"
	"hot pink\0"
	"HotPink\0"
	"HotPink1\0"
	"HotPink2\0"
	"HotPink3\0"
	"HotPink4\0"
	"indian red\0"
	"IndianRed\0"
	"IndianRed1\0"
	"IndianRed2\0"
	"IndianRed3\0"
	"IndianRed4\0"
	"indigo\0"
	"ivory\0"
	"ivory1\0"
	"ivory2\0"
	"ivory3\0"
	"ivory4\0"
	"khaki\0"
	"khaki1\0"
	"khaki2\0"
	"khaki3\0"
	"khaki4\0"
	"lavender\0"
	"lavender blush\0"
	"LavenderBlush\0"
	"LavenderBlush1\0"
	"LavenderBlush2\0"
	"LavenderBlush3\0"
	"LavenderBlush4\0"
	"lawn green\0"
	"LawnGreen\0"
	"lemon chiffon\0"
	"LemonChiffon\0"
	"LemonChiffon1\0"
	"LemonChiffon2\0"
	"LemonChiffon3\0"
	"LemonChiffon4\0"
	"light blue\0"
	"light coral\0"
	"light cyan\0"
	"light goldenrod\0"
	"light goldenrod yellow\0"
	"light gray\0"
	"light green\0"
	"light grey\0"
	"light pink\0"
	"light salmon\0"
	"light sea green\0"
	"light sky blue\0"
	"light slate blue\0"
	"light slate gray\0"
	"light slate grey\0"
	"light steel blue\0"
	"light yellow\0"
	"LightBlue\0"
	"LightBlue1\0"
	"LightBlue2\0"
	"LightBlue3\0"
	"LightBlue4\0"
	"LightCoral\0"
	"LightCyan\0"
	"LightCyan1\0"
	"LightCyan2\0"
	"LightCyan3\0"
	"LightCyan4\0"
	"LightGoldenrod\0"
	"LightGoldenrod1\0"
	"LightGoldenrod2\0"
	"LightGoldenrod3\0"
	"LightGoldenrod4\0"
	"LightGoldenrodYellow\0"
	"LightGray\0"
	"LightGreen\0"
	"LightGrey\0"
	"LightPink\0"
	"LightPink1\0"
	"LightPink2\0"
	"LightPink3\0"
	"LightPink4\0"
	"LightSalmon\0"
	"LightSalmon1\0"
	"LightSalmon2\0"
	"LightSalmon3\0"
	"LightSalmon4\0"
	"LightSeaGreen\0"
	"LightSkyBlue\0"
	"LightSkyBlue1\0"
	"LightSkyBlue2\0"
	"LightSkyBlue3\0"
	"LightSkyBlue4\0"
	"LightSlateBlue\0"
	"LightSlateGray\0"
	"LightSlateGrey\0"
	"LightSteelBlue\0"
	"LightSteelBlue1\0"
	"LightSteelBlue2\0"
	"LightSteelBlue3\0"
	"LightSteelBlue4\0"
	"LightYellow\0"
	"LightYellow1\0"
	"LightYellow2\0"
	"LightYellow3\0"
	"LightYellow4\0"
	"lime\0"
	"lime green\0"
	"LimeGreen\0"
	"linen\0"
	"magenta\0"
	"magenta1\0"
	"magenta2\0"
	"magenta3\0"
	"magenta4\0"
	"maroon\0"
	"maroon1\0"
	"maroon2\0"
	"maroon3\0"
	"maroon4\0"
	"medium aquamarine\0"
	"medium blue\0"
	"medium orchid\0"
	"medium purple\0"
	"medium sea green\0"
	"medium slate blue\0"
	"medium spring green\0"
	"medium turquoise\0"
	"medium violet red\0"
	"MediumAquamarine\0"
	"MediumBlue\0"
	"MediumOrchid\0"
	"MediumOrchid1\0"
	"MediumOrchid2\0"
	"MediumOrchid3\0"
	"MediumOrchid4\0"
	"MediumPurple\0"
	"MediumPurple1\0"
	"MediumPurple2\0"
	"MediumPurple3\0"
	"MediumPurple4\0"
	"MediumSeaGreen\0"
	"MediumSlateBlue\0"
	"MediumSpringGreen\0"
	"MediumTurquoise\0"
	"MediumVioletRed\0"
	"midnight blue\0"
	"MidnightBlue\0"
	"mint cream\0"
	"MintCream\0"
	"misty rose\0"
	"MistyRose\0"
	"MistyRose1\0"
	"MistyRose2\0"
	"MistyRose3\0"
	"MistyRose4\0"
	"moccasin\0"
	"navajo white\0"
	"NavajoWhite\0"
	"NavajoWhite1\0"
	"NavajoWhite2\0"
	"NavajoWhite3\0"
	"NavajoWhite4\0"
	"navy\0"
	"navy blue\0"
	"NavyBlue\0"
	"old lace\0"
	"OldLace\0"
	"olive\0"
	"olive drab\0"
	"OliveDrab\0"
	"OliveDrab1\0"
	"OliveDrab2\0"
	"OliveDrab3\0"
	"OliveDrab4\0"
	"orange\0"
	"orange red\0"
	"orange1\0"
	"orange2\0"
	"orange3\0"
	"orange4\0"
	"OrangeRed\0"
	"OrangeRed1\0"
	"OrangeRed2\0"
	"OrangeRed3\0"
	"OrangeRed4\0"
	"orchid\0"
	"orchid1\0"
	"orchid2\0"
	"orchid3\0"
	"orchid4\0"
	"pale goldenrod\0"
	"pale green\0"
	"pale turquoise\0"
	"pale violet red\0"
	"PaleGoldenrod\0"
	"PaleGreen\0"
	"PaleGreen1\0"
	"PaleGreen2\0"
	"PaleGreen3\0"
	"PaleGreen4\0"
	"PaleTurquoise\0"
	"PaleTurquoise1\0"
	"PaleTurquoise2\0"
	"PaleTurquoise3\0"
	"PaleTurquoise4\0"
	"PaleVioletRed\0"
	"PaleVioletRed1\0"
	"PaleVioletRed2\0"
	"PaleVioletRed3\0"
	"PaleVioletRed4\0"
	"papaya whip\0"
	"PapayaWhip\0"
	"peach puff\0"
	"PeachPuff\0"
	"PeachPuff1\0"
	"PeachPuff2\0"
	"PeachPuff3\0"
	"PeachPuff4\0"
	"peru\0"
	"pink\0"
	"pink1\0"
	"pink2\0"
	"pink3\0"
	"pink4\0"
	"plum\0"
	"plum1\0"
	"plum2\0"
	"plum3\0"
	"plum4\0"
	"powder blue\0"
	"PowderBlue\0"
	"purple\0"
	"purple1\0"
	"purple2\0"
	"purple3\0"
	"purple4\0"
	"rebecca purple\0"
	"RebeccaPurple\0"
	"red\0"
	"red1\0"
	"red2\0"
	"red3\0"
	"red4\0"
	"rosy brown\0"
	"RosyBrown\0"
	"RosyBrown1\0"
	"RosyBrown2\0"
	"RosyBrown3\0"
	"RosyBrown4\0"
	"royal blue\0"
	"RoyalBlue\0"
	"RoyalBlue1\0"
	"RoyalBlue2\0"
	"RoyalBlue3\0"
	"RoyalBlue4\0"
	"saddle brown\0"
	"SaddleBrown\0"
	"salmon\0"
	"salmon1\0"
	"salmon2\0"
	"salmon3\0"
	"salmon4\0"
	"sandy brown\0"
	"SandyBrown\0"
	"sea green\0"
	"SeaGreen\0"
	"SeaGreen1\0"
	"SeaGreen2\0"
	"SeaGreen3\0"
	"SeaGreen4\0"
	"seashell\0"
	"seashell1\0"
	"seashell2\0"
	"seashell3\0"
	"seashell4\0"
	"sienna\0"
	"sienna1\0"
	"sienna2\0"
	"sienna3\0"
	"sienna4\0"
	"silver\0"
	"sky blue\0"
	"SkyBlue\0"
	"SkyBlue1\0"
	"SkyBlue2\0"
	"SkyBlue3\0"
	"SkyBlue4\0"
	"slate blue\0"
	"slate gray\0"
	"slate grey\0"
	"SlateBlue\0"
	"SlateBlue1\0"
	"SlateBlue2\0"
	"SlateBlue3\0"
	"SlateBlue4\0"
	"SlateGray\0"
	"SlateGray1\0"
	"SlateGray2\0"
	"SlateGray3\0"
	"SlateGray4\0"
	"SlateGrey\0"
	"snow\0"
	"snow1\0"
	"snow2\0"
	"snow3\0"
	"snow4\0"
	"spring green\0"
	"SpringGreen\0"
	"SpringGreen1\0"
	"SpringGreen2\0"
	"SpringGreen3\0"
	"SpringGreen4\0"
	"steel blue\0"
	"SteelBlue\0"
	"SteelBlue1\0"
	"SteelBlue2\0"
	"SteelBlue3\0"
	"SteelBlue4\0"
	"tan\0"
	"tan1\0"
	"tan2\0"
	"tan3\0"
	"tan4\0"
	"teal\0"
	"thistle\0"
	"thistle1\0"
	"thistle2\0"
	"thistle3\0"
	"thistle4\0"
	"tomato\0"
	"tomato1\0"
	"tomato2\0"
	"tomato3\0"
	"tomato4\0"
	"turquoise\0"
	"turquoise1\0"
	"turquoise2\0"
	"turquoise3\0"
	"turquoise4\0"
	"violet\0"
	"violet red\0"
	"VioletRed\0"
	"VioletRed1\0"
	"VioletRed2\0"
	"VioletRed3\0"
	"VioletRed4\0"
	"web gray\0"
	"web green\0"
	"web grey\0"
	"web maroon\0"
	"web purple\0"
	"WebGray\0"
	"WebGreen\0"
	"WebGrey\0"
	"WebMaroon\0"
	"WebPurple\0"
	"wheat\0"
	"wheat1\0"
	"wheat2\0"
	"wheat3\0"
	"wheat4\0"
	"white\0"
	"white smoke\0"
	"WhiteSmoke\0"
	"x11 gray\0"
	"x11 green\0"
	"x11 grey\0"
	"x11 maroon\0"
	"x11 purple\0"
	"X11Gray\0"
	"X11Green\0"
	"X11Grey\0"
	"X11Maroon\0"
	"X11Purple\0"
	"yellow\0"
	"yellow green\0"
	"yellow1\0"
	"yellow2\0"
	"yellow3\0"
	"yellow4\0"
	"YellowGreen\0";
struct xcolor_entry {
	uint16_t name_offset;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};
static const struct xcolor_entry xcolors[] = {
	{ 0, 240, 248, 255 },
	{ 11, 240, 248, 255 },
	{ 21, 250, 235, 215 },
	{ 35, 250, 235, 215 },
	{ 48, 255, 239, 219 },
	{ 62, 238, 223, 204 },
	{ 76, 205, 192, 176 },
	{ 90, 139, 131, 120 },
	{ 104, 0, 255, 255 },
	{ 109, 127, 255, 212 },
	{ 120, 127, 255, 212 },
	{ 132, 118, 238, 198 },
	{ 144, 102, 205, 170 },
	{ 156, 69, 139, 116 },
	{ 168, 240, 255, 255 },
	{ 174, 240, 255, 255 },
	{ 181, 224, 238, 238 },
	{ 188, 193, 205, 205 },
	{ 195, 131, 139, 139 },
	{ 202, 245, 245, 220 },
	{ 208, 255, 228, 196 },
	{ 215, 255, 228, 196 },
	{ 223, 238, 213, 183 },
	{ 231, 205, 183, 158 },
	{ 239, 139, 125, 107 },
	{ 247, 0, 0, 0 },
	{ 253, 255, 235, 205 },
	{ 269, 255, 235, 205 },
	{ 284, 0, 0, 255 },
	{ 289, 138, 43, 226 },
	{ 301, 0, 0, 255 },
	{ 307, 0, 0, 238 },
	{ 313, 0, 0, 205 },
	{ 319, 0, 0, 139 },
	{ 325, 138, 43, 226 },
	{ 336, 165, 42, 42 },
	{ 342, 255, 64, 64 },
	{ 349, 238, 59, 59 },
	{ 356, 205, 51, 51 },
	{ 363, 139, 35, 35 },
	{ 370, 222, 184, 135 },
	{ 380, 255, 211, 155 },
	{ 391, 238, 197, 145 },
	{ 402, 205, 170, 125 },
	{ 413, 139, 115, 85 },
	{ 424, 95, 158, 160 },
	{ 435, 95, 158, 160 },
	{ 445, 152, 245, 255 },
	{ 456, 142, 229, 238 },
	{ 467, 122, 197, 205 },
	{ 478, 83, 134, 139 },
	{ 489, 127, 255, 0 },
	{ 500, 127, 255, 0 },
	{ 512, 118, 238, 0 },
	{ 524, 102, 205, 0 },
	{ 536, 69, 139, 0 },
	{ 548, 210, 105, 30 },
	{ 558, 255, 127, 36 },
	{ 569, 238, 118, 33 },
	{ 580, 205, 102, 29 },
	{ 591, 139, 69, 19 },
	{ 602, 255, 127, 80 },
	{ 608, 255, 114, 86 },
	{ 615, 238, 106, 80 },
	{ 622, 205, 91, 69 },
	{ 629, 139, 62, 47 },
	{ 636, 100, 149, 237 },
	{ 652, 100, 149, 237 },
	{ 667, 255, 248, 220 },
	{ 676, 255, 248, 220 },
	{ 686, 238, 232, 205 },
	{ 696, 205, 200, 177 },
	{ 706, 139, 136, 120 },
	{ 716, 220, 20, 60 },
	{ 724, 0, 255, 255 },
	{ 729, 0, 255, 255 },
	{ 735, 0, 238, 238 },
	{ 741, 0, 205, 205 },
	{ 747, 0, 139, 139 },
	{ 753, 0, 0, 139 },
	{ 763, 0, 139, 139 },
	{ 773, 184, 134, 11 },
	{ 788, 169, 169, 169 },
	{ 798, 0, 100, 0 },
	{ 809, 169, 169, 169 },
	{ 819, 189, 183, 107 },
	{ 830, 139, 0, 139 },
	{ 843, 85, 107, 47 },
	{ 860, 255, 140, 0 },
	{ 872, 153, 50, 204 },
	{ 884, 139, 0, 0 },
	{ 893, 233, 150, 122 },
	{ 905, 143, 188, 143 },
	{ 920, 72, 61, 139 },
	{ 936, 47, 79, 79 },
	{ 952, 47, 79, 79 },
	{ 968, 0, 206, 209 },
	{ 983, 148, 0, 211 },
	{ 995, 0, 0, 139 },
	{ 1004, 0, 139, 139 },
	{ 1013, 184, 134, 11 },
	{ 1027, 255, 185, 15 },
	{ 1042, 238, 173, 14 },
	{ 1057, 205, 149, 12 },
	{ 1072, 139, 101, 8 },
	{ 1087, 169, 169, 169 },
	{ 1096, 0, 100, 0 },
	{ 1106, 169, 169, 169 },
	{ 1115, 189, 183, 107 },
	{ 1125, 139, 0, 139 },
	{ 1137, 85, 107, 47 },
	{ 1152, 202, 255, 112 },
	{ 1168, 188, 238, 104 },
	{ 1184, 162, 205, 90 },
	{ 1200, 110, 139, 61 },
	{ 1216, 255, 140, 0 },
	{ 1227, 255, 127, 0 },
	{ 1239, 238, 118, 0 },
	{ 1251, 205, 102, 0 },
	{ 1263, 139, 69, 0 },
	{ 1275, 153, 50, 204 },
	{ 1286, 191, 62, 255 },
	{ 1298, 178, 58, 238 },
	{ 1310, 154, 50, 205 },
	{ 1322, 104, 34, 139 },
	{ 1334, 139, 0, 0 },
	{ 1342, 233, 150, 122 },
	{ 1353, 143, 188, 143 },
	{ 1366, 193, 255, 193 },
	{ 1380, 180, 238, 180 },
	{ 1394, 155, 205, 155 },
	{ 1408, 105, 139, 105 },
	{ 1422, 72, 61, 139 },
	{ 1436, 47, 79, 79 },
	{ 1450, 151, 255, 255 },
	{ 1465, 141, 238, 238 },
	{ 1480, 121, 205, 205 },
	{ 1495, 82, 139, 139 },
	{ 1510, 47, 79, 79 },
	{ 1524, 0, 206, 209 },
	{ 1538, 148, 0, 211 },
	{ 1549, 255, 20, 147 },
	{ 1559, 0, 191, 255 },
	{ 1573, 255, 20, 147 },
	{ 1582, 255, 20, 147 },
	{ 1592, 238, 18, 137 },
	{ 1602, 205, 16, 118 },
	{ 1612, 139, 10, 80 },
	{ 1622, 0, 191, 255 },
	{ 1634, 0, 191, 255 },
	{ 1647, 0, 178, 238 },
	{ 1660, 0, 154, 205 },
	{ 1673, 0, 104, 139 },
	{ 1686, 105, 105, 105 },
	{ 1695, 105, 105, 105 },
	{ 1704, 105, 105, 105 },
	{ 1712, 105, 105, 105 },
	{ 1720, 30, 144, 255 },
	{ 1732, 30, 144, 255 },
	{ 1743, 30, 144, 255 },
	{ 1755, 28, 134, 238 },
	{ 1767, 24, 116, 205 },
	{ 1779, 16, 78, 139 },
	{ 1791, 178, 34, 34 },
	{ 1801, 255, 48, 48 },
	{ 1812, 238, 44, 44 },
	{ 1823, 205, 38, 38 },
	{ 1834, 139, 26, 26 },
	{ 1845, 255, 250, 240 },
	{ 1858, 255, 250, 240 },
	{ 1870, 34, 139, 34 },
	{ 1883, 34, 139, 34 },
	{ 1895, 255, 0, 255 },
	{ 1903, 220, 220, 220 },
	{ 1913, 248, 248, 255 },
	{ 1925, 248, 248, 255 },
	{ 1936, 255, 215, 0 },
	{ 1941, 255, 215, 0 },
	{ 1947, 238, 201, 0 },
	{ 1953, 205, 173, 0 },
	{ 1959, 139, 117, 0 },
	{ 1965, 218, 165, 32 },
	{ 1975, 255, 193, 37 },
	{ 1986, 238, 180, 34 },
	{ 1997, 205, 155, 29 },
	{ 2008, 139, 105, 20 },
	{ 2019, 190, 190, 190 },
	{ 2024, 0, 0, 0 },
	{ 2030, 3, 3, 3 },
	{ 2036, 26, 26, 26 },
	{ 2043, 255, 255, 255 },
	{ 2051, 28, 28, 28 },
	{ 2058, 31, 31, 31 },
	{ 2065, 33, 33, 33 },
	{ 2072, 36, 36, 36 },
	{ 2079, 38, 38, 38 },
	{ 2086, 41, 41, 41 },
	{ 2093, 43, 43, 43 },
	{ 2100, 46, 46, 46 },
	{ 2107, 48, 48, 48 },
	{ 2114, 5, 5, 5 },
	{ 2120, 51, 51, 51 },
	{ 2127, 54, 54, 54 },
	{ 2134, 56, 56, 56 },
	{ 2141, 59, 59, 59 },
	{ 2148, 61, 61, 61 },
	{ 2155, 64, 64, 64 },
	{ 2162, 66, 66, 66 },
	{ 2169, 69, 69, 69 },
	{ 2176, 71, 71, 71 },
	{ 2183, 74, 74, 74 },
	{ 2190, 8, 8, 8 },
	{ 2196, 77, 77, 77 },
	{ 2203, 79, 79, 79 },
	{ 2210, 82, 82, 82 },
	{ 2217, 84, 84, 84 },
	{ 2224, 87, 87, 87 },
	{ 2231, 89, 89, 89 },
	{ 2238, 92, 92, 92 },
	{ 2245, 94, 94, 94 },
	{ 2252, 97, 97, 97 },
	{ 2259, 99, 99, 99 },
	{ 2266, 10, 10, 10 },
	{ 2272, 102, 102, 102 },
	{ 2279, 105, 105, 105 },
	{ 2286, 107, 107, 107 },
	{ 2293, 110, 110, 110 },
	{ 2300, 112, 112, 112 },
	{ 2307, 115, 115, 115 },
	{ 2314, 117, 117, 117 },
	{ 2321, 120, 120, 120 },
	{ 2328, 122, 122, 122 },
	{ 2335, 125, 125, 125 },
	{ 2342, 13, 13, 13 },
	{ 2348, 127, 127, 127 },
	{ 2355, 130, 130, 130 },
	{ 2362, 133, 133, 133 },
	{ 2369, 135, 135, 135 },
	{ 2376, 138, 138, 138 },
	{ 2383, 140, 140, 140 },
	{ 2390, 143, 143, 143 },
	{ 2397, 145, 145, 145 },
	{ 2404, 148, 148, 148 },
	{ 2411, 150, 150, 150 },
	{ 2418, 15, 15, 15 },
	{ 2424, 153, 153, 153 },
	{ 2431, 156, 156, 156 },
	{ 2438, 158, 158, 158 },
	{ 2445, 161, 161, 161 },
	{ 2452, 163, 163, 163 },
	{ 2459, 166, 166, 166 },
	{ 2466, 168, 168, 168 },
	{ 2473, 171, 171, 171 },
	{ 2480, 173, 173, 173 },
	{ 2487, 176, 176, 176 },
	{ 2494, 18, 18, 18 },
	{ 2500, 179, 179, 179 },
	{ 2507, 181, 181, 181 },
	{ 2514, 184, 184, 184 },
	{ 2521, 186, 186, 186 },
	{ 2528, 189, 189, 189 },
	{ 2535, 191, 191, 191 },
	{ 2542, 194, 194, 194 },
	{ 2549, 196, 196, 196 },
	{ 2556, 199, 199, 199 },
	{ 2563, 201, 201, 201 },
	{ 2570, 20, 20, 20 },
	{ 2576, 204, 204, 204 },
	{ 2583, 207, 207, 207 },
	{ 2590, 209, 209, 209 },
	{ 2597, 212, 212, 212 },
	{ 2604, 214, 214, 214 },
	{ 2611, 217, 217, 217 },
	{ 2618, 219, 219, 219 },
	{ 2625, 222, 222, 222 },
	{ 2632, 224, 224, 224 },
	{ 2639, 227, 227, 227 },
	{ 2646, 23, 23, 23 },
	{ 2652, 229, 229, 229 },
	{ 2659, 232, 232, 232 },
	{ 2666, 235, 235, 235 },
	{ 2673, 237, 237, 237 },
	{ 2680, 240, 240, 240 },
	{ 2687, 242, 242, 242 },
	{ 2694, 245, 245, 245 },
	{ 2701, 247, 247, 247 },
	{ 2708, 250, 250, 250 },
	{ 2715, 252, 252, 252 },
	{ 2722, 0, 255, 0 },
	{ 2728, 173, 255, 47 },
	{ 2741, 0, 255, 0 },
	{ 2748, 0, 238, 0 },
	{ 2755, 0, 205, 0 },
	{ 2762, 0, 139, 0 },
	{ 2769, 173, 255, 47 },
	{ 2781, 190, 190, 190 },
	{ 2786, 0, 0, 0 },
	{ 2792, 3, 3, 3 },
	{ 2798, 26, 26, 26 },
	{ 2805, 255, 255, 255 },
	{ 2813, 28, 28, 28 },
	{ 2820, 31, 31, 31 },
	{ 2827, 33, 33, 33 },
	{ 2834, 36, 36, 36 },
	{ 2841, 38, 38, 38 },
	{ 2848, 41, 41, 41 },
	{ 2855, 43, 43, 43 },
	{ 2862, 46, 46, 46 },
	{ 2869, 48, 48, 48 },
	{ 2876, 5, 5, 5 },
	{ 2882, 51, 51, 51 },
	{ 2889, 54, 54, 54 },
	{ 2896, 56, 56, 56 },
	{ 2903, 59, 59, 59 },
	{ 2910, 61, 61, 61 },
	{ 2917, 64, 64, 64 },
	{ 2924, 66, 66, 66 },
	{ 2931, 69, 69, 69 },
	{ 2938, 71, 71, 71 },
	{ 2945, 74, 74, 74 },
	{ 2952, 8, 8, 8 },
	{ 2958, 77, 77, 77 },
	{ 2965, 79, 79, 79 },
	{ 2972, 82, 82, 82 },
	{ 2979, 84, 84, 84 },
	{ 2986, 87, 87, 87 },
	{ 2993, 89, 89, 89 },
	{ 3000, 92, 92, 92 },
	{ 3007, 94, 94, 94 },
	{ 3014, 97, 97, 97 },
	{ 3021, 99, 99, 99 },
	{ 3028, 10, 10, 10 },
	{ 3034, 102, 102, 102 },
	{ 3041, 105, 105, 105 },
	{ 3048, 107, 107, 107 },
	{ 3055, 110, 110, 110 },
	{ 3062, 112, 112, 112 },
	{ 3069, 115, 115, 115 },
	{ 3076, 117, 117, 117 },
	{ 3083, 120, 120, 120 },
	{ 3090, 122, 122, 122 },
	{ 3097, 125, 125, 125 },
	{ 3104, 13, 13, 13 },
	{ 3110, 127, 127, 127 },
	{ 3117, 130, 130, 130 },
	{ 3124, 133, 133, 133 },
	{ 3131, 135, 135, 135 },
	{ 3138, 138, 138, 138 },
	{ 3145, 140, 140, 140 },
	{ 3152, 143, 143, 143 },
	{ 3159, 145, 145, 145 },
	{ 3166, 148, 148, 148 },
	{ 3173, 150, 150, 150 },
	{ 3180, 15, 15, 15 },
	{ 3186, 153, 153, 153 },
	{ 3193, 156, 156, 156 },
	{ 3200, 158, 158, 158 },
	{ 3207, 161, 161, 161 },
	{ 3214, 163, 163, 163 },
	{ 3221, 166, 166, 166 },
	{ 3228, 168, 168, 168 },
	{ 3235, 171, 171, 171 },
	{ 3242, 173, 173, 173 },
	{ 3249, 176, 176, 176 },
	{ 3256, 18, 18, 18 },
	{ 3262, 179, 179, 179 },
	{ 3269, 181, 181, 181 },
	{ 3276, 184, 184, 184 },
	{ 3283, 186, 186, 186 },
	{ 3290, 189, 189, 189 },
	{ 3297, 191, 191, 191 },
	{ 3304, 194, 194, 194 },
	{ 3311, 196, 196, 196 },
	{ 3318, 199, 199, 199 },
	{ 3325, 201, 201, 201 },
	{ 3332, 20, 20, 20 },
	{ 3338, 204, 204, 204 },
	{ 3345, 207, 207, 207 },
	{ 3352, 209, 209, 209 },
	{ 3359, 212, 212, 212 },
	{ 3366, 214, 214, 214 },
	{ 3373, 217, 217, 217 },
	{ 3380, 219, 219, 219 },
	{ 3387, 222, 222, 222 },
	{ 3394, 224, 224, 224 },
	{ 3401, 227, 227, 227 },
	{ 3408, 23, 23, 23 },
	{ 3414, 229, 229, 229 },
	{ 3421, 232, 232, 232 },
	{ 3428, 235, 235, 235 },
	{ 3435, 237, 237, 237 },
	{ 3442, 240, 240, 240 },
	{ 3449, 242, 242, 242 },
	{ 3456, 245, 245, 245 },
	{ 3463, 247, 247, 247 },
	{ 3470, 250, 250, 250 },
	{ 3477, 252, 252, 252 },
	{ 3484, 240, 255, 240 },
	{ 3493, 240, 255, 240 },
	{ 3503, 224, 238, 224 },
	{ 3513, 193, 205, 193 },
	{ 3523, 131, 139, 131 },
	{ 3533, 255, 105, 180 },
	{ 3542, 255, 105, 180 },
	{ 3550, 255, 110, 180 },
	{ 3559, 238, 106, 167 },
	{ 3568, 205, 96, 144 },
	{ 3577, 139, 58, 98 },
	{ 3586, 205, 92, 92 },
	{ 3597, 205, 92, 92 },
	{ 3607, 255, 106, 106 },
	{ 3618, 238, 99, 99 },
	{ 3629, 205, 85, 85 },
	{ 3640, 139, 58, 58 },
	{ 3651, 75, 0, 130 },
	{ 3658, 255, 255, 240 },
	{ 3664, 255, 255, 240 },
	{ 3671, 238, 238, 224 },
	{ 3678, 205, 205, 193 },
	{ 3685, 139, 139, 131 },
	{ 3692, 240, 230, 140 },
	{ 3698, 255, 246, 143 },
	{ 3705, 238, 230, 133 },
	{ 3712, 205, 198, 115 },
	{ 3719, 139, 134, 78 },
	{ 3726, 230, 230, 250 },
	{ 3735, 255, 240, 245 },
	{ 3750, 255, 240, 245 },
	{ 3764, 255, 240, 245 },
	{ 3779, 238, 224, 229 },
	{ 3794, 205, 193, 197 },
	{ 3809, 139, 131, 134 },
	{ 3824, 124, 252, 0 },
	{ 3835, 124, 252, 0 },
	{ 3845, 255, 250, 205 },
	{ 3859, 255, 250, 205 },
	{ 3872, 255, 250, 205 },
	{ 3886, 238, 233, 191 },
	{ 3900, 205, 201, 165 },
	{ 3914, 139, 137, 112 },
	{ 3928, 173, 216, 230 },
	{ 3939, 240, 128, 128 },
	{ 3951, 224, 255, 255 },
	{ 3962, 238, 221, 130 },
	{ 3978, 250, 250, 210 },
	{ 4001, 211, 211, 211 },
	{ 4012, 144, 238, 144 },
	{ 4024, 211, 211, 211 },
	{ 4035, 255, 182, 193 },
	{ 4046, 255, 160, 122 },
	{ 4059, 32, 178, 170 },
	{ 4075, 135, 206, 250 },
	{ 4090, 132, 112, 255 },
	{ 4107, 119, 136, 153 },
	{ 4124, 119, 136, 153 },
	{ 4141, 176, 196, 222 },
	{ 4158, 255, 255, 224 },
	{ 4171, 173, 216, 230 },
	{ 4181, 191, 239, 255 },
	{ 4192, 178, 223, 238 },
	{ 4203, 154, 192, 205 },
	{ 4214, 104, 131, 139 },
	{ 4225, 240, 128, 128 },
	{ 4236, 224, 255, 255 },
	{ 4246, 224, 255, 255 },
	{ 4257, 209, 238, 238 },
	{ 4268, 180, 205, 205 },
	{ 4279, 122, 139, 139 },
	{ 4290, 238, 221, 130 },
	{ 4305, 255, 236, 139 },
	{ 4321, 238, 220, 130 },
	{ 4337, 205, 190, 112 },
	{ 4353, 139, 129, 76 },
	{ 4369, 250, 250, 210 },
	{ 4390, 211, 211, 211 },
	{ 4400, 144, 238, 144 },
	{ 4411, 211, 211, 211 },
	{ 4421, 255, 182, 193 },
	{ 4431, 255, 174, 185 },
	{ 4442, 238, 162, 173 },
	{ 4453, 205, 140, 149 },
	{ 4464, 139, 95, 101 },
	{ 4475, 255, 160, 122 },
	{ 4487, 255, 160, 122 },
	{ 4500, 238, 149, 114 },
	{ 4513, 205, 129, 98 },
	{ 4526, 139, 87, 66 },
	{ 4539, 32, 178, 170 },
	{ 4553, 135, 206, 250 },
	{ 4566, 176, 226, 255 },
	{ 4580, 164, 211, 238 },
	{ 4594, 141, 182, 205 },
	{ 4608, 96, 123, 139 },
	{ 4622, 132, 112, 255 },
	{ 4637, 119, 136, 153 },
	{ 4652, 119, 136, 153 },
	{ 4667, 176, 196, 222 },
	{ 4682, 202, 225, 255 },
	{ 4698, 188, 210, 238 },
	{ 4714, 162, 181, 205 },
	{ 4730, 110, 123, 139 },
	{ 4746, 255, 255, 224 },
	{ 4758, 255, 255, 224 },
	{ 4771, 238, 238, 209 },
	{ 4784, 205, 205, 180 },
	{ 4797, 139, 139, 122 },
	{ 4810, 0, 255, 0 },
	{ 4815, 50, 205, 50 },
	{ 4826, 50, 205, 50 },
	{ 4836, 250, 240, 230 },
	{ 4842, 255, 0, 255 },
	{ 4850, 255, 0, 255 },
	{ 4859, 238, 0, 238 },
	{ 4868, 205, 0, 205 },
	{ 4877, 139, 0, 139 },
	{ 4886, 176, 48, 96 },
	{ 4893, 255, 52, 179 },
	{ 4901, 238, 48, 167 },
	{ 4909, 205, 41, 144 },
	{ 4917, 139, 28, 98 },
	{ 4925, 102, 205, 170 },
	{ 4943, 0, 0, 205 },
	{ 4955, 186, 85, 211 },
	{ 4969, 147, 112, 219 },
	{ 4983, 60, 179, 113 },
	{ 5000, 123, 104, 238 },
	{ 5018, 0, 250, 154 },
	{ 5038, 72, 209, 204 },
	{ 5055, 199, 21, 133 },
	{ 5073, 102, 205, 170 },
	{ 5090, 0, 0, 205 },
	{ 5101, 186, 85, 211 },
	{ 5114, 224, 102, 255 },
	{ 5128, 209, 95, 238 },
	{ 5142, 180, 82, 205 },
	{ 5156, 122, 55, 139 },
	{ 5170, 147, 112, 219 },
	{ 5183, 171, 130, 255 },
	{ 5197, 159, 121, 238 },
	{ 5211, 137, 104, 205 },
	{ 5225, 93, 71, 139 },
	{ 5239, 60, 179, 113 },
	{ 5254, 123, 104, 238 },
	{ 5270, 0, 250, 154 },
	{ 5288, 72, 209, 204 },
	{ 5304, 199, 21, 133 },
	{ 5320, 25, 25, 112 },
	{ 5334, 25, 25, 112 },
	{ 5347, 245, 255, 250 },
	{ 5358, 245, 255, 250 },
	{ 5368, 255, 228, 225 },
	{ 5379, 255, 228, 225 },
	{ 5389, 255, 228, 225 },
	{ 5400, 238, 213, 210 },
	{ 5411, 205, 183, 181 },
	{ 5422, 139, 125, 123 },
	{ 5433, 255, 228, 181 },
	{ 5442, 255, 222, 173 },
	{ 5455, 255, 222, 173 },
	{ 5467, 255, 222, 173 },
	{ 5480, 238, 207, 161 },
	{ 5493, 205, 179, 139 },
	{ 5506, 139, 121, 94 },
	{ 5519, 0, 0, 128 },
	{ 5524, 0, 0, 128 },
	{ 5534, 0, 0, 128 },
	{ 5543, 253, 245, 230 },
	{ 5552, 253, 245, 230 },
	{ 5560, 128, 128, 0 },
	{ 5566, 107, 142, 35 },
	{ 5577, 107, 142, 35 },
	{ 5587, 192, 255, 62 },
	{ 5598, 179, 238, 58 },
	{ 5609, 154, 205, 50 },
	{ 5620, 105, 139, 34 },
	{ 5631, 255, 165, 0 },
	{ 5638, 255, 69, 0 },
	{ 5649, 255, 165, 0 },
	{ 5657, 238, 154, 0 },
	{ 5665, 205, 133, 0 },
	{ 5673, 139, 90, 0 },
	{ 5681, 255, 69, 0 },
	{ 5691, 255, 69, 0 },
	{ 5702, 238, 64, 0 },
	{ 5713, 205, 55, 0 },
	{ 5724, 139, 37, 0 },
	{ 5735, 218, 112, 214 },
	{ 5742, 255, 131, 250 },
	{ 5750, 238, 122, 233 },
	{ 5758, 205, 105, 201 },
	{ 5766, 139, 71, 137 },
	{ 5774, 238, 232, 170 },
	{ 5789, 152, 251, 152 },
	{ 5800, 175, 238, 238 },
	{ 5815, 219, 112, 147 },
	{ 5831, 238, 232, 170 },
	{ 5845, 152, 251, 152 },
	{ 5855, 154, 255, 154 },
	{ 5866, 144, 238, 144 },
	{ 5877, 124, 205, 124 },
	{ 5888, 84, 139, 84 },
	{ 5899, 175, 238, 238 },
	{ 5913, 187, 255, 255 },
	{ 5928, 174, 238, 238 },
	{ 5943, 150, 205, 205 },
	{ 5958, 102, 139, 139 },
	{ 5973, 219, 112, 147 },
	{ 5987, 255, 130, 171 },
	{ 6002, 238, 121, 159 },
	{ 6017, 205, 104, 137 },
	{ 6032, 139, 71, 93 },
	{ 6047, 255, 239, 213 },
	{ 6059, 255, 239, 213 },
	{ 6070, 255, 218, 185 },
	{ 6081, 255, 218, 185 },
	{ 6091, 255, 218, 185 },
	{ 6102, 238, 203, 173 },
	{ 6113, 205, 175, 149 },
	{ 6124, 139, 119, 101 },
	{ 6135, 205, 133, 63 },
	{ 6140, 255, 192, 203 },
	{ 6145, 255, 181, 197 },
	{ 6151, 238, 169, 184 },
	{ 6157, 205, 145, 158 },
	{ 6163, 139, 99, 108 },
	{ 6169, 221, 160, 221 },
	{ 6174, 255, 187, 255 },
	{ 6180, 238, 174, 238 },
	{ 6186, 205, 150, 205 },
	{ 6192, 139, 102, 139 },
	{ 6198, 176, 224, 230 },
	{ 6210, 176, 224, 230 },
	{ 6221, 160, 32, 240 },
	{ 6228, 155, 48, 255 },
	{ 6236, 145, 44, 238 },
	{ 6244, 125, 38, 205 },
	{ 6252, 85, 26, 139 },
	{ 6260, 102, 51, 153 },
	{ 6275, 102, 51, 153 },
	{ 6289, 255, 0, 0 },
	{ 6293, 255, 0, 0 },
	{ 6298, 238, 0, 0 },
	{ 6303, 205, 0, 0 },
	{ 6308, 139, 0, 0 },
	{ 6313, 188, 143, 143 },
	{ 6324, 188, 143, 143 },
	{ 6334, 255, 193, 193 },
	{ 6345, 238, 180, 180 },
	{ 6356, 205, 155, 155 },
	{ 6367, 139, 105, 105 },
	{ 6378, 65, 105, 225 },
	{ 6389, 65, 105, 225 },
	{ 6399, 72, 118, 255 },
	{ 6410, 67, 110, 238 },
	{ 6421, 58, 95, 205 },
	{ 6432, 39, 64, 139 },
	{ 6443, 139, 69, 19 },
	{ 6456, 139, 69, 19 },
	{ 6468, 250, 128, 114 },
	{ 6475, 255, 140, 105 },
	{ 6483, 238, 130, 98 },
	{ 6491, 205, 112, 84 },
	{ 6499, 139, 76, 57 },
	{ 6507, 244, 164, 96 },
	{ 6519, 244, 164, 96 },
	{ 6530, 46, 139, 87 },
	{ 6540, 46, 139, 87 },
	{ 6549, 84, 255, 159 },
	{ 6559, 78, 238, 148 },
	{ 6569, 67, 205, 128 },
	{ 6579, 46, 139, 87 },
	{ 6589, 255, 245, 238 },
	{ 6598, 255, 245, 238 },
	{ 6608, 238, 229, 222 },
	{ 6618, 205, 197, 191 },
	{ 6628, 139, 134, 130 },
	{ 6638, 160, 82, 45 },
	{ 6645, 255, 130, 71 },
	{ 6653, 238, 121, 66 },
	{ 6661, 205, 104, 57 },
	{ 6669, 139, 71, 38 },
	{ 6677, 192, 192, 192 },
	{ 6684, 135, 206, 235 },
	{ 6693, 135, 206, 235 },
	{ 6701, 135, 206, 255 },
	{ 6710, 126, 192, 238 },
	{ 6719, 108, 166, 205 },
	{ 6728, 74, 112, 139 },
	{ 6737, 106, 90, 205 },
	{ 6748, 112, 128, 144 },
	{ 6759, 112, 128, 144 },
	{ 6770, 106, 90, 205 },
	{ 6780, 131, 111, 255 },
	{ 6791, 122, 103, 238 },
	{ 6802, 105, 89, 205 },
	{ 6813, 71, 60, 139 },
	{ 6824, 112, 128, 144 },
	{ 6834, 198, 226, 255 },
	{ 6845, 185, 211, 238 },
	{ 6856, 159, 182, 205 },
	{ 6867, 108, 123, 139 },
	{ 6878, 112, 128, 144 },
	{ 6888, 255, 250, 250 },
	{ 6893, 255, 250, 250 },
	{ 6899, 238, 233, 233 },
	{ 6905, 205, 201, 201 },
	{ 6911, 139, 137, 137 },
	{ 6917, 0, 255, 127 },
	{ 6930, 0, 255, 127 },
	{ 6942, 0, 255, 127 },
	{ 6955, 0, 238, 118 },
	{ 6968, 0, 205, 102 },
	{ 6981, 0, 139, 69 },
	{ 6994, 70, 130, 180 },
	{ 7005, 70, 130, 180 },
	{ 7015, 99, 184, 255 },
	{ 7026, 92, 172, 238 },
	{ 7037, 79, 148, 205 },
	{ 7048, 54, 100, 139 },
	{ 7059, 210, 180, 140 },
	{ 7063, 255, 165, 79 },
	{ 7068, 238, 154, 73 },
	{ 7073, 205, 133, 63 },
	{ 7078, 139, 90, 43 },
	{ 7083, 0, 128, 128 },
	{ 7088, 216, 191, 216 },
	{ 7096, 255, 225, 255 },
	{ 7105, 238, 210, 238 },
	{ 7114, 205, 181, 205 },
	{ 7123, 139, 123, 139 },
	{ 7132, 255, 99, 71 },
	{ 7139, 255, 99, 71 },
	{ 7147, 238, 92, 66 },
	{ 7155, 205, 79, 57 },
	{ 7163, 139, 54, 38 },
	{ 7171, 64, 224, 208 },
	{ 7181, 0, 245, 255 },
	{ 7192, 0, 229, 238 },
	{ 7203, 0, 197, 205 },
	{ 7214, 0, 134, 139 },
	{ 7225, 238, 130, 238 },
	{ 7232, 208, 32, 144 },
	{ 7243, 208, 32, 144 },
	{ 7253, 255, 62, 150 },
	{ 7264, 238, 58, 140 },
	{ 7275, 205, 50, 120 },
	{ 7286, 139, 34, 82 },
	{ 7297, 128, 128, 128 },
	{ 7306, 0, 128, 0 },
	{ 7316, 128, 128, 128 },
	{ 7325, 128, 0, 0 },
	{ 7336, 128, 0, 128 },
	{ 7347, 128, 128, 128 },
	{ 7355, 0, 128, 0 },
	{ 7364, 128, 128, 128 },
	{ 7372, 128, 0, 0 },
	{ 7382, 128, 0, 128 },
	{ 7392, 245, 222, 179 },
	{ 7398, 255, 231, 186 },
	{ 7405, 238, 216, 174 },
	{ 7412, 205, 186, 150 },
	{ 7419, 139, 126, 102 },
	{ 7426, 255, 255, 255 },
	{ 7432, 245, 245, 245 },
	{ 7444, 245, 245, 245 },
	{ 7455, 190, 190, 190 },
	{ 7464, 0, 255, 0 },
	{ 7474, 190, 190, 190 },
	{ 7483, 176, 48, 96 },
	{ 7494, 160, 32, 240 },
	{ 7505, 190, 190, 190 },
	{ 7513, 0, 255, 0 },
	{ 7522, 190, 190, 190 },
	{ 7530, 176, 48, 96 },
	{ 7540, 160, 32, 240 },
	{ 7550, 255, 255, 0 },
	{ 7557, 154, 205, 50 },
	{ 7570, 255, 255, 0 },
	{ 7578, 238, 238, 0 },
	{ 7586, 205, 205, 0 },
	{ 7594, 139, 139, 0 },
	{ 7602, 154, 205, 50 }
};
#ifndef LABWC_DND_H
#define LABWC_DND_H
#include <wayland-server-core.h>
struct seat;
void dnd_init(struct seat *seat);
void dnd_icons_show(struct seat *seat, bool show);
void dnd_icons_move(struct seat *seat, double x, double y);
void dnd_finish(struct seat *seat);
#endif
#ifndef LABWC_LAYERS_H
#define LABWC_LAYERS_H
#include <wayland-server-core.h>
struct server;
struct output;
struct seat;
struct lab_layer_surface {
	struct wlr_layer_surface_v1 *layer_surface;
	struct wlr_scene_layer_surface_v1 *scene_layer_surface;
	bool mapped;
	bool being_unmapped;
	struct wl_listener map;
	struct wl_listener unmap;
	struct wl_listener surface_commit;
	struct wl_listener output_destroy;
	struct wl_listener node_destroy;
	struct wl_listener new_popup;
};
struct lab_layer_popup {
	struct wlr_xdg_popup *wlr_popup;
	struct wlr_scene_tree *scene_tree;
	bool parent_was_focused;
	struct wlr_box output_toplevel_sx_box;
	struct wl_listener commit;
	struct wl_listener destroy;
	struct wl_listener new_popup;
	struct wl_listener reposition;
};
void layers_init(void);
void layers_finish(void);
void layers_arrange(struct output *output);
void layer_try_set_focus(struct seat *seat,
	struct wlr_layer_surface_v1 *layer_surface);
#endif
#ifndef LABWC_EDGES_H
#define LABWC_EDGES_H
#include <limits.h>
#include <stdbool.h>
struct border;
struct output;
struct server;
struct view;
struct wlr_box;
static inline int
clipped_add(int a, int b)
{
	if (b > 0) {
		return a >= (INT_MAX - b) ? INT_MAX : (a + b);
	} else if (b < 0) {
		return a <= (INT_MIN - b) ? INT_MIN : (a + b);
	}
	return a;
}
static inline int
clipped_sub(int a, int b)
{
	if (b > 0) {
		return a <= (INT_MIN + b) ? INT_MIN : (a - b);
	} else if (b < 0) {
		return a >= (INT_MAX + b) ? INT_MAX : (a - b);
	}
	return a;
}
static inline int
edge_get_best(int next, int edge, bool decreasing)
{
	if (!BOUNDED_INT(next)) {
		return BOUNDED_INT(edge) ? edge : next;
	}
	if (!BOUNDED_INT(edge)) {
		return next;
	}
	return decreasing ? MAX(next, edge) : MIN(next, edge);
}
struct edge {
	int offset;
	int min;
	int max;
};
typedef void (*edge_validator_t)(int *best, struct edge current,
	struct edge target, struct edge oppose, struct edge align, bool lesser);
void edges_initialize(struct border *edges);
void edges_find_neighbors(struct border *nearest_edges, struct view *view,
	struct wlr_box origin, struct wlr_box target,
	struct output *output, edge_validator_t validator, bool ignore_hidden);
void edges_find_outputs(struct border *nearest_edges, struct view *view,
	struct wlr_box origin, struct wlr_box target,
	struct output *output, edge_validator_t validator);
void edges_adjust_move_coords(struct view *view, struct border edges,
	int *x, int *y, bool use_pending);
void edges_adjust_resize_geom(struct view *view, struct border edges,
	enum lab_edge resize_edges, struct wlr_box *geom, bool use_pending);
bool edges_traverse_edge(struct edge current, struct edge target, struct edge edge);
void edges_calculate_visibility(struct view *ignored_view);
#endif
#ifndef LABWC_IDLE_H
#define LABWC_IDLE_H
struct wl_display;
struct wlr_seat;
void idle_manager_create(struct wl_display *display);
void idle_manager_notify_activity(struct wlr_seat *wlr_seat);
#endif
#ifndef LABWC_MOUSEBIND_H
#define LABWC_MOUSEBIND_H
#include <stdbool.h>
#include <wayland-util.h>
enum mouse_event {
	MOUSE_ACTION_NONE = 0,
	MOUSE_ACTION_DOUBLECLICK,
	MOUSE_ACTION_CLICK,
	MOUSE_ACTION_PRESS,
	MOUSE_ACTION_RELEASE,
	MOUSE_ACTION_DRAG,
	MOUSE_ACTION_SCROLL,
};
enum direction {
	LAB_DIRECTION_INVALID = 0,
	LAB_DIRECTION_LEFT,
	LAB_DIRECTION_RIGHT,
	LAB_DIRECTION_UP,
	LAB_DIRECTION_DOWN,
};
struct mousebind {
	enum lab_node_type context;
	uint32_t button;
	enum direction direction;
	uint32_t modifiers;
	enum mouse_event mouse_event;
	struct wl_list actions;
	struct wl_list link;
	bool pressed_in_context;
};
enum mouse_event mousebind_event_from_str(const char *str);
uint32_t mousebind_button_from_str(const char *str, uint32_t *modifiers);
enum direction mousebind_direction_from_str(const char *str, uint32_t *modifiers);
struct mousebind *mousebind_create(const char *context);
bool mousebind_the_same(struct mousebind *a, struct mousebind *b);
#endif
#ifndef LABWC_GESTURES_H
#define LABWC_GESTURES_H
struct seat;
void gestures_init(struct seat *seat);
void gestures_finish(struct seat *seat);
#endif
#ifndef LABWC_RESISTANCE_H
#define LABWC_RESISTANCE_H
#include <stdbool.h>
struct view;
bool resistance_unsnap_apply(struct view *view, int *x, int *y);
void resistance_move_apply(struct view *view, int *x, int *y);
void resistance_resize_apply(struct view *view, struct wlr_box *new_view_geo);
#endif
#ifndef LABWC_RESIZE_OUTLINES_H
#define LABWC_RESIZE_OUTLINES_H
struct view;
void resize_outlines_update(struct view *view, struct wlr_box new_geo);
void resize_outlines_finish(struct view *view);
bool resize_outlines_enabled(struct view *view);
#endif
#ifndef LABWC_XWAYLAND_H
#define LABWC_XWAYLAND_H
#if HAVE_XWAYLAND
struct wlr_compositor;
struct wlr_output;
struct wlr_output_layout;
struct xwayland_unmanaged {
	struct wlr_xwayland_surface *xwayland_surface;
	struct wlr_scene_node *node;
	struct wl_list link;
	struct mappable mappable;
	struct wl_listener associate;
	struct wl_listener dissociate;
	struct wl_listener grab_focus;
	struct wl_listener request_activate;
	struct wl_listener request_configure;
	struct wl_listener set_geometry;
	struct wl_listener destroy;
	struct wl_listener set_override_redirect;
	bool ever_grabbed_focus;
};
struct xwayland_view {
	struct view base;
	struct wlr_xwayland_surface *xwayland_surface;
	bool focused_before_map;
	bool initial_geometry_set;
	struct wl_listener associate;
	struct wl_listener dissociate;
	struct wl_listener request_above;
	struct wl_listener request_activate;
	struct wl_listener request_close;
	struct wl_listener request_configure;
	struct wl_listener set_class;
	struct wl_listener set_decorations;
	struct wl_listener set_override_redirect;
	struct wl_listener set_strut_partial;
	struct wl_listener set_window_type;
	struct wl_listener focus_in;
	struct {
		struct wl_listener always_on_top;
	} on_view;
};
void xwayland_unmanaged_create(struct wlr_xwayland_surface *xsurface, bool mapped);
void xwayland_view_create(struct wlr_xwayland_surface *xsurface, bool mapped);
void xwayland_server_init(struct wlr_compositor *compositor);
void xwayland_server_finish(void);
void xwayland_adjust_usable_area(struct view *view,
	struct wlr_output_layout *layout, struct wlr_output *output,
	struct wlr_box *usable);
void xwayland_update_workarea(void);
void xwayland_flush(void);
#endif
#endif
#ifndef LABWC_IME_H
#define LABWC_IME_H
#include <wayland-server-core.h>
struct keyboard;
struct wlr_keyboard_key_event;
struct input_method_relay {
	struct seat *seat;
	struct wl_list text_inputs;
	struct wlr_input_method_v2 *input_method;
	struct wlr_surface *focused_surface;
	struct text_input *active_text_input;
	struct wl_list popups;
	struct wlr_scene_tree *popup_tree;
	struct wl_listener new_text_input;
	struct wl_listener new_input_method;
	struct wl_listener input_method_commit;
	struct wl_listener input_method_grab_keyboard;
	struct wl_listener input_method_destroy;
	struct wl_listener input_method_new_popup_surface;
	struct wl_listener keyboard_grab_destroy;
	struct wl_listener focused_surface_destroy;
};
struct input_method_popup {
	struct wlr_input_popup_surface_v2 *popup_surface;
	struct wlr_scene_tree *tree;
	struct input_method_relay *relay;
	struct wl_list link;
	struct wl_listener destroy;
	struct wl_listener commit;
};
struct text_input {
	struct input_method_relay *relay;
	struct wlr_text_input_v3 *input;
	struct wl_list link;
	struct wl_listener enable;
	struct wl_listener commit;
	struct wl_listener disable;
	struct wl_listener destroy;
};
bool input_method_keyboard_grab_forward_key(struct keyboard *keyboard,
	struct wlr_keyboard_key_event *event);
bool input_method_keyboard_grab_forward_modifiers(struct keyboard *keyboard);
struct input_method_relay *input_method_relay_create(struct seat *seat);
void input_method_relay_finish(struct input_method_relay *relay);
void input_method_relay_set_focus(struct input_method_relay *relay,
	struct wlr_surface *surface);
#endif
#ifndef LABWC_KEYBIND_H
#define LABWC_KEYBIND_H
#include <stdbool.h>
#include <wayland-util.h>
#include <xkbcommon/xkbcommon.h>
#define MAX_KEYSYMS 32
#define MAX_KEYCODES 16
struct server;
struct keybind {
	uint32_t modifiers;
	xkb_keysym_t *keysyms;
	size_t keysyms_len;
	bool use_syms_only;
	xkb_keycode_t keycodes[MAX_KEYCODES];
	size_t keycodes_len;
	int keycodes_layout;
	bool allow_when_locked;
	struct wl_list actions;
	struct wl_list link;
	bool on_release;
	bool override_inhibition;
};
struct keybind *keybind_create(const char *keybind);
void keybind_destroy(struct keybind *keybind);
uint32_t parse_modifier(const char *symname);
bool keybind_the_same(struct keybind *a, struct keybind *b);
bool keybind_contains_keycode(struct keybind *keybind, xkb_keycode_t keycode);
bool keybind_contains_keysym(struct keybind *keybind, xkb_keysym_t keysym);
void keybind_update_keycodes(void);
#endif
#ifndef LABWC_SESSION_LOCK_H
#define LABWC_SESSION_LOCK_H
#include <wayland-server-core.h>
struct output;
struct server;
struct session_lock_manager {
	struct wlr_session_lock_manager_v1 *wlr_manager;
	struct view *last_active_view;
	struct wlr_surface *focused;
	struct wlr_session_lock_v1 *lock;
	bool locked;
	struct wl_list lock_outputs;
	struct wl_listener new_lock;
	struct wl_listener destroy;
	struct wl_listener lock_new_surface;
	struct wl_listener lock_unlock;
	struct wl_listener lock_destroy;
};
void session_lock_init(void);
void session_lock_output_create(struct session_lock_manager *manager, struct output *output);
void session_lock_update_for_layout_change(void);
#endif
#ifndef LABWC_SESSION_H
#define LABWC_SESSION_H
struct server;
void session_environment_init(void);
void session_autostart_init(void);
void session_shutdown(void);
#endif
#ifndef LABWC_OUTPUT_STATE_H
#define LABWC_OUTPUT_STATE_H
#include <stdbool.h>
struct output;
void output_state_init(struct output *output);
bool output_state_commit(struct output *output);
#endif
#ifndef LABWC_PLACEMENT_H
#define LABWC_PLACEMENT_H
#include <stdbool.h>
struct view;
bool placement_find_best(struct view *view, struct wlr_box *geometry);
#endif
#ifndef LABWC_RESIZE_INDICATOR_H
#define LABWC_RESIZE_INDICATOR_H
struct server;
struct view;
void resize_indicator_reconfigure(void);
void resize_indicator_show(struct view *view);
void resize_indicator_update(struct view *view);
void resize_indicator_hide(struct view *view);
#endif
#ifndef LABWC_SNAP_CONSTRAINTS_H
#define LABWC_SNAP_CONSTRAINTS_H
struct view;
struct wlr_box;
void snap_constraints_set(struct view *view, enum lab_edge direction,
	struct wlr_box geom);
void snap_constraints_invalidate(struct view *view);
void snap_constraints_update(struct view *view);
struct wlr_box snap_constraints_effective(struct view *view,
	enum lab_edge direction, bool use_pending);
#endif
#ifndef LABWC_LIBINPUT_H
#define LABWC_LIBINPUT_H
#include <libinput.h>
#include <string.h>
#include <wayland-server-core.h>
enum lab_libinput_device_type {
	LAB_LIBINPUT_DEVICE_NONE = 0,
	LAB_LIBINPUT_DEVICE_DEFAULT,
	LAB_LIBINPUT_DEVICE_TOUCHPAD,
	LAB_LIBINPUT_DEVICE_NON_TOUCH,
};
struct libinput_category {
	enum lab_libinput_device_type type;
	char *name;
	struct wl_list link;
	float pointer_speed;
	int natural_scroll;
	int left_handed;
	enum libinput_config_tap_state tap;
	enum libinput_config_tap_button_map tap_button_map;
	int tap_and_drag;
	int drag_lock;
	int three_finger_drag;
	int accel_profile;
	int middle_emu;
	int dwt;
	int click_method;
	int scroll_method;
	int scroll_button;
	int send_events_mode;
	bool have_calibration_matrix;
	double scroll_factor;
	float calibration_matrix[6];
};
enum lab_libinput_device_type get_device_type(const char *s);
const char *libinput_device_type_name(enum lab_libinput_device_type type);
struct libinput_category *libinput_category_create(void);
struct libinput_category *libinput_category_get_default(void);
#endif
#ifndef LABWC_DECORATIONS_H
#define LABWC_DECORATIONS_H
struct server;
struct view;
struct wlr_surface;
void kde_server_decoration_init(void);
void xdg_server_decoration_init(void);
void kde_server_decoration_update_default(void);
void kde_server_decoration_set_view(struct view *view, struct wlr_surface *surface);
void kde_server_decoration_finish(void);
void xdg_server_decoration_finish(void);
#endif
#ifndef LABWC_ARRAY_H
#define LABWC_ARRAY_H
#include <wayland-server-core.h>
static inline size_t
wl_array_len(struct wl_array *array)
{
	return array->size / sizeof(const char *);
}
#define wl_array_for_each_reverse(pos, array)                                          \
	for (pos = !(array)->data ? NULL                                               \
		: (void *)((const char *)(array)->data + (array)->size - sizeof(pos)); \
		pos && (const char *)pos >= (const char *)(array)->data;               \
		(pos)--)
#define array_add(_arr, _val) do {                           \
		__typeof__(_val) *_entry = wl_array_add(     \
			(_arr), sizeof(__typeof__(_val)));   \
		die_if_null(_entry);                         \
		*_entry = (_val);                            \
	} while (0)
#endif
#ifndef LABWC_SNAP_H
#define LABWC_SNAP_H
struct view;
struct wlr_box;
void snap_move_to_edge(struct view *view,
	enum lab_edge direction, bool snap_to_windows, int *dx, int *dy);
void snap_grow_to_next_edge(struct view *view,
	enum lab_edge direction, struct wlr_box *geo);
void snap_shrink_to_next_edge(struct view *view,
	enum lab_edge direction, struct wlr_box *geo);
void snap_invalidate_edge_cache(struct view *view);
void snap_update_cache_geometry(struct view *view);
#endif
#ifndef LABWC_SSD_INTERNAL_H
#define LABWC_SSD_INTERNAL_H
struct ssd {
	struct view *view;
	struct wlr_scene_tree *tree;
	struct {
		bool was_shaded;
		bool was_omnipresent;
		bool was_maximized;
		bool was_squared;
		struct wlr_box geometry;
	} state;
	struct ssd_extents_scene {
		struct wlr_scene_tree *tree;
		struct wlr_scene_rect *top, *bottom, *left, *right;
	} extents;
	struct ssd_titlebar_scene {
		int height;
		struct wlr_scene_tree *tree;
		struct ssd_titlebar_subtree {
			struct wlr_scene_tree *tree;
			struct wlr_scene_buffer *corner_left;
			struct wlr_scene_buffer *corner_right;
			struct wlr_scene_buffer *bar;
			struct wl_list buttons_left;
			struct wl_list buttons_right;
		} subtrees[2];
	} titlebar;
	struct ssd_border_scene {
		struct wlr_scene_tree *tree;
		struct ssd_border_subtree {
			struct wlr_scene_tree *tree;
			struct wlr_scene_rect *top, *bottom, *left, *right;
		} subtrees[2];
	} border;
	struct ssd_shadow_scene {
		struct wlr_scene_tree *tree;
		struct ssd_shadow_subtree {
			struct wlr_scene_tree *tree;
			struct wlr_scene_buffer *top, *bottom, *left, *right,
				*top_left, *top_right, *bottom_left, *bottom_right;
		} subtrees[2];
	} shadow;
	struct border margin;
};
struct ssd_button {
	struct wlr_scene_node *node;
	enum lab_node_type type;
	uint8_t state_set;
	struct scaled_img_buffer *img_buffers[LAB_BS_ALL + 1];
	struct wl_list link;
};
struct wlr_buffer;
struct wlr_scene_tree;
struct ssd_button *attach_ssd_button(struct wl_list *button_parts,
	enum lab_node_type type, struct wlr_scene_tree *parent,
	struct lab_img *imgs[LAB_BS_ALL + 1], int x, int y,
	struct view *view);
void ssd_titlebar_create(struct ssd *ssd);
void ssd_titlebar_update(struct ssd *ssd);
void ssd_titlebar_destroy(struct ssd *ssd);
bool ssd_should_be_squared(struct ssd *ssd);
void ssd_border_create(struct ssd *ssd);
void ssd_border_update(struct ssd *ssd);
void ssd_border_destroy(struct ssd *ssd);
void ssd_extents_create(struct ssd *ssd);
void ssd_extents_update(struct ssd *ssd);
void ssd_extents_destroy(struct ssd *ssd);
void ssd_shadow_create(struct ssd *ssd);
void ssd_shadow_update(struct ssd *ssd);
void ssd_shadow_destroy(struct ssd *ssd);
#endif
#ifndef LABWC_VIEW_IMPL_COMMON_H
#define LABWC_VIEW_IMPL_COMMON_H
struct view;
void view_impl_map(struct view *view);
void view_impl_unmap(struct view *view);
void view_impl_apply_geometry(struct view *view, int w, int h);
#endif
#ifndef LABWC_FOREIGN_TOPLEVEL_H
#define LABWC_FOREIGN_TOPLEVEL_H
struct view;
struct foreign_toplevel;
struct foreign_toplevel *foreign_toplevel_create(struct view *view);
void foreign_toplevel_set_parent(struct foreign_toplevel *toplevel,
	struct foreign_toplevel *parent);
void foreign_toplevel_destroy(struct foreign_toplevel *toplevel);
#endif
#ifndef LABWC_WLR_FOREIGN_TOPLEVEL_H
#define LABWC_WLR_FOREIGN_TOPLEVEL_H
#include <wayland-server-core.h>
struct wlr_foreign_toplevel {
	struct view *view;
	struct wlr_foreign_toplevel_handle_v1 *handle;
	struct {
		struct wl_listener request_maximize;
		struct wl_listener request_minimize;
		struct wl_listener request_fullscreen;
		struct wl_listener request_activate;
		struct wl_listener request_close;
		struct wl_listener handle_destroy;
	} on;
	struct {
		struct wl_listener new_app_id;
		struct wl_listener new_title;
		struct wl_listener new_outputs;
		struct wl_listener maximized;
		struct wl_listener minimized;
		struct wl_listener fullscreened;
		struct wl_listener activated;
	} on_view;
};
void wlr_foreign_toplevel_init(struct wlr_foreign_toplevel *wlr_toplevel,
	struct view *view);
void wlr_foreign_toplevel_set_parent(struct wlr_foreign_toplevel *wlr_toplevel,
	struct wlr_foreign_toplevel *parent);
void wlr_foreign_toplevel_finish(struct wlr_foreign_toplevel *wlr_toplevel);
#endif
#ifndef LABWC_EXT_FOREIGN_TOPLEVEL_H
#define LABWC_EXT_FOREIGN_TOPLEVEL_H
#include <wayland-server-core.h>
struct ext_foreign_toplevel {
	struct view *view;
	struct wlr_ext_foreign_toplevel_handle_v1 *handle;
	struct {
		struct wl_listener handle_destroy;
	} on;
	struct {
		struct wl_listener new_app_id;
		struct wl_listener new_title;
	} on_view;
};
void ext_foreign_toplevel_init(struct ext_foreign_toplevel *ext_toplevel,
	struct view *view);
void ext_foreign_toplevel_finish(struct ext_foreign_toplevel *ext_toplevel);
#endif
#ifndef LABWC_DEFAULT_BINDINGS_H
#define LABWC_DEFAULT_BINDINGS_H
#include <stddef.h>
static struct key_combos {
	const char *binding, *action;
	struct {
		const char *name, *value;
	} attributes[2];
} key_combos[] = { {
		.binding = "W-j",
		.action = "NextWindow",
	}, {
		.binding = "W-k",
		.action = "PreviousWindow",
	}, {
		.binding = "W-Return",
		.action = "Execute",
		.attributes[0] = {
			.name = "command",
			.value = "footclient",
		},
	}, {
		.binding = "W-S-q",
		.action = "Close",
	}, {
		.binding = "W-a",
		.action = "ToggleMaximize",
	}, {
		.binding = "W-m",
		.action = "Iconify",
	},{
		.binding = "W-d",
		.action = "ToggleShowDesktop",
	},
	{
		.binding = "W-1",
		.action = "GoToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "1",
		},
	}, {
		.binding = "W-2",
		.action = "GoToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "2",
		},
	}, {
		.binding = "W-3",
		.action = "GoToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "3",
		},
	}, {
		.binding = "W-4",
		.action = "GoToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "4",
		},
	}, {
		.binding = "W-5",
		.action = "GoToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "5",
		},
	}, {
		.binding = "W-6",
		.action = "GoToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "6",
		},
	}, {
		.binding = "W-7",
		.action = "GoToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "7",
		},
	}, {
		.binding = "W-8",
		.action = "GoToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "8",
		},
	}, {
		.binding = "W-9",
		.action = "GoToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "9",
		},
	}, {
		.binding = "W-S-1",
		.action = "SendToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "1",
		},
		.attributes[1] = {
			.name = "follow",
			.value = "yes",
		},
	}, {
		.binding = "W-S-2",
		.action = "SendToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "2",
		},
		.attributes[1] = {
			.name = "follow",
			.value = "yes",
		},
	}, {
		.binding = "W-S-3",
		.action = "SendToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "3",
		},
		.attributes[1] = {
			.name = "follow",
			.value = "yes",
		},
	}, {
		.binding = "W-S-4",
		.action = "SendToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "4",
		},
		.attributes[1] = {
			.name = "follow",
			.value = "no",
		},
	}, {
		.binding = "W-S-5",
		.action = "SendToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "5",
		},
		.attributes[1] = {
			.name = "follow",
			.value = "no",
		},
	}, {
		.binding = "W-S-6",
		.action = "SendToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "6",
		},
		.attributes[1] = {
			.name = "follow",
			.value = "no",
		},
	}, {
		.binding = "W-S-7",
		.action = "SendToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "7",
		},
		.attributes[1] = {
			.name = "follow",
			.value = "no",
		},
	}, {
		.binding = "W-S-8",
		.action = "SendToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "8",
		},
		.attributes[1] = {
			.name = "follow",
			.value = "no",
		},
	}, {
		.binding = "W-S-9",
		.action = "SendToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "9",
		},
		.attributes[1] = {
			.name = "follow",
			.value = "no",
		},
	},
	{
		.binding = "W-S-h",
		.action = "SnapToEdge",
		.attributes[0] = {
			.name = "direction",
			.value = "left",
		},
		.attributes[1] = {
			.name = "combine",
			.value = "yes",
		},
	}, {
		.binding = "W-S-l",
		.action = "SnapToEdge",
		.attributes[0] = {
			.name = "direction",
			.value = "right",
		},
		.attributes[1] = {
			.name = "combine",
			.value = "yes",
		},
	}, {
		.binding = "W-S-k",
		.action = "SnapToEdge",
		.attributes[0] = {
			.name = "direction",
			.value = "up",
		},
		.attributes[1] = {
			.name = "combine",
			.value = "yes",
		},
	}, {
		.binding = "W-S-j",
		.action = "SnapToEdge",
		.attributes[0] = {
			.name = "direction",
			.value = "down",
		},
		.attributes[1] = {
			.name = "combine",
			.value = "yes",
		},
	}, {
		.binding = "W-S-a",
		.action = "UnSnap",
	}, {
		.binding = "W-i",
		.action = "ShowMenu",
		.attributes[0] = {
			.name = "menu",
			.value = "client-menu"
		},
		.attributes[1] = {
			.name = "atCursor",
			.value = "no",
		},
	}, {
		.binding = "W-Space",
		.action = "Execute",
		.attributes[0] = {
			.name = "command",
			.value = "jtlabctl menu",
		},
	},{
		.binding = "XF86AudioLowerVolume",
		.action = "Execute",
		.attributes[0] = {
			.name = "command",
			.value = "pactl set-sink-volume @DEFAULT_SINK@ -5%",
		},
	}, {
		.binding = "XF86AudioRaiseVolume",
		.action = "Execute",
		.attributes[0] = {
			.name = "command",
			.value = "pactl set-sink-volume @DEFAULT_SINK@ +5%",
		},
	}, {
		.binding = "XF86AudioMute",
		.action = "Execute",
		.attributes[0] = {
			.name = "command",
			.value = "pactl set-sink-mute @DEFAULT_SINK@ toggle",
		},
	}, {
		.binding = "XF86MonBrightnessUp",
		.action = "Execute",
		.attributes[0] = {
			.name = "command",
			.value = "brightnessctl set +10%",
		},
	}, {
		.binding = "XF86MonBrightnessDown",
		.action = "Execute",
		.attributes[0] = {
			.name = "command",
			.value = "brightnessctl set 10%-",
		},
	}, {
		.binding = NULL,
	},
};
static struct mouse_combos {
	const char *context, *button, *event, *action;
	struct {
		const char *name, *value;
	} attributes[2];
} mouse_combos[] = { {
		.context = "Border",
		.button = "Left",
		.event = "Press",
		.action = "Focus",
	}, {
		.context = "Border",
		.button = "Left",
		.event = "Press",
		.action = "Raise",
	}, {
		.context = "Border",
		.button = "Left",
		.event = "Drag",
		.action = "Resize",
	}, {
		.context = "Frame",
		.button = "W-Left",
		.event = "Press",
		.action = "Focus",
	}, {
		.context = "Frame",
		.button = "W-Left",
		.event = "Press",
		.action = "Raise",
	}, {
		.context = "Frame",
		.button = "W-Left",
		.event = "Drag",
		.action = "Move",
	}, {
		.context = "Frame",
		.button = "W-Right",
		.event = "Press",
		.action = "Focus",
	}, {
		.context = "Frame",
		.button = "W-Right",
		.event = "Press",
		.action = "Raise",
	}, {
		.context = "Frame",
		.button = "W-Right",
		.event = "Drag",
		.action = "Resize",
	}, {
		.context = "Titlebar",
		.button = "Left",
		.event = "Press",
		.action = "Focus",
	}, {
		.context = "Titlebar",
		.button = "Left",
		.event = "Press",
		.action = "Raise",
	}, {
		.context = "Titlebar",
		.button = "Up",
		.event = "Scroll",
		.action = "Unfocus",
	}, {
		.context = "Titlebar",
		.button = "Up",
		.event = "Scroll",
		.action = "Shade",
	}, {
		.context = "Titlebar",
		.button = "Down",
		.event = "Scroll",
		.action = "Unshade",
	}, {
		.context = "Titlebar",
		.button = "Down",
		.event = "Scroll",
		.action = "Focus",
	}, {
		.context = "Title",
		.button = "Left",
		.event = "Drag",
		.action = "Move",
	}, {
		.context = "Title",
		.button = "Left",
		.event = "DoubleClick",
		.action = "ToggleMaximize",
	}, {
		.context = "TitleBar",
		.button = "Right",
		.event = "Click",
		.action = "Focus",
	}, {
		.context = "TitleBar",
		.button = "Right",
		.event = "Click",
		.action = "Raise",
	}, {
		.context = "Title",
		.button = "Right",
		.event = "Click",
		.action = "ShowMenu",
		.attributes[0] = {
			.name = "menu",
			.value = "client-menu",
		},
		.attributes[1] = {
			.name = "atCursor",
			.value = "yes",
		},
	}, {
		.context = "Close",
		.button = "Left",
		.event = "Click",
		.action = "Close",
	}, {
		.context = "Iconify",
		.button = "Left",
		.event = "Click",
		.action = "Iconify",
	}, {
		.context = "Maximize",
		.button = "Left",
		.event = "Click",
		.action = "ToggleMaximize",
	}, {
		.context = "Shade",
		.button = "Left",
		.event = "Click",
		.action = "ToggleShade",
	}, {
		.context = "AllDesktops",
		.button = "Left",
		.event = "Click",
		.action = "ToggleOmnipresent",
	}, {
		.context = "Maximize",
		.button = "Right",
		.event = "Click",
		.action = "ToggleMaximize",
		.attributes[0] = {
			.name = "direction",
			.value = "horizontal",
		},
	}, {
		.context = "Maximize",
		.button = "Middle",
		.event = "Click",
		.action = "ToggleMaximize",
		.attributes[0] = {
			.name = "direction",
			.value = "vertical",
		},
	}, {
		.context = "WindowMenu",
		.button = "Left",
		.event = "Click",
		.action = "ShowMenu",
		.attributes[0] = {
			.name = "menu",
			.value = "client-menu",
		},
		.attributes[1] = {
			.name = "atCursor",
			.value = "no",
		},
	}, {
		.context = "WindowMenu",
		.button = "Right",
		.event = "Click",
		.action = "ShowMenu",
		.attributes[0] = {
			.name = "menu",
			.value = "client-menu",
		},
		.attributes[1] = {
			.name = "atCursor",
			.value = "no",
		},
	}, {
		.context = "Root",
		.button = "Up",
		.event = "Scroll",
		.action = "GoToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "left",
		},
	}, {
		.context = "Root",
		.button = "Down",
		.event = "Scroll",
		.action = "GoToDesktop",
		.attributes[0] = {
			.name = "to",
			.value = "right",
		},
	}, {
		.context = "Client",
		.button = "Left",
		.event = "Press",
		.action = "Focus",
	}, {
		.context = "Client",
		.button = "Left",
		.event = "Press",
		.action = "Raise",
	}, {
		.context = "Client",
		.button = "Right",
		.event = "Press",
		.action = "Focus",
	}, {
		.context = "Client",
		.button = "Right",
		.event = "Press",
		.action = "Raise",
	}, {
		.context = "Client",
		.button = "Middle",
		.event = "Press",
		.action = "Focus",
	}, {
		.context = "Client",
		.button = "Middle",
		.event = "Press",
		.action = "Raise",
	}, {
		.context = NULL,
	},
};
#endif
#include <assert.h>
#include <signal.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
enum LABWC_src_action_c_action_arg_type {
	LAB_ACTION_ARG_STR = 0,
	LAB_ACTION_ARG_BOOL,
	LAB_ACTION_ARG_INT,
	LAB_ACTION_ARG_QUERY_LIST,
	LAB_ACTION_ARG_ACTION_LIST,
};
struct action_arg {
	struct wl_list link;
	char *key;
	enum LABWC_src_action_c_action_arg_type type;
};
struct LABWC_src_action_c_action_arg_str {
	struct action_arg base;
	char *value;
};
struct LABWC_src_action_c_action_arg_bool {
	struct action_arg base;
	bool value;
};
struct LABWC_src_action_c_action_arg_int {
	struct action_arg base;
	int value;
};
struct LABWC_src_action_c_action_arg_list {
	struct action_arg base;
	struct wl_list value;
};
#define ACTION_TYPE_LIST(X) \
	X(NONE, "None") \
	X(CLOSE, "Close") \
	X(KILL, "Kill") \
	X(EXECUTE, "Execute") \
	X(EXIT, "Exit") \
	X(MOVE_TO_EDGE, "MoveToEdge") \
	X(TOGGLE_SNAP_TO_EDGE, "ToggleSnapToEdge") \
	X(SNAP_TO_EDGE, "SnapToEdge") \
	X(GROW_TO_EDGE, "GrowToEdge") \
	X(SHRINK_TO_EDGE, "ShrinkToEdge") \
	X(NEXT_WINDOW, "NextWindow") \
	X(PREVIOUS_WINDOW, "PreviousWindow") \
	X(NEXT_WINDOW_IMMEDIATE, "NextWindowImmediate") \
	X(PREVIOUS_WINDOW_IMMEDIATE, "PreviousWindowImmediate") \
	X(SHOW_MENU, "ShowMenu") \
	X(TOGGLE_MAXIMIZE, "ToggleMaximize") \
	X(MAXIMIZE, "Maximize") \
	X(UNMAXIMIZE, "UnMaximize") \
	X(TOGGLE_FULLSCREEN, "ToggleFullscreen") \
	X(SET_DECORATIONS, "SetDecorations") \
	X(TOGGLE_DECORATIONS, "ToggleDecorations") \
	X(TOGGLE_ALWAYS_ON_TOP, "ToggleAlwaysOnTop") \
	X(TOGGLE_ALWAYS_ON_BOTTOM, "ToggleAlwaysOnBottom") \
	X(TOGGLE_OMNIPRESENT, "ToggleOmnipresent") \
	X(FOCUS, "Focus") \
	X(UNFOCUS, "Unfocus") \
	X(ICONIFY, "Iconify") \
	X(RAISE, "Raise") \
	X(LOWER, "Lower") \
	X(MOVE, "Move") \
	X(RESIZE, "Resize") \
	X(RESIZE_RELATIVE, "ResizeRelative") \
	X(MOVETO, "MoveTo") \
	X(RESIZETO, "ResizeTo") \
	X(MOVETO_CURSOR, "MoveToCursor") \
	X(MOVE_RELATIVE, "MoveRelative") \
	X(SEND_TO_DESKTOP, "SendToDesktop") \
	X(GO_TO_DESKTOP, "GoToDesktop") \
	X(TOGGLE_SNAP_TO_REGION, "ToggleSnapToRegion") \
	X(SNAP_TO_REGION, "SnapToRegion") \
	X(UNSNAP, "UnSnap") \
	X(TOGGLE_KEYBINDS, "ToggleKeybinds") \
	X(FOCUS_OUTPUT, "FocusOutput") \
	X(MOVE_TO_OUTPUT, "MoveToOutput") \
	X(FIT_TO_OUTPUT, "FitToOutput") \
	X(IF, "If") \
	X(FOR_EACH, "ForEach") \
	X(VIRTUAL_OUTPUT_ADD, "VirtualOutputAdd") \
	X(VIRTUAL_OUTPUT_REMOVE, "VirtualOutputRemove") \
	X(AUTO_PLACE, "AutoPlace") \
	X(TOGGLE_TEARING, "ToggleTearing") \
	X(SHADE, "Shade") \
	X(UNSHADE, "Unshade") \
	X(TOGGLE_SHADE, "ToggleShade") \
	X(ENABLE_SCROLL_WHEEL_EMULATION, "EnableScrollWheelEmulation") \
	X(DISABLE_SCROLL_WHEEL_EMULATION, "DisableScrollWheelEmulation") \
	X(TOGGLE_SCROLL_WHEEL_EMULATION, "ToggleScrollWheelEmulation") \
	X(TOGGLE_SHOW_DESKTOP, "ToggleShowDesktop") \
	X(WARP_CURSOR, "WarpCursor") \
	X(HIDE_CURSOR, "HideCursor") \
	X(DEBUG_TOGGLE_KEY_STATE_INDICATOR, "DebugToggleKeyStateIndicator")
enum action_type {
	ACTION_TYPE_INVALID = 0,
#define X(name, str) ACTION_TYPE_##name,
	ACTION_TYPE_LIST(X)
#undef X
};
static const char *LABWC_src_action_c_action_names[] = {
	[ACTION_TYPE_INVALID] = "INVALID",
#undef X
#define X(name, str)[ACTION_TYPE_##name] = (str),
	ACTION_TYPE_LIST(X)
#undef X
	NULL
};
#undef ACTION_TYPE_LIST
void
action_arg_add_str(struct action *action, const char *key, const char *value)
{
	assert(action);
	assert(key);
	assert(value && "Tried to add NULL action string argument");
	struct LABWC_src_action_c_action_arg_str *arg = znew(*arg);
	arg->base.type = LAB_ACTION_ARG_STR;
	arg->base.key = xstrdup(key);
	arg->value = xstrdup(value);
	wl_list_append(&action->args, &arg->base.link);
}
static void
LABWC_src_action_c_action_arg_add_bool(struct action *action, const char *key, bool value)
{
	assert(action);
	assert(key);
	struct LABWC_src_action_c_action_arg_bool *arg = znew(*arg);
	arg->base.type = LAB_ACTION_ARG_BOOL;
	arg->base.key = xstrdup(key);
	arg->value = value;
	wl_list_append(&action->args, &arg->base.link);
}
static void
LABWC_src_action_c_action_arg_add_int(struct action *action, const char *key, int value)
{
	assert(action);
	assert(key);
	struct LABWC_src_action_c_action_arg_int *arg = znew(*arg);
	arg->base.type = LAB_ACTION_ARG_INT;
	arg->base.key = xstrdup(key);
	arg->value = value;
	wl_list_append(&action->args, &arg->base.link);
}
static void
LABWC_src_action_c_action_arg_add_list(struct action *action, const char *key, enum LABWC_src_action_c_action_arg_type type)
{
	assert(action);
	assert(key);
	struct LABWC_src_action_c_action_arg_list *arg = znew(*arg);
	arg->base.type = type;
	arg->base.key = xstrdup(key);
	wl_list_init(&arg->value);
	wl_list_append(&action->args, &arg->base.link);
}
void
action_arg_add_querylist(struct action *action, const char *key)
{
	LABWC_src_action_c_action_arg_add_list(action, key, LAB_ACTION_ARG_QUERY_LIST);
}
void
action_arg_add_actionlist(struct action *action, const char *key)
{
	LABWC_src_action_c_action_arg_add_list(action, key, LAB_ACTION_ARG_ACTION_LIST);
}
static void *
LABWC_src_action_c_action_get_arg(struct action *action, const char *key, enum LABWC_src_action_c_action_arg_type type)
{
	assert(action);
	assert(key);
	struct action_arg *arg;
	wl_list_for_each(arg, &action->args, link) {
		if (!strcasecmp(key, arg->key) && arg->type == type) {
			return arg;
		}
	}
	return NULL;
}
const char *
action_get_str(struct action *action, const char *key, const char *default_value)
{
	struct LABWC_src_action_c_action_arg_str *arg = LABWC_src_action_c_action_get_arg(action, key, LAB_ACTION_ARG_STR);
	return arg ? arg->value : default_value;
}
static bool
LABWC_src_action_c_action_get_bool(struct action *action, const char *key, bool default_value)
{
	struct LABWC_src_action_c_action_arg_bool *arg = LABWC_src_action_c_action_get_arg(action, key, LAB_ACTION_ARG_BOOL);
	return arg ? arg->value : default_value;
}
static int
LABWC_src_action_c_action_get_int(struct action *action, const char *key, int default_value)
{
	struct LABWC_src_action_c_action_arg_int *arg = LABWC_src_action_c_action_get_arg(action, key, LAB_ACTION_ARG_INT);
	return arg ? arg->value : default_value;
}
struct wl_list *
action_get_querylist(struct action *action, const char *key)
{
	struct LABWC_src_action_c_action_arg_list *arg = LABWC_src_action_c_action_get_arg(action, key, LAB_ACTION_ARG_QUERY_LIST);
	return arg ? &arg->value : NULL;
}
struct wl_list *
action_get_actionlist(struct action *action, const char *key)
{
	struct LABWC_src_action_c_action_arg_list *arg = LABWC_src_action_c_action_get_arg(action, key, LAB_ACTION_ARG_ACTION_LIST);
	return arg ? &arg->value : NULL;
}
void
action_arg_from_xml_node(struct action *action, const char *nodename, const char *content)
{
	assert(action);
	char *argument = xstrdup(nodename);
	string_truncate_at_pattern(argument, ".action");
	switch (action->type) {
	case ACTION_TYPE_EXECUTE:
		if (!strcmp(argument, "command") || !strcmp(argument, "execute")) {
			action_arg_add_str(action, "command", content);
			goto cleanup;
		}
		break;
	case ACTION_TYPE_MOVE_TO_EDGE:
	case ACTION_TYPE_TOGGLE_SNAP_TO_EDGE:
	case ACTION_TYPE_SNAP_TO_EDGE:
	case ACTION_TYPE_GROW_TO_EDGE:
	case ACTION_TYPE_SHRINK_TO_EDGE:
		if (!strcmp(argument, "direction")) {
			bool tiled = (action->type == ACTION_TYPE_TOGGLE_SNAP_TO_EDGE
					|| action->type == ACTION_TYPE_SNAP_TO_EDGE);
			enum lab_edge edge = lab_edge_parse(content, tiled,   false);
			if (edge == LAB_EDGE_NONE) {
			} else {
				LABWC_src_action_c_action_arg_add_int(action, argument, edge);
			}
			goto cleanup;
		}
		if (action->type == ACTION_TYPE_MOVE_TO_EDGE
				&& !strcasecmp(argument, "snapWindows")) {
			LABWC_src_action_c_action_arg_add_bool(action, argument, parse_bool(content, true));
			goto cleanup;
		}
		if ((action->type == ACTION_TYPE_SNAP_TO_EDGE
					|| action->type == ACTION_TYPE_TOGGLE_SNAP_TO_EDGE)
				&& !strcasecmp(argument, "combine")) {
			LABWC_src_action_c_action_arg_add_bool(action, argument, parse_bool(content, false));
			goto cleanup;
		}
		break;
	case ACTION_TYPE_NEXT_WINDOW:
	case ACTION_TYPE_PREVIOUS_WINDOW:
	case ACTION_TYPE_NEXT_WINDOW_IMMEDIATE:
	case ACTION_TYPE_PREVIOUS_WINDOW_IMMEDIATE:
		if (!strcasecmp(argument, "workspace")) {
			if (!strcasecmp(content, "all")) {
				LABWC_src_action_c_action_arg_add_int(action, argument, 0);
			} else if (!strcasecmp(content, "current")) {
				LABWC_src_action_c_action_arg_add_int(action, argument, 0);
			} else {
			}
			goto cleanup;
		}
		if (!strcasecmp(argument, "output")) {
			if (!strcasecmp(content, "all")) {
				LABWC_src_action_c_action_arg_add_int(action, argument, 0);
			} else if (!strcasecmp(content, "cursor")) {
				LABWC_src_action_c_action_arg_add_int(action, argument, 0);
			} else if (!strcasecmp(content, "focused")) {
				LABWC_src_action_c_action_arg_add_int(action, argument, 0);
			} else {
			}
			goto cleanup;
		}
		if (!strcasecmp(argument, "identifier")) {
			if (!strcasecmp(content, "all")) {
				LABWC_src_action_c_action_arg_add_int(action, argument, 0);
			} else if (!strcasecmp(content, "current")) {
				LABWC_src_action_c_action_arg_add_int(action, argument, 0);
			} else {
			}
			goto cleanup;
		}
		break;
	case ACTION_TYPE_SHOW_MENU:
		if (!strcmp(argument, "menu")) {
			action_arg_add_str(action, argument, content);
			goto cleanup;
		}
		if (!strcasecmp(argument, "atCursor")) {
			LABWC_src_action_c_action_arg_add_bool(action, argument, parse_bool(content, true));
			goto cleanup;
		}
		if (!strcasecmp(argument, "x.position")) {
			action_arg_add_str(action, argument, content);
			goto cleanup;
		}
		if (!strcasecmp(argument, "y.position")) {
			action_arg_add_str(action, argument, content);
			goto cleanup;
		}
		break;
	case ACTION_TYPE_TOGGLE_MAXIMIZE:
	case ACTION_TYPE_MAXIMIZE:
	case ACTION_TYPE_UNMAXIMIZE:
		if (!strcmp(argument, "direction")) {
			enum view_axis axis = view_axis_parse(content);
			if (axis == VIEW_AXIS_NONE || axis == VIEW_AXIS_INVALID) {
			} else {
				LABWC_src_action_c_action_arg_add_int(action, argument, axis);
			}
			goto cleanup;
		}
		break;
	case ACTION_TYPE_SET_DECORATIONS:
		if (!strcmp(argument, "decorations")) {
			enum lab_ssd_mode mode = ssd_mode_parse(content);
			if (mode != LAB_SSD_MODE_INVALID) {
				LABWC_src_action_c_action_arg_add_int(action, argument, mode);
			} else {
			}
			goto cleanup;
		}
		if (!strcasecmp(argument, "forceSSD")) {
			LABWC_src_action_c_action_arg_add_bool(action, argument, parse_bool(content, false));
			goto cleanup;
		}
		break;
	case ACTION_TYPE_RESIZE:
		if (!strcmp(argument, "direction")) {
			enum lab_edge edge = lab_edge_parse(content,
				  true,   false);
			if (edge == LAB_EDGE_NONE || edge == LAB_EDGE_CENTER) {
			} else {
				LABWC_src_action_c_action_arg_add_int(action, argument, edge);
			}
			goto cleanup;
		}
		break;
	case ACTION_TYPE_RESIZE_RELATIVE:
		if (!strcmp(argument, "left") || !strcmp(argument, "right") ||
				!strcmp(argument, "top") || !strcmp(argument, "bottom")) {
			LABWC_src_action_c_action_arg_add_int(action, argument, atoi(content));
			goto cleanup;
		}
		break;
	case ACTION_TYPE_MOVETO:
	case ACTION_TYPE_MOVE_RELATIVE:
		if (!strcmp(argument, "x") || !strcmp(argument, "y")) {
			LABWC_src_action_c_action_arg_add_int(action, argument, atoi(content));
			goto cleanup;
		}
		break;
	case ACTION_TYPE_RESIZETO:
		if (!strcmp(argument, "width") || !strcmp(argument, "height")) {
			LABWC_src_action_c_action_arg_add_int(action, argument, atoi(content));
			goto cleanup;
		}
		break;
	case ACTION_TYPE_SEND_TO_DESKTOP:
		if (!strcmp(argument, "follow")) {
			LABWC_src_action_c_action_arg_add_bool(action, argument, parse_bool(content, true));
			goto cleanup;
		}
	case ACTION_TYPE_GO_TO_DESKTOP:
		if (!strcmp(argument, "to")) {
			action_arg_add_str(action, argument, content);
			goto cleanup;
		}
		if (!strcmp(argument, "wrap")) {
			LABWC_src_action_c_action_arg_add_bool(action, argument, parse_bool(content, true));
			goto cleanup;
		}
		if (!strcmp(argument, "toggle")) {
			LABWC_src_action_c_action_arg_add_bool(
				action, argument, parse_bool(content, false));
			goto cleanup;
		}
		break;
	case ACTION_TYPE_TOGGLE_SNAP_TO_REGION:
	case ACTION_TYPE_SNAP_TO_REGION:
		if (!strcmp(argument, "region")) {
			action_arg_add_str(action, argument, content);
			goto cleanup;
		}
		break;
	case ACTION_TYPE_FOCUS_OUTPUT:
	case ACTION_TYPE_MOVE_TO_OUTPUT:
		if (!strcmp(argument, "output")) {
			action_arg_add_str(action, argument, content);
			goto cleanup;
		}
		if (!strcmp(argument, "direction")) {
			enum lab_edge edge = lab_edge_parse(content,
				  false,   false);
			if (edge == LAB_EDGE_NONE) {
			} else {
				LABWC_src_action_c_action_arg_add_int(action, argument, edge);
			}
			goto cleanup;
		}
		if (!strcmp(argument, "wrap")) {
			LABWC_src_action_c_action_arg_add_bool(action, argument, parse_bool(content, false));
			goto cleanup;
		}
		break;
	case ACTION_TYPE_VIRTUAL_OUTPUT_ADD:
	case ACTION_TYPE_VIRTUAL_OUTPUT_REMOVE:
		if (!strcmp(argument, "output_name")) {
			action_arg_add_str(action, argument, content);
			goto cleanup;
		}
		break;
	case ACTION_TYPE_AUTO_PLACE:
		if (!strcmp(argument, "policy")) {
			enum lab_placement_policy policy =
				view_placement_parse(content);
			if (policy == LAB_PLACE_INVALID) {
			} else {
				LABWC_src_action_c_action_arg_add_int(action, argument, policy);
			}
			goto cleanup;
		}
		break;
	case ACTION_TYPE_WARP_CURSOR:
		if (!strcmp(argument, "to") || !strcmp(argument, "x") || !strcmp(argument, "y")) {
			action_arg_add_str(action, argument, content);
			goto cleanup;
		}
		break;
	}
cleanup:
	free(argument);
}
static enum action_type
LABWC_src_action_c_action_type_from_str(const char *action_name)
{
	for (size_t i = 1; LABWC_src_action_c_action_names[i]; i++) {
		if (!strcasecmp(action_name, LABWC_src_action_c_action_names[i])) {
			return i;
		}
	}
	return ACTION_TYPE_INVALID;
}
struct action *
action_create(const char *action_name)
{
	if (!action_name) {
		return NULL;
	}
	enum action_type action_type = LABWC_src_action_c_action_type_from_str(action_name);
	if (action_type == ACTION_TYPE_NONE) {
		return NULL;
	}
	struct action *action = znew(*action);
	action->type = action_type;
	wl_list_init(&action->args);
	return action;
}
bool
actions_contain_toggle_keybinds(struct wl_list *action_list)
{
	struct action *action;
	wl_list_for_each(action, action_list, link) {
		if (action->type == ACTION_TYPE_TOGGLE_KEYBINDS) {
			return true;
		}
	}
	return false;
}
static bool
LABWC_src_action_c_action_list_is_valid(struct wl_list *actions)
{
	assert(actions);
	struct action *action;
	wl_list_for_each(action, actions, link) {
		if (!action_is_valid(action)) {
			return false;
		}
	}
	return true;
}
static bool
LABWC_src_action_c_action_branches_are_valid(struct action *action)
{
	static const char * const branches[] = { "then", "else", "none" };
	for (size_t i = 0; i < ARRAY_SIZE(branches); i++) {
		struct wl_list *children =
			action_get_actionlist(action, branches[i]);
		if (children && !LABWC_src_action_c_action_list_is_valid(children)) {
			return false;
		}
	}
	return true;
}
bool
action_is_valid(struct action *action)
{
	const char *arg_name = NULL;
	enum LABWC_src_action_c_action_arg_type arg_type = LAB_ACTION_ARG_STR;
	switch (action->type) {
	case ACTION_TYPE_EXECUTE:
		arg_name = "command";
		break;
	case ACTION_TYPE_MOVE_TO_EDGE:
	case ACTION_TYPE_TOGGLE_SNAP_TO_EDGE:
	case ACTION_TYPE_SNAP_TO_EDGE:
	case ACTION_TYPE_GROW_TO_EDGE:
	case ACTION_TYPE_SHRINK_TO_EDGE:
		arg_name = "direction";
		arg_type = LAB_ACTION_ARG_INT;
		break;
	case ACTION_TYPE_SHOW_MENU:
		arg_name = "menu";
		break;
	case ACTION_TYPE_GO_TO_DESKTOP:
	case ACTION_TYPE_SEND_TO_DESKTOP:
		arg_name = "to";
		break;
	case ACTION_TYPE_TOGGLE_SNAP_TO_REGION:
	case ACTION_TYPE_SNAP_TO_REGION:
		arg_name = "region";
		break;
	case ACTION_TYPE_IF:
	case ACTION_TYPE_FOR_EACH:
		return LABWC_src_action_c_action_branches_are_valid(action);
	default:
		return true;
	}
	if (LABWC_src_action_c_action_get_arg(action, arg_name, arg_type)) {
		return true;
	}
	return false;
}
bool
action_is_show_menu(struct action *action)
{
	return action->type == ACTION_TYPE_SHOW_MENU;
}
void
action_free(struct action *action)
{
	struct action_arg *arg, *arg_tmp;
	wl_list_for_each_safe(arg, arg_tmp, &action->args, link) {
		wl_list_remove(&arg->link);
		zfree(arg->key);
		if (arg->type == LAB_ACTION_ARG_STR) {
			struct LABWC_src_action_c_action_arg_str *str_arg = (struct LABWC_src_action_c_action_arg_str *)arg;
			zfree(str_arg->value);
		} else if (arg->type == LAB_ACTION_ARG_ACTION_LIST) {
			struct LABWC_src_action_c_action_arg_list *list_arg = (struct LABWC_src_action_c_action_arg_list *)arg;
			action_list_free(&list_arg->value);
		} else if (arg->type == LAB_ACTION_ARG_QUERY_LIST) {
			struct LABWC_src_action_c_action_arg_list *list_arg = (struct LABWC_src_action_c_action_arg_list *)arg;
			struct view_query *elm, *next;
			wl_list_for_each_safe(elm, next, &list_arg->value, link) {
				view_query_free(elm);
			}
		}
		zfree(arg);
	}
	zfree(action);
}
void
action_list_free(struct wl_list *action_list)
{
	struct action *action, *action_tmp;
	wl_list_for_each_safe(action, action_tmp, action_list, link) {
		wl_list_remove(&action->link);
		action_free(action);
	}
}
static void
LABWC_src_action_c_show_menu(struct view *view, struct cursor_context *ctx,
		const char *menu_name, bool at_cursor,
		const char *pos_x, const char *pos_y)
{
	if (server.input_mode != LAB_INPUT_STATE_PASSTHROUGH
			&& server.input_mode != LAB_INPUT_STATE_MENU) {
		return;
	}
	struct menu *menu = menu_get_by_id(menu_name);
	if (!menu) {
		return;
	}
	int x = (int)server.seat.cursor->x;
	int y = (int)server.seat.cursor->y;
	bool is_client_menu = !strcasecmp(menu_name, "client-menu");
	if (is_client_menu && !view) {
		return;
	}
	if (!at_cursor && view) {
		struct wlr_box extent = ssd_max_extents(view);
		x = extent.x;
		y = view->current.y;
		if (is_client_menu && node_type_contains(
				LAB_NODE_BUTTON, ctx->type)) {
			assert(ctx->node);
			int lx, ly;
			wlr_scene_node_coords(ctx->node, &lx, &ly);
			x = MAX(x, lx - rc.theme->menu_border_width);
		}
	}
	if (pos_x && pos_y) {
		struct output *output = output_nearest_to(
			(int)server.seat.cursor->x, (int)server.seat.cursor->y);
		struct wlr_box usable = output_usable_area_in_layout_coords(output);
		if (!strcasecmp(pos_x, "center")) {
			x = (usable.width - menu->size.width) / 2;
		} else if (strchr(pos_x, '%')) {
			x = (usable.width * atoi(pos_x)) / 100;
		} else {
			if (pos_x[0] == '-') {
				int neg_x = (int)strtol(pos_x, NULL, 10);
				x = usable.width + neg_x;
			} else {
				x = atoi(pos_x);
			}
		}
		if (!strcasecmp(pos_y, "center")) {
			y = (usable.height / 2) - (menu->size.height / 2);
		} else if (strchr(pos_y, '%')) {
			y = (usable.height * atoi(pos_y)) / 100;
		} else {
			if (pos_y[0] == '-') {
				int neg_y = (int)strtol(pos_y, NULL, 10);
				y = usable.height + neg_y;
			} else {
				y = atoi(pos_y);
			}
		}
		x = MAX(x, 0);
		x = MIN(x, usable.width - 1);
		y = MAX(y, 0);
		y = MIN(y, usable.height - 1);
		x += usable.x;
		y += usable.y;
	}
	menu->triggered_by_view = view;
	menu_open(menu, x, y);
}
static struct view *
LABWC_src_action_c_view_for_action(struct view *activator, struct action *action, struct cursor_context *ctx)
{
	if (activator) {
		return activator;
	}
	switch (action->type) {
	case ACTION_TYPE_FOCUS:
	case ACTION_TYPE_MOVE:
	case ACTION_TYPE_RESIZE: {
		*ctx = get_cursor_context();
		return ctx->view;
	}
	default:
		return server.active_view;
	}
}
static bool
LABWC_src_action_c_match_queries(struct view *view, struct action *action)
{
	assert(view);
	struct wl_list *queries = action_get_querylist(action, "query");
	if (!queries) {
		return true;
	}
	struct view_query *query;
	wl_list_for_each(query, queries, link) {
		if (view_matches_query(view, query)) {
			return true;
		}
	}
	return false;
}
static struct output *
LABWC_src_action_c_get_target_output(struct output *output, struct action *action)
{
	const char *output_name = action_get_str(action, "output", NULL);
	struct output *target = NULL;
	if (output_name) {
		target = output_from_name(output_name);
	} else {
		enum lab_edge edge =
			LABWC_src_action_c_action_get_int(action, "direction", LAB_EDGE_NONE);
		bool wrap = LABWC_src_action_c_action_get_bool(action, "wrap", false);
		target = output_get_adjacent(output, edge, wrap);
	}
	if (!target) {
	}
	return target;
}
static void
LABWC_src_action_c_warp_cursor(struct view *view, const char *to, const char *x, const char *y)
{
	struct output *output = output_nearest_to_cursor();
	struct wlr_box target_area = {0};
	int goto_x;
	int goto_y;
	if (!strcasecmp(to, "output") && output) {
		target_area = output_usable_area_in_layout_coords(output);
	} else if (!strcasecmp(to, "window") && view) {
		target_area = view->current;
	} else {
	}
	if (!strcasecmp(x, "center")) {
		goto_x = target_area.x + target_area.width / 2;
	} else {
		int offset_x = atoi(x);
		goto_x = offset_x >= 0 ?
			target_area.x + offset_x :
			target_area.x + target_area.width + offset_x;
	}
	if (!strcasecmp(y, "center")) {
		goto_y = target_area.y + target_area.height / 2;
	} else {
		int offset_y = atoi(y);
		goto_y = offset_y >= 0 ?
			target_area.y + offset_y :
			target_area.y + target_area.height + offset_y;
	}
	wlr_cursor_warp(server.seat.cursor, NULL, goto_x, goto_y);
	cursor_update_focus();
}
static void
LABWC_src_action_c_run_action(struct view *view, struct action *action,
	struct cursor_context *ctx)
{
	switch (action->type) {
	case ACTION_TYPE_CLOSE:
		if (view) {
			view_close(view);
		}
		break;
	case ACTION_TYPE_KILL:
		if (view) {
			assert(view->impl->get_pid);
			pid_t pid = view->impl->get_pid(view);
			if (pid == getpid()) {
			} else if (pid > 0) {
				kill(pid, SIGTERM);
			}
		}
		break;
	case ACTION_TYPE_EXECUTE: {
		struct buf cmd = BUF_INIT;
		buf_add(&cmd, action_get_str(action, "command", NULL));
		buf_expand_tilde(&cmd);
		spawn_async_no_shell(cmd.data);
		buf_reset(&cmd);
		break;
	}
	case ACTION_TYPE_EXIT:
		wl_display_terminate(server.wl_display);
		break;
	case ACTION_TYPE_MOVE_TO_EDGE:
		if (view) {
			enum lab_edge edge = LABWC_src_action_c_action_get_int(action, "direction", 0);
			bool snap_to_windows = LABWC_src_action_c_action_get_bool(action, "snapWindows", true);
			view_move_to_edge(view, edge, snap_to_windows);
		}
		break;
	case ACTION_TYPE_TOGGLE_SNAP_TO_EDGE:
	case ACTION_TYPE_SNAP_TO_EDGE:
		if (view) {
			enum lab_edge edge = LABWC_src_action_c_action_get_int(action, "direction", 0);
			if (action->type == ACTION_TYPE_TOGGLE_SNAP_TO_EDGE
					&& view->maximized == VIEW_AXIS_NONE
					&& !view->fullscreen
					&& view_is_tiled(view)
					&& view->tiled == edge) {
				view_set_untiled(view);
				view_apply_natural_geometry(view);
				break;
			}
			bool combine = LABWC_src_action_c_action_get_bool(action, "combine", false);
			view_snap_to_edge(view, edge,   true,
				combine);
		}
		break;
	case ACTION_TYPE_GROW_TO_EDGE:
		if (view) {
			enum lab_edge edge = LABWC_src_action_c_action_get_int(action, "direction", 0);
			view_grow_to_edge(view, edge);
		}
		break;
	case ACTION_TYPE_SHRINK_TO_EDGE:
		if (view) {
			enum lab_edge edge = LABWC_src_action_c_action_get_int(action, "direction", 0);
			view_shrink_to_edge(view, edge);
		}
		break;
	case ACTION_TYPE_NEXT_WINDOW:
	case ACTION_TYPE_PREVIOUS_WINDOW: {
			bool forward = action->type == ACTION_TYPE_NEXT_WINDOW;
		enum lab_view_criteria criteria = LAB_VIEW_CRITERIA_NO_SKIP_WINDOW_SWITCHER
			| LAB_VIEW_CRITERIA_NO_DIALOG;
		struct wl_list *head = &server.views;
		struct wl_list *from = forward && server.active_view
			? &server.active_view->link : head;
		for (struct wl_list *elm = (forward ? from->next : from->prev);
				elm != head; elm = (forward ? elm->next : elm->prev)) {
			struct view *v = wl_container_of(elm, view, link);
			if (!view_matches_criteria(v, criteria)) {
				continue;
			}
			if (server.active_view && forward) {
				view_move_to_back(server.active_view);
			}
			desktop_focus_view(v, true);
			break;
		}
		cursor_update_focus();
		break;
	}
	case ACTION_TYPE_NEXT_WINDOW_IMMEDIATE:
	case ACTION_TYPE_PREVIOUS_WINDOW_IMMEDIATE: {
		bool forward = action->type == ACTION_TYPE_NEXT_WINDOW_IMMEDIATE;
		enum lab_view_criteria criteria = LAB_VIEW_CRITERIA_NO_SKIP_WINDOW_SWITCHER
			| LAB_VIEW_CRITERIA_NO_DIALOG;
		struct wl_list *head = &server.views;
		struct wl_list *from = forward && server.active_view
			? &server.active_view->link : head;
		for (struct wl_list *elm = (forward ? from->next : from->prev);
				elm != head; elm = (forward ? elm->next : elm->prev)) {
			struct view *v = wl_container_of(elm, view, link);
			if (!view_matches_criteria(v, criteria)) {
				continue;
			}
			if (server.active_view && forward) {
				view_move_to_back(server.active_view);
			}
			desktop_focus_view(v, true);
			break;
		}
		cursor_update_focus();
		break;
	}
	case ACTION_TYPE_SHOW_MENU:
		LABWC_src_action_c_show_menu(view, ctx,
			action_get_str(action, "menu", NULL),
			LABWC_src_action_c_action_get_bool(action, "atCursor", true),
			action_get_str(action, "x.position", NULL),
			action_get_str(action, "y.position", NULL));
		break;
	case ACTION_TYPE_TOGGLE_MAXIMIZE:
		if (view) {
			enum view_axis axis = LABWC_src_action_c_action_get_int(action,
				"direction", VIEW_AXIS_BOTH);
			view_toggle_maximize(view, axis);
		}
		break;
	case ACTION_TYPE_MAXIMIZE:
		if (view) {
			enum view_axis axis = LABWC_src_action_c_action_get_int(action,
				"direction", VIEW_AXIS_BOTH);
			view_maximize(view, axis);
		}
		break;
	case ACTION_TYPE_UNMAXIMIZE:
		if (view) {
			enum view_axis axis = LABWC_src_action_c_action_get_int(action,
				"direction", VIEW_AXIS_BOTH);
			view_maximize(view, view->maximized & ~axis);
		}
		break;
	case ACTION_TYPE_TOGGLE_FULLSCREEN:
		if (view) {
			view_toggle_fullscreen(view);
		}
		break;
	case ACTION_TYPE_SET_DECORATIONS:
		if (view) {
			enum lab_ssd_mode mode = LABWC_src_action_c_action_get_int(action,
				"decorations", LAB_SSD_MODE_FULL);
			bool force_ssd = LABWC_src_action_c_action_get_bool(action,
				"forceSSD", false);
			view_set_decorations(view, mode, force_ssd);
		}
		break;
	case ACTION_TYPE_TOGGLE_DECORATIONS:
		if (view) {
			view_toggle_decorations(view);
		}
		break;
	case ACTION_TYPE_TOGGLE_ALWAYS_ON_TOP:
		if (view) {
			view_toggle_always_on_top(view);
		}
		break;
	case ACTION_TYPE_TOGGLE_ALWAYS_ON_BOTTOM:
		if (view) {
			view_toggle_always_on_bottom(view);
		}
		break;
	case ACTION_TYPE_TOGGLE_OMNIPRESENT:
		if (view) {
			view_toggle_visible_on_all_workspaces(view);
		}
		break;
	case ACTION_TYPE_FOCUS:
		if (view) {
			desktop_focus_view(view,   false);
		}
		break;
	case ACTION_TYPE_UNFOCUS:
		seat_focus_surface(&server.seat, NULL);
		break;
	case ACTION_TYPE_ICONIFY:
		if (view) {
			view_minimize(view, true);
		}
		break;
	case ACTION_TYPE_RAISE:
		if (view) {
			view_move_to_front(view);
		}
		break;
	case ACTION_TYPE_LOWER:
		if (view) {
			view_move_to_back(view);
		}
		break;
	case ACTION_TYPE_MOVE:
		if (view) {
			if (view != server.seat.pressed.ctx.view) {
				interactive_set_grab_context(ctx);
			}
			interactive_begin(view, LAB_INPUT_STATE_MOVE,
				LAB_EDGE_NONE);
		}
		break;
	case ACTION_TYPE_RESIZE:
		if (view) {
			enum lab_edge resize_edges =
				LABWC_src_action_c_action_get_int(action, "direction", LAB_EDGE_NONE);
			if (view != server.seat.pressed.ctx.view) {
				interactive_set_grab_context(ctx);
			}
			interactive_begin(view, LAB_INPUT_STATE_RESIZE,
				resize_edges);
		}
		break;
	case ACTION_TYPE_RESIZE_RELATIVE:
		if (view) {
			int left = LABWC_src_action_c_action_get_int(action, "left", 0);
			int right = LABWC_src_action_c_action_get_int(action, "right", 0);
			int top = LABWC_src_action_c_action_get_int(action, "top", 0);
			int bottom = LABWC_src_action_c_action_get_int(action, "bottom", 0);
			view_resize_relative(view, left, right, top, bottom);
		}
		break;
	case ACTION_TYPE_MOVETO:
		if (view) {
			int x = LABWC_src_action_c_action_get_int(action, "x", 0);
			int y = LABWC_src_action_c_action_get_int(action, "y", 0);
			struct border margin = ssd_thickness(view);
			view_move(view, x + margin.left, y + margin.top);
		}
		break;
	case ACTION_TYPE_RESIZETO:
		if (view) {
			int width = LABWC_src_action_c_action_get_int(action, "width", 0);
			int height = LABWC_src_action_c_action_get_int(action, "height", 0);
			struct wlr_box box = {
				.x = view->pending.x,
				.y = view->pending.y,
				.width = width ? : view->pending.width,
				.height = height ? : view->pending.height,
			};
			view_set_shade(view, false);
			view_move_resize(view, box);
		}
		break;
	case ACTION_TYPE_MOVE_RELATIVE:
		if (view) {
			int x = LABWC_src_action_c_action_get_int(action, "x", 0);
			int y = LABWC_src_action_c_action_get_int(action, "y", 0);
			view_move_relative(view, x, y);
		}
		break;
	case ACTION_TYPE_MOVETO_CURSOR:
		if (view) {
			view_place_by_policy(view,   true,
				LAB_PLACE_CURSOR);
		}
		break;
	case ACTION_TYPE_SEND_TO_DESKTOP:
		if (!view) {
			break;
		}
	case ACTION_TYPE_GO_TO_DESKTOP: {
		bool follow = true;
		bool wrap = LABWC_src_action_c_action_get_bool(action, "wrap", true);
		const char *to = action_get_str(action, "to", NULL);
		struct workspace *target_workspace = workspaces_find(
			server.workspaces.current, to, wrap);
		if (action->type == ACTION_TYPE_GO_TO_DESKTOP) {
			bool toggle = LABWC_src_action_c_action_get_bool(action, "toggle", false);
			if (target_workspace == server.workspaces.current
				&& toggle) {
				target_workspace = server.workspaces.last;
			}
		}
		if (!target_workspace) {
			break;
		}
		if (action->type == ACTION_TYPE_SEND_TO_DESKTOP) {
			view_move_to_workspace(view, target_workspace);
			follow = LABWC_src_action_c_action_get_bool(action, "follow", true);
			if (!follow && server.active_view == view) {
				desktop_focus_topmost_view();
			}
		}
		if (follow) {
			workspaces_switch_to(target_workspace,
				  true);
		}
		break;
	}
	case ACTION_TYPE_MOVE_TO_OUTPUT: {
		if (!view) {
			break;
		}
		struct output *target_output =
			LABWC_src_action_c_get_target_output(view->output, action);
		if (target_output) {
			view_move_to_output(view, target_output);
		}
		break;
	}
	case ACTION_TYPE_FIT_TO_OUTPUT:
		if (!view) {
			break;
		}
		view_constrain_size_to_that_of_usable_area(view);
		break;
	case ACTION_TYPE_TOGGLE_SNAP_TO_REGION:
	case ACTION_TYPE_SNAP_TO_REGION: {
		if (!view) {
			break;
		}
		struct output *output = view->output;
		if (!output_is_usable(output)) {
			break;
		}
		const char *region_name = action_get_str(action, "region", NULL);
		struct region *region = regions_from_name(region_name, output);
		if (region) {
			if (action->type == ACTION_TYPE_TOGGLE_SNAP_TO_REGION
					&& view->maximized == VIEW_AXIS_NONE
					&& !view->fullscreen
					&& view_is_tiled(view)
					&& view->tiled_region == region) {
				view_set_untiled(view);
				view_apply_natural_geometry(view);
				break;
			}
			view_snap_to_region(view, region);
		} else {
		}
		break;
	}
	case ACTION_TYPE_UNSNAP:
		if (view && !view->fullscreen && !view_is_floating(view)) {
			view_maximize(view, VIEW_AXIS_NONE);
			view_set_untiled(view);
			view_apply_natural_geometry(view);
		}
		break;
	case ACTION_TYPE_TOGGLE_KEYBINDS:
		if (view) {
			view_toggle_keybinds(view);
		}
		break;
	case ACTION_TYPE_FOCUS_OUTPUT: {
		struct output *output = output_nearest_to_cursor();
		struct output *target_output =
			LABWC_src_action_c_get_target_output(output, action);
		if (target_output) {
			desktop_focus_output(target_output);
		}
		break;
	}
	case ACTION_TYPE_IF: {
		if (view) {
			struct wl_list *actions;
			if (LABWC_src_action_c_match_queries(view, action)) {
				actions = action_get_actionlist(action, "then");
			} else {
				actions = action_get_actionlist(action, "else");
			}
			if (actions) {
				actions_run(view, actions, ctx);
			}
		}
		break;
	}
	case ACTION_TYPE_FOR_EACH: {
		struct wl_list *actions = NULL;
		bool matches = false;
		struct wl_array views;
		wl_array_init(&views);
		view_array_append(&views, LAB_VIEW_CRITERIA_NONE);
		struct view **item;
		wl_array_for_each(item, &views) {
			if (LABWC_src_action_c_match_queries(*item, action)) {
				matches = true;
				actions = action_get_actionlist(action, "then");
			} else {
				actions = action_get_actionlist(action, "else");
			}
			if (actions) {
				actions_run(*item, actions, ctx);
			}
		}
		wl_array_release(&views);
		if (!matches) {
			actions = action_get_actionlist(action, "none");
			if (actions) {
				actions_run(view, actions, NULL);
			}
		}
		break;
	}
	case ACTION_TYPE_VIRTUAL_OUTPUT_ADD: {
		const char *output_name =
			action_get_str(action, "output_name", NULL);
		output_virtual_add(output_name,
				  NULL);
		break;
	}
	case ACTION_TYPE_VIRTUAL_OUTPUT_REMOVE: {
		const char *output_name =
			action_get_str(action, "output_name", NULL);
		output_virtual_remove(output_name);
		break;
	}
	case ACTION_TYPE_AUTO_PLACE:
		if (view) {
			enum lab_placement_policy policy =
				LABWC_src_action_c_action_get_int(action, "policy", LAB_PLACE_AUTOMATIC);
			view_place_by_policy(view,
				  true, policy);
		}
		break;
	case ACTION_TYPE_TOGGLE_TEARING:
		if (view) {
			switch (view->force_tearing) {
			case LAB_STATE_UNSPECIFIED:
				view->force_tearing =
					output_get_tearing_allowance(view->output)
						? LAB_STATE_DISABLED : LAB_STATE_ENABLED;
				break;
			case LAB_STATE_DISABLED:
				view->force_tearing = LAB_STATE_ENABLED;
				break;
			case LAB_STATE_ENABLED:
				view->force_tearing = LAB_STATE_DISABLED;
				break;
			}
		}
		break;
	case ACTION_TYPE_TOGGLE_SHADE:
		if (view) {
			view_set_shade(view, !view->shaded);
		}
		break;
	case ACTION_TYPE_SHADE:
		if (view) {
			view_set_shade(view, true);
		}
		break;
	case ACTION_TYPE_UNSHADE:
		if (view) {
			view_set_shade(view, false);
		}
		break;
	case ACTION_TYPE_ENABLE_SCROLL_WHEEL_EMULATION:
		server.seat.cursor_scroll_wheel_emulation = true;
		break;
	case ACTION_TYPE_DISABLE_SCROLL_WHEEL_EMULATION:
		server.seat.cursor_scroll_wheel_emulation = false;
		break;
	case ACTION_TYPE_TOGGLE_SCROLL_WHEEL_EMULATION:
		server.seat.cursor_scroll_wheel_emulation =
			!server.seat.cursor_scroll_wheel_emulation;
		break;
	case ACTION_TYPE_TOGGLE_SHOW_DESKTOP:
		show_desktop_toggle();
		break;
	case ACTION_TYPE_WARP_CURSOR: {
		const char *to = action_get_str(action, "to", "output");
		const char *x = action_get_str(action, "x", "center");
		const char *y = action_get_str(action, "y", "center");
		LABWC_src_action_c_warp_cursor(view, to, x, y);
		break;
	}
	case ACTION_TYPE_HIDE_CURSOR:
		cursor_set_visible(&server.seat, false);
		break;
	case ACTION_TYPE_DEBUG_TOGGLE_KEY_STATE_INDICATOR:
		key_state_indicator_toggle();
		break;
	case ACTION_TYPE_INVALID:
		break;
	default:
	}
}
void
actions_run(struct view *activator, struct wl_list *actions, struct cursor_context *cursor_ctx)
{
	if (!actions) {
		return;
	}
	keyboard_reset_current_keybind();
	struct cursor_context ctx = {0};
	if (cursor_ctx) {
		ctx = *cursor_ctx;
	}
	struct action *action;
	wl_list_for_each(action, actions, link) {
		if (false) {
			continue;
		}
		struct view *view = LABWC_src_action_c_view_for_action(activator, action, &ctx);
		LABWC_src_action_c_run_action(view, action, &ctx);
	}
}
#undef ACTION_TYPE_LIST
#undef X
#include <assert.h>
#include <stdlib.h>
#include <drm_fourcc.h>
static struct lab_data_buffer *LABWC_src_buffer_c_data_buffer_from_buffer(
	struct wlr_buffer *buffer);
static void
LABWC_src_buffer_c_data_buffer_destroy(struct wlr_buffer *wlr_buffer)
{
	struct lab_data_buffer *buffer = LABWC_src_buffer_c_data_buffer_from_buffer(wlr_buffer);
	cairo_surface_destroy(buffer->surface);
	if (!buffer->surface_owns_data) {
		free(buffer->data);
	}
	wlr_buffer_finish(wlr_buffer);
	free(buffer);
}
static bool
LABWC_src_buffer_c_data_buffer_begin_data_ptr_access(struct wlr_buffer *wlr_buffer, uint32_t flags,
		void **data, uint32_t *format, size_t *stride)
{
	struct lab_data_buffer *buffer =
		wl_container_of(wlr_buffer, buffer, base);
	assert(buffer->data);
	*data = (void *)buffer->data;
	*format = buffer->format;
	*stride = buffer->stride;
	return true;
}
static void
LABWC_src_buffer_c_data_buffer_end_data_ptr_access(struct wlr_buffer *wlr_buffer)
{
}
static const struct wlr_buffer_impl LABWC_src_buffer_c_data_buffer_impl = {
	.destroy = LABWC_src_buffer_c_data_buffer_destroy,
	.begin_data_ptr_access = LABWC_src_buffer_c_data_buffer_begin_data_ptr_access,
	.end_data_ptr_access = LABWC_src_buffer_c_data_buffer_end_data_ptr_access,
};
static struct lab_data_buffer *
LABWC_src_buffer_c_data_buffer_from_buffer(struct wlr_buffer *buffer)
{
	assert(buffer->impl == &LABWC_src_buffer_c_data_buffer_impl);
	return (struct lab_data_buffer *)buffer;
}
struct lab_data_buffer *
buffer_adopt_cairo_surface(cairo_surface_t *surface)
{
	assert(cairo_surface_get_type(surface) == CAIRO_SURFACE_TYPE_IMAGE);
	assert(cairo_image_surface_get_format(surface) == CAIRO_FORMAT_ARGB32);
	int width = cairo_image_surface_get_width(surface);
	int height = cairo_image_surface_get_height(surface);
	struct lab_data_buffer *buffer = znew(*buffer);
	wlr_buffer_init(&buffer->base, &LABWC_src_buffer_c_data_buffer_impl, width, height);
	buffer->surface = surface;
	buffer->data = cairo_image_surface_get_data(buffer->surface);
	assert(buffer->data);
	buffer->format = DRM_FORMAT_ARGB8888;
	buffer->stride = (size_t)cairo_image_surface_get_stride(buffer->surface);
	buffer->logical_width = (uint32_t)width;
	buffer->logical_height = (uint32_t)height;
	buffer->surface_owns_data = true;
	return buffer;
}
struct lab_data_buffer *
buffer_create_cairo(uint32_t logical_width, uint32_t logical_height, float scale)
{
	if (!logical_width || !logical_height) {
		return NULL;
	}
	cairo_surface_t *surface =
		cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
			(int)lroundf((float)logical_width * scale),
			(int)lroundf((float)logical_height * scale));
	cairo_surface_set_device_scale(surface, scale, scale);
	struct lab_data_buffer *buffer = buffer_adopt_cairo_surface(surface);
	buffer->logical_width = logical_width;
	buffer->logical_height = logical_height;
	return buffer;
}
struct lab_data_buffer *
buffer_create_from_data(void *pixel_data, uint32_t width, uint32_t height,
		uint32_t stride)
{
	struct lab_data_buffer *buffer = znew(*buffer);
	wlr_buffer_init(&buffer->base, &LABWC_src_buffer_c_data_buffer_impl,
		(int)width, (int)height);
	buffer->logical_width = width;
	buffer->logical_height = height;
	buffer->data = pixel_data;
	assert(buffer->data);
	buffer->format = DRM_FORMAT_ARGB8888;
	buffer->stride = stride;
	buffer->surface = cairo_image_surface_create_for_data(
		pixel_data, CAIRO_FORMAT_ARGB32, (int)width, (int)height,
		(int)stride);
	buffer->surface_owns_data = false;
	return buffer;
}
struct lab_data_buffer *
buffer_resize(struct lab_data_buffer *src_buffer, int width, int height,
		double scale)
{
	assert(src_buffer);
	cairo_surface_t *surface = src_buffer->surface;
	int src_w = cairo_image_surface_get_width(surface);
	int src_h = cairo_image_surface_get_height(surface);
	struct lab_data_buffer *buffer =
		buffer_create_cairo((uint32_t)width, (uint32_t)height, (float)scale);
	if (!buffer) {
		return NULL;
	}
	cairo_t *cairo = cairo_create(buffer->surface);
	struct wlr_box container = {
		.width = width,
		.height = height,
	};
	struct wlr_box dst_box = box_fit_within(src_w, src_h, &container);
	double scene_scale = (double)dst_box.width / (double)src_w;
	cairo_translate(cairo, dst_box.x, dst_box.y);
	cairo_scale(cairo, scene_scale, scene_scale);
	cairo_set_source_surface(cairo, surface, 0, 0);
	cairo_pattern_set_filter(cairo_get_source(cairo), CAIRO_FILTER_GOOD);
	cairo_set_operator(cairo, CAIRO_OPERATOR_SOURCE);
	cairo_paint(cairo);
	cairo_surface_flush(buffer->surface);
	cairo_destroy(cairo);
	return buffer;
}
#include <assert.h>
#include <fcntl.h>
#include <glib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
static void
LABWC_src_common_spawn_c_reset_signals_and_limits(void)
{
	restore_nofile_limit();
	sigset_t set;
	sigemptyset(&set);
	sigprocmask(SIG_SETMASK, &set, NULL);
	signal(SIGPIPE, SIG_DFL);
}
static bool
LABWC_src_common_spawn_c_set_cloexec(int fd)
{
	int flags = fcntl(fd, F_GETFD);
	if (flags == -1) {
		return false;
	}
	flags = flags | FD_CLOEXEC;
	if (fcntl(fd, F_SETFD, flags) == -1) {
		return false;
	}
	return true;
}
void
spawn_async_no_shell(char const *command)
{
	GError *err = NULL;
	gchar **argv = NULL;
	assert(command);
	g_shell_parse_argv((gchar *)command, NULL, &argv, &err);
	if (err) {
		g_message("%s", err->message);
		g_error_free(err);
		return;
	}
	pid_t child = 0, grandchild = 0;
	child = fork();
	switch (child) {
	case -1:
		goto out;
	case 0:
		LABWC_src_common_spawn_c_reset_signals_and_limits();
		setsid();
		grandchild = fork();
		if (grandchild == 0) {
			execvp(argv[0], argv);
			_exit(0);
		} else if (grandchild < 0) {
		}
		_exit(0);
	default:
		break;
	}
	waitpid(child, NULL, 0);
out:
	g_strfreev(argv);
}
void
spawn_sync_no_shell(char const *command)
{
	GError *err = NULL;
	gchar **argv = NULL;
	assert(command);
	g_shell_parse_argv((gchar *)command, NULL, &argv, &err);
	if (err) {
		g_message("%s", err->message);
		g_error_free(err);
		return;
	}
	pid_t child = fork();
	switch (child) {
	case -1:
		break;
	case 0:
		LABWC_src_common_spawn_c_reset_signals_and_limits();
		execvp(argv[0], argv);
		_exit(1);
	default:
		waitpid(child, NULL, 0);
		break;
	}
	g_strfreev(argv);
}
pid_t
spawn_primary_client(const char *command)
{
	assert(command);
	GError *err = NULL;
	gchar **argv = NULL;
	g_shell_parse_argv((gchar *)command, NULL, &argv, &err);
	if (err) {
		g_message("%s", err->message);
		g_error_free(err);
		return -1;
	}
	pid_t child = fork();
	switch (child) {
	case -1:
		g_strfreev(argv);
		return -1;
	case 0:
		close(STDIN_FILENO);
		LABWC_src_common_spawn_c_reset_signals_and_limits();
		execvp(argv[0], argv);
		_exit(1);
	default:
		g_strfreev(argv);
		return child;
	}
}
pid_t
spawn_piped(const char *command, int *pipe_fd)
{
	assert(command);
	int pipe_rw[2];
	if (pipe(pipe_rw) != 0) {
		return -1;
	}
	pid_t pid = fork();
	if (pid < 0) {
		close(pipe_rw[0]);
		close(pipe_rw[1]);
		return pid;
	}
	if (pid == 0) {
		LABWC_src_common_spawn_c_reset_signals_and_limits();
		dup2(pipe_rw[1], STDOUT_FILENO);
		close(pipe_rw[0]);
		close(pipe_rw[1]);
		int dev_null = open("/dev/null", O_RDWR);
		if (dev_null < 0) {
			close(STDIN_FILENO);
			close(STDERR_FILENO);
		} else {
			dup2(dev_null, STDIN_FILENO);
			dup2(dev_null, STDERR_FILENO);
			close(dev_null);
		}
		execl("/bin/sh", "sh", "-c", command, NULL);
		_exit(1);
	}
	close(pipe_rw[1]);
	LABWC_src_common_spawn_c_set_cloexec(pipe_rw[0]);
	*pipe_fd = pipe_rw[0];
	return pid;
}
void
spawn_piped_close(pid_t pid, int pipe_fd)
{
	close(pipe_fd);
}
#include <assert.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
struct LABWC_src_common_dir_c_dir {
	const char *prefix;
	const char *default_prefix;
	const char *path;
};
static struct LABWC_src_common_dir_c_dir LABWC_src_common_dir_c_config_dirs[] = {
	{
		.prefix = "XDG_CONFIG_HOME",
		.default_prefix = "$HOME/.config",
		.path = "labwc"
	}, {
		.prefix = "XDG_CONFIG_DIRS",
		.default_prefix = "/etc/xdg",
		.path = "labwc",
	}, {
		.path = NULL,
	}
};
struct LABWC_src_common_dir_c_ctx {
	void (*build_path_fn)(struct LABWC_src_common_dir_c_ctx *ctx, char *prefix, const char *path);
	const char *filename;
	char *buf;
	size_t len;
	struct LABWC_src_common_dir_c_dir *dirs;
	struct wl_list *list;
};
static void
LABWC_src_common_dir_c_build_config_path(struct LABWC_src_common_dir_c_ctx *ctx, char *prefix, const char *path)
{
	assert(prefix);
	snprintf(ctx->buf, ctx->len, "%s/%s/%s", prefix, path, ctx->filename);
}
static void
LABWC_src_common_dir_c_find_dir(struct LABWC_src_common_dir_c_ctx *ctx)
{
	struct buf prefix = BUF_INIT;
	for (int i = 0; ctx->dirs[i].path; i++) {
		struct LABWC_src_common_dir_c_dir d = ctx->dirs[i];
		buf_clear(&prefix);
		char *pfxenv = getenv(d.prefix);
		buf_add(&prefix, pfxenv ? pfxenv : d.default_prefix);
		if (!prefix.len) {
			continue;
		}
		buf_expand_shell_variables(&prefix);
		gchar **prefixes;
		prefixes = g_strsplit(prefix.data, ":", -1);
		for (gchar **p = prefixes; *p; p++) {
			ctx->build_path_fn(ctx, *p, d.path);
			struct path *path = znew(*path);
			path->string = xstrdup(ctx->buf);
			wl_list_append(ctx->list, &path->link);
		}
		g_strfreev(prefixes);
	}
	buf_reset(&prefix);
}
void
paths_config_create(struct wl_list *paths, const char *filename)
{
	char buf[4096] = { 0 };
	wl_list_init(paths);
	if (rc.config_dir) {
		struct path *path = znew(*path);
		path->string = strdup_printf("%s/%s", rc.config_dir, filename);
		wl_list_append(paths, &path->link);
		return;
	}
	struct LABWC_src_common_dir_c_ctx ctx = {
		.build_path_fn = LABWC_src_common_dir_c_build_config_path,
		.filename = filename,
		.buf = buf,
		.len = sizeof(buf),
		.dirs = LABWC_src_common_dir_c_config_dirs,
		.list = paths,
	};
	LABWC_src_common_dir_c_find_dir(&ctx);
}
void
paths_destroy(struct wl_list *paths)
{
	struct path *path, *next;
	wl_list_for_each_safe(path, next, paths, link) {
		free(path->string);
		wl_list_remove(&path->link);
		free(path);
	}
}
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
enum LABWC_src_common_string_helpers_c_str_flags {
	STR_FLAG_NONE = 0,
	STR_FLAG_IGNORE_CASE,
};
bool
string_null_or_empty(const char *s)
{
	return !s || !*s;
}
static void
LABWC_src_common_string_helpers_c_rtrim(char *s)
{
	size_t len = strlen(s);
	if (!len) {
		return;
	}
	char *end = s + len - 1;
	while (end >= s && isspace(*end)) {
		end--;
	}
	*(end + 1) = '\0';
}
char *
string_strip(char *s)
{
	while (isspace(*s)) {
		s++;
	}
	LABWC_src_common_string_helpers_c_rtrim(s);
	return s;
}
void
string_truncate_at_pattern(char *buf, const char *pattern)
{
	char *p = strstr(buf, pattern);
	if (!p) {
		return;
	}
	*p = '\0';
}
char *
strdup_printf(const char *fmt, ...)
{
	assert(fmt);
	size_t size = 0;
	char *p = NULL;
	va_list ap;
	va_start(ap, fmt);
	int n = vsnprintf(p, size, fmt, ap);
	va_end(ap);
	if (n < 0) {
		return NULL;
	}
	size = (size_t)n + 1;
	p = xzalloc(size);
	va_start(ap, fmt);
	n = vsnprintf(p, size, fmt, ap);
	va_end(ap);
	if (n < 0) {
		free(p);
		return NULL;
	}
	return p;
}
char *
str_join(const char * const parts[], const char *fmt, const char *sep)
{
	assert(parts);
	if (!fmt) {
		fmt = "%s";
	}
	if (!sep) {
		sep = " ";
	}
	size_t size = 0;
	size_t n_parts = 0;
	size_t sep_len = strlen(sep);
	for (const char * const *s = parts; *s; ++s) {
		int n = snprintf(NULL, 0, fmt, *s);
		if (n < 0) {
			return NULL;
		}
		size += (size_t)n;
		++n_parts;
	}
	if (n_parts < 1) {
		return NULL;
	}
	size += (n_parts - 1) * sep_len + 1;
	char *buf = xzalloc(size);
	char *p = buf;
	for (const char * const *s = parts; *s; ++s) {
		int n = 0;
		if (p != buf) {
			n = snprintf(p, size, "%s", sep);
			if (n < 0 || (size_t)n >= size) {
				p = NULL;
				break;
			}
			size -= (size_t)n;
			p += (size_t)n;
		}
		n = snprintf(p, size, fmt, *s);
		if (n < 0 || (size_t)n >= size) {
			p = NULL;
			break;
		}
		size -= (size_t)n;
		p += (size_t)n;
	}
	if (!p) {
		free(buf);
		return NULL;
	}
	return buf;
}
static bool
LABWC_src_common_string_helpers_c__str_endswith(const char * const string, const char * const suffix, uint32_t flags)
{
	size_t len_str = string ? strlen(string) : 0;
	size_t len_sfx = suffix ? strlen(suffix) : 0;
	if (len_str < len_sfx) {
		return false;
	}
	if (len_sfx == 0) {
		return true;
	}
	if (flags & STR_FLAG_IGNORE_CASE) {
		return strcasecmp(string + len_str - len_sfx, suffix) == 0;
	} else {
		return strcmp(string + len_str - len_sfx, suffix) == 0;
	}
}
bool
str_endswith(const char * const string, const char * const suffix)
{
	return LABWC_src_common_string_helpers_c__str_endswith(string, suffix, STR_FLAG_NONE);
}
bool
str_endswith_ignore_case(const char * const string, const char * const suffix)
{
	return LABWC_src_common_string_helpers_c__str_endswith(string, suffix, STR_FLAG_IGNORE_CASE);
}
bool
str_starts_with(const char *s, char needle, const char *ignore_chars)
{
	return (s + strspn(s, ignore_chars))[0] == needle;
}
bool
str_equal(const char *a, const char *b)
{
	return a == b || (a && b && !strcmp(a, b));
}
bool
str_space_only(const char *s)
{
	for (; *s; s++) {
		if (!isspace(*s)) {
			return false;
		}
	}
	return true;
}
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
void
buf_expand_tilde(struct buf *s)
{
	struct buf tmp = BUF_INIT;
	for (int i = 0; i < s->len; i++) {
		if (s->data[i] == '~') {
			buf_add(&tmp, getenv("HOME"));
		} else {
			buf_add_char(&tmp, s->data[i]);
		}
	}
	buf_move(s, &tmp);
}
static void
LABWC_src_common_buf_c_strip_curly_braces(char *s)
{
	size_t len = strlen(s);
	if (s[0] != '{' || s[len - 1] != '}') {
		return;
	}
	len -= 2;
	memmove(s, s + 1, len);
	s[len] = 0;
}
static bool
LABWC_src_common_buf_c_isvalid(char p)
{
	return isalnum(p) || p == '_' || p == '{' || p == '}';
}
void
buf_expand_shell_variables(struct buf *s)
{
	struct buf tmp = BUF_INIT;
	struct buf environment_variable = BUF_INIT;
	for (int i = 0; i < s->len; i++) {
		if (s->data[i] == '$' && LABWC_src_common_buf_c_isvalid(s->data[i + 1])) {
			buf_clear(&environment_variable);
			int len = 0;
			while (LABWC_src_common_buf_c_isvalid(s->data[i + 1 + len])) {
				buf_add_char(&environment_variable, s->data[i + 1 + len]);
				++len;
			}
			i += len;
			LABWC_src_common_buf_c_strip_curly_braces(environment_variable.data);
			char *p = getenv(environment_variable.data);
			if (p) {
				buf_add(&tmp, p);
			}
		} else {
			buf_add_char(&tmp, s->data[i]);
		}
	}
	buf_reset(&environment_variable);
	buf_move(s, &tmp);
}
static void
LABWC_src_common_buf_c_buf_expand(struct buf *s, int new_alloc)
{
	if (s->alloc && new_alloc <= s->alloc) {
		return;
	}
	new_alloc = MAX(new_alloc, 256);
	new_alloc = MAX(new_alloc, s->alloc * 3 / 2);
	if (s->alloc) {
		assert(s->data);
		s->data = xrealloc(s->data, (size_t)new_alloc);
	} else {
		assert(!s->len);
		s->data = xmalloc(new_alloc);
		s->data[0] = '\0';
	}
	s->alloc = new_alloc;
}
void
buf_add_fmt(struct buf *s, const char *fmt, ...)
{
	if (string_null_or_empty(fmt)) {
		return;
	}
	va_list ap;
	va_start(ap, fmt);
	int n = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);
	if (n < 0) {
		return;
	}
	size_t size = (size_t)n + 1;
	LABWC_src_common_buf_c_buf_expand(s, s->len + (int)size);
	va_start(ap, fmt);
	n = vsnprintf(s->data + s->len, size, fmt, ap);
	va_end(ap);
	if (n < 0) {
		return;
	}
	s->len += n;
	s->data[s->len] = 0;
}
void
buf_add_hex_color(struct buf *s, float color[4])
{
	float alpha = color[3];
	if (alpha == 0.0f) {
		buf_add(s, "#00000000");
		return;
	}
	buf_add_fmt(s, "#%02x%02x%02x%02x",
		(int)(color[0] / alpha * 255),
		(int)(color[1] / alpha * 255),
		(int)(color[2] / alpha * 255),
		(int)(alpha * 255));
}
void
buf_add(struct buf *s, const char *data)
{
	if (string_null_or_empty(data)) {
		return;
	}
	int len = (int)strlen(data);
	LABWC_src_common_buf_c_buf_expand(s, s->len + len + 1);
	memcpy(s->data + s->len, data, (size_t)len);
	s->len += len;
	s->data[s->len] = 0;
}
void
buf_add_char(struct buf *s, char ch)
{
	LABWC_src_common_buf_c_buf_expand(s, s->len + 2);
	s->data[s->len++] = ch;
	s->data[s->len] = '\0';
}
void
buf_clear(struct buf *s)
{
	if (s->alloc) {
		assert(s->data);
		s->len = 0;
		s->data[0] = '\0';
	} else {
		*s = BUF_INIT;
	}
}
void
buf_reset(struct buf *s)
{
	if (s->alloc) {
		free(s->data);
	}
	*s = BUF_INIT;
}
void
buf_move(struct buf *dst, struct buf *src)
{
	if (dst->alloc) {
		free(dst->data);
	}
	*dst = *src;
	*src = BUF_INIT;
}
struct buf
buf_from_file(const char *filename)
{
	struct buf buf = BUF_INIT;
	FILE *stream = fopen(filename, "r");
	if (!stream) {
		return buf;
	}
	if (fseek(stream, 0, SEEK_END) == -1) {
		fclose(stream);
		return buf;
	}
	long size = ftell(stream);
	if (size == -1) {
		fclose(stream);
		return buf;
	}
	rewind(stream);
	LABWC_src_common_buf_c_buf_expand(&buf, (int)(size + 1));
	if (fread(buf.data, 1, (size_t)size, stream) == (size_t)size) {
		buf.len = (int)size;
		buf.data[size] = '\0';
	} else {
		buf_reset(&buf);
	}
	fclose(stream);
	return buf;
}
#include <strings.h>
enum lab_tristate
parse_tristate(const char *str)
{
	if (!str) {
		goto error_not_a_boolean;
	} else if (!strcasecmp(str, "yes")) {
		return LAB_STATE_ENABLED;
	} else if (!strcasecmp(str, "true")) {
		return LAB_STATE_ENABLED;
	} else if (!strcasecmp(str, "on")) {
		return LAB_STATE_ENABLED;
	} else if (!strcmp(str, "1")) {
		return LAB_STATE_ENABLED;
	} else if (!strcasecmp(str, "no")) {
		return LAB_STATE_DISABLED;
	} else if (!strcasecmp(str, "false")) {
		return LAB_STATE_DISABLED;
	} else if (!strcasecmp(str, "off")) {
		return LAB_STATE_DISABLED;
	} else if (!strcmp(str, "0")) {
		return LAB_STATE_DISABLED;
	}
error_not_a_boolean:
	return LAB_STATE_UNSPECIFIED;
}
int
parse_bool(const char *str, int default_value)
{
	enum lab_tristate val = parse_tristate(str);
	if (val == LAB_STATE_UNSPECIFIED) {
		return default_value;
	}
	return (val == LAB_STATE_ENABLED) ? 1 : 0;
}
void
set_bool(const char *str, bool *variable)
{
	int ret = parse_bool(str, -1);
	if (ret < 0) {
		return;
	}
	*variable = ret;
}
void
set_bool_as_int(const char *str, int *variable)
{
	int ret = parse_bool(str, -1);
	if (ret < 0) {
		return;
	}
	*variable = ret;
}
#include <sys/resource.h>
static struct rlimit LABWC_src_common_fd_util_c_original_nofile_rlimit = {0};
void
increase_nofile_limit(void)
{
	if (getrlimit(RLIMIT_NOFILE, &LABWC_src_common_fd_util_c_original_nofile_rlimit) != 0) {
		return;
	}
	struct rlimit new_rlimit = LABWC_src_common_fd_util_c_original_nofile_rlimit;
	new_rlimit.rlim_cur = new_rlimit.rlim_max;
	if (setrlimit(RLIMIT_NOFILE, &new_rlimit) != 0) {
	}
}
void
restore_nofile_limit(void)
{
	if (LABWC_src_common_fd_util_c_original_nofile_rlimit.rlim_cur == 0) {
		return;
	}
	if (setrlimit(RLIMIT_NOFILE, &LABWC_src_common_fd_util_c_original_nofile_rlimit) != 0) {
	}
}
#include <locale.h>
#include <stdlib.h>
struct LABWC_src_common_parse_double_c_dec_separator {
	int index;
	bool multiple;
};
struct LABWC_src_common_parse_double_c_converted_double {
	double value;
	bool valid;
};
static struct LABWC_src_common_parse_double_c_dec_separator
LABWC_src_common_parse_double_c_find_dec_separator(const char *str)
{
	struct LABWC_src_common_parse_double_c_dec_separator loc = {
		.index = -1,
		.multiple = false
	};
	for (int i = 0; *str; i++, str++) {
		switch (*str) {
		case ',':
		case '.':
			if (loc.index >= 0) {
				loc.multiple = true;
				return loc;
			} else {
				loc.index = i;
			}
			break;
		}
	}
	return loc;
}
static struct LABWC_src_common_parse_double_c_converted_double
LABWC_src_common_parse_double_c_convert_double(const char *str)
{
	struct LABWC_src_common_parse_double_c_converted_double result = {
		.value = 0,
		.valid = true,
	};
	char *eptr = NULL;
	errno = 0;
	result.value = strtod(str, &eptr);
	if (errno) {
		result.valid = false;
	}
	if (*eptr) {
		result.valid = false;
	}
	return result;
}
bool
set_double(const char *str, double *val)
{
	assert(str);
	assert(val);
	struct LABWC_src_common_parse_double_c_dec_separator dloc = LABWC_src_common_parse_double_c_find_dec_separator(str);
	if (dloc.multiple) {
		return false;
	}
	char *lstr = NULL;
	if (dloc.index >= 0) {
		lstr = xstrdup(str);
		struct lconv *lc = localeconv();
		lstr[dloc.index] = *lc->decimal_point;
		str = lstr;
	}
	struct LABWC_src_common_parse_double_c_converted_double conv = LABWC_src_common_parse_double_c_convert_double(str);
	if (conv.valid) {
		*val = conv.value;
	}
	free(lstr);
	return conv.valid;
}
#include <assert.h>
#include <stdio.h>
#include <string.h>
void
die_if_null(void *ptr)
{
	if (!ptr) {
		perror("Failed to allocate memory");
		exit(EXIT_FAILURE);
	}
}
void *
xzalloc(size_t size)
{
	if (!size) {
		return NULL;
	}
	void *ptr = calloc(1, size);
	die_if_null(ptr);
	return ptr;
}
void *
xrealloc(void *ptr, size_t size)
{
	if (!size) {
		free(ptr);
		return NULL;
	}
	ptr = realloc(ptr, size);
	die_if_null(ptr);
	return ptr;
}
char *
xstrdup(const char *str)
{
	assert(str);
	char *copy = strdup(str);
	die_if_null(copy);
	return copy;
}
#include <assert.h>
#include <stdlib.h>
#include <string.h>
static struct lab_data_buffer *
LABWC_src_scaled_buffer_scaled_font_buffer_c__create_buffer(struct scaled_buffer *scaled_buffer, double scale)
{
	struct lab_data_buffer *buffer = NULL;
	struct scaled_font_buffer *self = scaled_buffer->data;
	cairo_pattern_t *bg_pattern = color_to_pattern(self->bg_color);
	font_buffer_create(&buffer, self->max_width, self->height, self->text,
		&self->font, self->color, bg_pattern, scale, self->use_markup);
	if (!buffer) {
	}
	cairo_pattern_destroy(bg_pattern);
	return buffer;
}
static void
LABWC_src_scaled_buffer_scaled_font_buffer_c__destroy(struct scaled_buffer *scaled_buffer)
{
	struct scaled_font_buffer *self = scaled_buffer->data;
	scaled_buffer->data = NULL;
	zfree(self->text);
	zfree(self->font.name);
	free(self);
}
static bool
LABWC_src_scaled_buffer_scaled_font_buffer_c__equal(struct scaled_buffer *scaled_buffer_a,
	struct scaled_buffer *scaled_buffer_b)
{
	struct scaled_font_buffer *a = scaled_buffer_a->data;
	struct scaled_font_buffer *b = scaled_buffer_b->data;
	return str_equal(a->text, b->text)
		&& a->use_markup == b->use_markup
		&& a->max_width == b->max_width
		&& str_equal(a->font.name, b->font.name)
		&& a->font.size == b->font.size
		&& a->font.slant == b->font.slant
		&& a->font.weight == b->font.weight
		&& !memcmp(a->color, b->color, sizeof(a->color))
		&& !memcmp(a->bg_color, b->bg_color, sizeof(a->bg_color));
}
static const struct scaled_buffer_impl LABWC_src_scaled_buffer_scaled_font_buffer_c_impl = {
	.create_buffer = LABWC_src_scaled_buffer_scaled_font_buffer_c__create_buffer,
	.destroy = LABWC_src_scaled_buffer_scaled_font_buffer_c__destroy,
	.equal = LABWC_src_scaled_buffer_scaled_font_buffer_c__equal,
};
struct scaled_font_buffer *
scaled_font_buffer_create(struct wlr_scene_tree *parent)
{
	assert(parent);
	struct scaled_font_buffer *self = znew(*self);
	struct scaled_buffer *scaled_buffer = scaled_buffer_create(
		parent, &LABWC_src_scaled_buffer_scaled_font_buffer_c_impl,   true);
	if (!scaled_buffer) {
		free(self);
		return NULL;
	}
	scaled_buffer->data = self;
	self->scaled_buffer = scaled_buffer;
	self->scene_buffer = scaled_buffer->scene_buffer;
	return self;
}
static void
LABWC_src_scaled_buffer_scaled_font_buffer_c__scaled_font_buffer_update(struct scaled_font_buffer *self, const char *text,
		int max_width, struct font *font, const float *color,
		const float *bg_color, bool use_markup)
{
	assert(self);
	assert(text);
	assert(font);
	assert(color);
	zfree(self->text);
	zfree(self->font.name);
	self->text = xstrdup(text);
	self->use_markup = use_markup;
	self->max_width = max_width;
	if (font->name) {
		self->font.name = xstrdup(font->name);
	}
	self->font.size = font->size;
	self->font.slant = font->slant;
	self->font.weight = font->weight;
	memcpy(self->color, color, sizeof(self->color));
	memcpy(self->bg_color, bg_color, sizeof(self->bg_color));
	font_get_buffer_size(self->max_width, self->text, &self->font,
		&self->width, &self->height);
	scaled_buffer_request_update(self->scaled_buffer,
		self->width, self->height);
}
void
scaled_font_buffer_update(struct scaled_font_buffer *self, const char *text,
		int max_width, struct font *font, const float *color,
		const float *bg_color)
{
	LABWC_src_scaled_buffer_scaled_font_buffer_c__scaled_font_buffer_update(self, text, max_width, font, color, bg_color, false);
}
void
scaled_font_buffer_update_markup(struct scaled_font_buffer *self, const char *text,
		int max_width, struct font *font, const float *color,
		const float *bg_color, bool use_markup)
{
	LABWC_src_scaled_buffer_scaled_font_buffer_c__scaled_font_buffer_update(self, text, max_width, font, color, bg_color, use_markup);
}
#include <assert.h>
#include <stdlib.h>
#include <wayland-server-core.h>
static struct wl_list LABWC_src_scaled_buffer_scaled_buffer_c_all_scaled_buffers = WL_LIST_INIT(&LABWC_src_scaled_buffer_scaled_buffer_c_all_scaled_buffers);
static void
LABWC_src_scaled_buffer_scaled_buffer_c__cache_entry_destroy(struct scaled_buffer_cache_entry *cache_entry, bool drop_buffer)
{
	wl_list_remove(&cache_entry->link);
	if (cache_entry->buffer) {
		if (drop_buffer && !cache_entry->buffer->dropped) {
			wlr_buffer_drop(cache_entry->buffer);
		}
		wlr_buffer_unlock(cache_entry->buffer);
	}
	free(cache_entry);
}
static struct scaled_buffer_cache_entry *
LABWC_src_scaled_buffer_scaled_buffer_c_find_cache_for_scale(struct scaled_buffer *scene_buffer, double scale)
{
	struct scaled_buffer_cache_entry *cache_entry;
	wl_list_for_each(cache_entry, &scene_buffer->cache, link) {
		if (cache_entry->scale == scale) {
			return cache_entry;
		}
	}
	return NULL;
}
static void
LABWC_src_scaled_buffer_scaled_buffer_c__update_buffer(struct scaled_buffer *self, double scale)
{
	self->active_scale = scale;
	struct scaled_buffer_cache_entry *cache_entry =
		LABWC_src_scaled_buffer_scaled_buffer_c_find_cache_for_scale(self, scale);
	if (cache_entry) {
		wl_list_remove(&cache_entry->link);
		wl_list_insert(&self->cache, &cache_entry->link);
		wlr_scene_buffer_set_buffer(self->scene_buffer, cache_entry->buffer);
		return;
	}
	struct wlr_buffer *wlr_buffer = NULL;
	if (self->impl->equal) {
		struct scaled_buffer *scene_buffer;
		wl_list_for_each(scene_buffer, &LABWC_src_scaled_buffer_scaled_buffer_c_all_scaled_buffers, link) {
			if (scene_buffer == self) {
				continue;
			}
			if (self->impl != scene_buffer->impl) {
				continue;
			}
			if (!self->impl->equal(self, scene_buffer)) {
				continue;
			}
			cache_entry = LABWC_src_scaled_buffer_scaled_buffer_c_find_cache_for_scale(scene_buffer, scale);
			if (!cache_entry) {
				continue;
			}
			self->width = scene_buffer->width;
			self->height = scene_buffer->height;
			wlr_buffer = cache_entry->buffer;
			break;
		}
	}
	if (!wlr_buffer) {
		struct lab_data_buffer *buffer =
			self->impl->create_buffer(self, scale);
		if (buffer) {
			self->width = (int)buffer->logical_width;
			self->height = (int)buffer->logical_height;
			wlr_buffer = &buffer->base;
		} else {
			self->width = 0;
			self->height = 0;
		}
	}
	if (wlr_buffer) {
		wlr_buffer_lock(wlr_buffer);
	}
	if (wl_list_length(&self->cache) < LAB_SCALED_BUFFER_MAX_CACHE) {
		cache_entry = znew(*cache_entry);
	} else {
		cache_entry = wl_container_of(self->cache.prev, cache_entry, link);
		if (cache_entry->buffer) {
			if (self->drop_buffer && !cache_entry->buffer->dropped) {
				wlr_buffer_drop(cache_entry->buffer);
			}
			wlr_buffer_unlock(cache_entry->buffer);
		}
		wl_list_remove(&cache_entry->link);
	}
	cache_entry->scale = scale;
	cache_entry->buffer = wlr_buffer;
	wl_list_insert(&self->cache, &cache_entry->link);
	wlr_scene_buffer_set_buffer(self->scene_buffer, cache_entry->buffer);
	wlr_scene_buffer_set_dest_size(self->scene_buffer, self->width, self->height);
}
static void
LABWC_src_scaled_buffer_scaled_buffer_c__handle_node_destroy(struct wl_listener *listener, void *data)
{
	struct scaled_buffer_cache_entry *cache_entry, *cache_entry_tmp;
	struct scaled_buffer *self = wl_container_of(listener, self, destroy);
	wl_list_remove(&self->destroy.link);
	wl_list_remove(&self->outputs_update.link);
	wl_list_for_each_safe(cache_entry, cache_entry_tmp, &self->cache, link) {
		LABWC_src_scaled_buffer_scaled_buffer_c__cache_entry_destroy(cache_entry, self->drop_buffer);
	}
	assert(wl_list_empty(&self->cache));
	if (self->impl->destroy) {
		self->impl->destroy(self);
	}
	wl_list_remove(&self->link);
	free(self);
}
static void
LABWC_src_scaled_buffer_scaled_buffer_c__handle_outputs_update(struct wl_listener *listener, void *data)
{
	struct scaled_buffer *self =
		wl_container_of(listener, self, outputs_update);
	double max_scale = 0;
	struct wlr_scene_outputs_update_event *event = data;
	for (size_t i = 0; i < event->size; i++) {
		max_scale = MAX(max_scale, event->active[i]->output->scale);
	}
	if (max_scale && self->active_scale != max_scale) {
		LABWC_src_scaled_buffer_scaled_buffer_c__update_buffer(self, max_scale);
	}
}
struct scaled_buffer *
scaled_buffer_create(struct wlr_scene_tree *parent,
		const struct scaled_buffer_impl *impl,
		bool drop_buffer)
{
	assert(parent);
	assert(impl);
	assert(impl->create_buffer);
	struct scaled_buffer *self = znew(*self);
	self->scene_buffer = lab_wlr_scene_buffer_create(parent, NULL);
	self->impl = impl;
	self->active_scale = 0;
	self->drop_buffer = drop_buffer;
	wl_list_init(&self->cache);
	wl_list_insert(&LABWC_src_scaled_buffer_scaled_buffer_c_all_scaled_buffers, &self->link);
	self->outputs_update.notify = LABWC_src_scaled_buffer_scaled_buffer_c__handle_outputs_update;
	wl_signal_add(&self->scene_buffer->events.outputs_update, &self->outputs_update);
	self->destroy.notify = LABWC_src_scaled_buffer_scaled_buffer_c__handle_node_destroy;
	wl_signal_add(&self->scene_buffer->node.events.destroy, &self->destroy);
	return self;
}
void
scaled_buffer_request_update(struct scaled_buffer *self,
		int width, int height)
{
	assert(self);
	assert(width >= 0);
	assert(height >= 0);
	struct scaled_buffer_cache_entry *cache_entry, *cache_entry_tmp;
	wl_list_for_each_safe(cache_entry, cache_entry_tmp, &self->cache, link) {
		LABWC_src_scaled_buffer_scaled_buffer_c__cache_entry_destroy(cache_entry, self->drop_buffer);
	}
	assert(wl_list_empty(&self->cache));
	wlr_scene_buffer_set_dest_size(self->scene_buffer, width, height);
	self->width = width;
	self->height = height;
	if (self->active_scale > 0) {
		LABWC_src_scaled_buffer_scaled_buffer_c__update_buffer(self, self->active_scale);
	}
}
void
scaled_buffer_invalidate_sharing(void)
{
	struct scaled_buffer *scene_buffer, *tmp;
	wl_list_for_each_safe(scene_buffer, tmp, &LABWC_src_scaled_buffer_scaled_buffer_c_all_scaled_buffers, link) {
		wl_list_remove(&scene_buffer->link);
		wl_list_init(&scene_buffer->link);
	}
}
#include <assert.h>
#include <stdint.h>
#define LABWC_BUILTIN_ICON_SIZE (6)
static uint32_t
LABWC_src_img_img_xbm_c_argb32(float *rgba)
{
	uint32_t r[4] = { 0 };
	for (int i = 0; i < 4; i++) {
		r[i] = (uint32_t)(rgba[i] * 255);
	}
	return ((r[3] & 0xff) << 24) | ((r[0] & 0xff) << 16) |
		((r[1] & 0xff) << 8) | (r[2] & 0xff);
}
struct lab_data_buffer *
img_xbm_load_from_bitmap(const char *bitmap, float *rgba)
{
	assert(bitmap);
	const int size = LABWC_BUILTIN_ICON_SIZE;
	uint32_t color = LABWC_src_img_img_xbm_c_argb32(rgba);
	uint32_t *data = znew_n(uint32_t, (size_t)(size * size));
	for (int row = 0; row < size; row++) {
		uint8_t bits = (uint8_t)bitmap[row];
		for (int col = 0; col < size; col++) {
			if (bits & (1u << col)) {
				data[row * size + col] = color;
			}
		}
	}
	return buffer_create_from_data(data, (uint32_t)size, (uint32_t)size,
		(uint32_t)(size * 4));
}
#undef LABWC_BUILTIN_ICON_SIZE
#include <assert.h>
struct lab_img_data {
	int refcount;
	struct lab_data_buffer *buffer;
};
static struct lab_img *
LABWC_src_img_img_c_create_img(struct lab_img_data *img_data)
{
	struct lab_img *img = znew(*img);
	img->data = img_data;
	img_data->refcount++;
	wl_array_init(&img->modifiers);
	return img;
}
struct lab_img *
lab_img_load_from_bitmap(const char *bitmap, float *rgba)
{
	struct lab_data_buffer *buffer = img_xbm_load_from_bitmap(bitmap, rgba);
	if (!buffer) {
		return NULL;
	}
	struct lab_img_data *img_data = znew(*img_data);
	img_data->buffer = buffer;
	return LABWC_src_img_img_c_create_img(img_data);
}
struct lab_img *
lab_img_copy(struct lab_img *img)
{
	struct lab_img *new_img = LABWC_src_img_img_c_create_img(img->data);
	wl_array_copy(&new_img->modifiers, &img->modifiers);
	return new_img;
}
void
lab_img_add_modifier(struct lab_img *img, lab_img_modifier_func_t modifier)
{
	lab_img_modifier_func_t *mod = wl_array_add(&img->modifiers, sizeof(*mod));
	*mod = modifier;
}
struct lab_data_buffer *
lab_img_render(struct lab_img *img, int width, int height, double scale)
{
	struct lab_data_buffer *buffer =
		buffer_resize(img->data->buffer, width, height, scale);
	if (!buffer) {
		return NULL;
	}
	cairo_t *cairo = cairo_create(buffer->surface);
	lab_img_modifier_func_t *modifier;
	wl_array_for_each(modifier, &img->modifiers) {
		cairo_save(cairo);
		(*modifier)(cairo, width, height);
		cairo_restore(cairo);
	}
	cairo_surface_flush(buffer->surface);
	cairo_destroy(cairo);
	return buffer;
}
void
lab_img_destroy(struct lab_img *img)
{
	if (!img) {
		return;
	}
	img->data->refcount--;
	if (img->data->refcount == 0) {
		if (img->data->buffer) {
			wlr_buffer_drop(&img->data->buffer->base);
		}
		free(img->data);
	}
	wl_array_release(&img->modifiers);
	free(img);
}
bool
lab_img_equal(struct lab_img *img_a, struct lab_img *img_b)
{
	if (img_a == img_b) {
		return true;
	}
	if (!img_a || !img_b || img_a->data != img_b->data
			|| img_a->modifiers.size != img_b->modifiers.size) {
		return false;
	}
	return img_a->modifiers.size == 0
		|| !memcmp(img_a->modifiers.data, img_b->modifiers.data,
			img_a->modifiers.size);
}
#include <assert.h>
static struct lab_data_buffer *
LABWC_src_scaled_buffer_scaled_img_buffer_c__create_buffer(struct scaled_buffer *scaled_buffer, double scale)
{
	struct scaled_img_buffer *self = scaled_buffer->data;
	struct lab_data_buffer *buffer = lab_img_render(self->img,
		self->width, self->height, scale);
	return buffer;
}
static void
LABWC_src_scaled_buffer_scaled_img_buffer_c__destroy(struct scaled_buffer *scaled_buffer)
{
	struct scaled_img_buffer *self = scaled_buffer->data;
	lab_img_destroy(self->img);
	free(self);
}
static bool
LABWC_src_scaled_buffer_scaled_img_buffer_c__equal(struct scaled_buffer *scaled_buffer_a,
	struct scaled_buffer *scaled_buffer_b)
{
	struct scaled_img_buffer *a = scaled_buffer_a->data;
	struct scaled_img_buffer *b = scaled_buffer_b->data;
	return lab_img_equal(a->img, b->img)
		&& a->width == b->width
		&& a->height == b->height;
}
static struct scaled_buffer_impl LABWC_src_scaled_buffer_scaled_img_buffer_c_impl = {
	.create_buffer = LABWC_src_scaled_buffer_scaled_img_buffer_c__create_buffer,
	.destroy = LABWC_src_scaled_buffer_scaled_img_buffer_c__destroy,
	.equal = LABWC_src_scaled_buffer_scaled_img_buffer_c__equal,
};
struct scaled_img_buffer *
scaled_img_buffer_create(struct wlr_scene_tree *parent, struct lab_img *img,
	int width, int height)
{
	assert(parent);
	assert(img);
	assert(width >= 0 && height >= 0);
	struct scaled_buffer *scaled_buffer = scaled_buffer_create(
		parent, &LABWC_src_scaled_buffer_scaled_img_buffer_c_impl,   true);
	struct scaled_img_buffer *self = znew(*self);
	self->scaled_buffer = scaled_buffer;
	self->scene_buffer = scaled_buffer->scene_buffer;
	self->img = lab_img_copy(img);
	self->width = width;
	self->height = height;
	scaled_buffer->data = self;
	scaled_buffer_request_update(scaled_buffer, width, height);
	return self;
}
#include <fnmatch.h>
bool
match_glob(const char *pattern, const char *string)
{
	return fnmatch(pattern, string, FNM_CASEFOLD) == 0;
}
#include <strings.h>
enum lab_node_type
node_type_parse(const char *context)
{
	if (!strcasecmp(context, "Close")) {
		return LAB_NODE_BUTTON_CLOSE;
	} else if (!strcasecmp(context, "Maximize")) {
		return LAB_NODE_BUTTON_MAXIMIZE;
	} else if (!strcasecmp(context, "Iconify")) {
		return LAB_NODE_BUTTON_ICONIFY;
	} else if (!strcasecmp(context, "WindowMenu")) {
		return LAB_NODE_BUTTON_WINDOW_MENU;
	} else if (!strcasecmp(context, "Shade")) {
		return LAB_NODE_BUTTON_SHADE;
	} else if (!strcasecmp(context, "AllDesktops")) {
		return LAB_NODE_BUTTON_OMNIPRESENT;
	} else if (!strcasecmp(context, "Titlebar")
			|| !strcasecmp(context, "Title")) {
		return LAB_NODE_TITLEBAR;
	} else if (!strcasecmp(context, "TLCorner")) {
		return LAB_NODE_CORNER_TOP_LEFT;
	} else if (!strcasecmp(context, "TRCorner")) {
		return LAB_NODE_CORNER_TOP_RIGHT;
	} else if (!strcasecmp(context, "BRCorner")) {
		return LAB_NODE_CORNER_BOTTOM_RIGHT;
	} else if (!strcasecmp(context, "BLCorner")) {
		return LAB_NODE_CORNER_BOTTOM_LEFT;
	} else if (!strcasecmp(context, "Border")) {
		return LAB_NODE_BORDER;
	} else if (!strcasecmp(context, "Top")) {
		return LAB_NODE_BORDER_TOP;
	} else if (!strcasecmp(context, "Right")) {
		return LAB_NODE_BORDER_RIGHT;
	} else if (!strcasecmp(context, "Bottom")) {
		return LAB_NODE_BORDER_BOTTOM;
	} else if (!strcasecmp(context, "Left")) {
		return LAB_NODE_BORDER_LEFT;
	} else if (!strcasecmp(context, "Frame")) {
		return LAB_NODE_FRAME;
	} else if (!strcasecmp(context, "Client")) {
		return LAB_NODE_CLIENT;
	} else if (!strcasecmp(context, "Desktop")) {
		return LAB_NODE_ROOT;
	} else if (!strcasecmp(context, "Root")) {
		return LAB_NODE_ROOT;
	} else if (!strcasecmp(context, "All")) {
		return LAB_NODE_ALL;
	}
	return LAB_NODE_NONE;
}
bool
node_type_contains(enum lab_node_type whole, enum lab_node_type part)
{
	if (whole == part || whole == LAB_NODE_ALL) {
		return true;
	}
	if (whole == LAB_NODE_BUTTON) {
		return part >= LAB_NODE_BUTTON_FIRST
			&& part <= LAB_NODE_BUTTON_LAST;
	}
	if (whole == LAB_NODE_TITLEBAR) {
		return part == LAB_NODE_TITLEBAR;
	}
	if (whole == LAB_NODE_FRAME) {
		return part >= LAB_NODE_BUTTON_FIRST
			&& part <= LAB_NODE_CLIENT;
	}
	if (whole == LAB_NODE_BORDER) {
		return part >= LAB_NODE_CORNER_TOP_LEFT
			&& part <= LAB_NODE_BORDER_LEFT;
	}
	if (whole == LAB_NODE_BORDER_TOP) {
		return part == LAB_NODE_CORNER_TOP_LEFT
			|| part == LAB_NODE_CORNER_TOP_RIGHT;
	}
	if (whole == LAB_NODE_BORDER_RIGHT) {
		return part == LAB_NODE_CORNER_TOP_RIGHT
			|| part == LAB_NODE_CORNER_BOTTOM_RIGHT;
	}
	if (whole == LAB_NODE_BORDER_BOTTOM) {
		return part == LAB_NODE_CORNER_BOTTOM_RIGHT
			|| part == LAB_NODE_CORNER_BOTTOM_LEFT;
	}
	if (whole == LAB_NODE_BORDER_LEFT) {
		return part == LAB_NODE_CORNER_TOP_LEFT
			|| part == LAB_NODE_CORNER_BOTTOM_LEFT;
	}
	return false;
}
enum lab_edge
node_type_to_edges(enum lab_node_type type)
{
	switch (type) {
	case LAB_NODE_BORDER_TOP:
		return LAB_EDGE_TOP;
	case LAB_NODE_BORDER_RIGHT:
		return LAB_EDGE_RIGHT;
	case LAB_NODE_BORDER_BOTTOM:
		return LAB_EDGE_BOTTOM;
	case LAB_NODE_BORDER_LEFT:
		return LAB_EDGE_LEFT;
	case LAB_NODE_CORNER_TOP_LEFT:
		return LAB_EDGES_TOP_LEFT;
	case LAB_NODE_CORNER_TOP_RIGHT:
		return LAB_EDGES_TOP_RIGHT;
	case LAB_NODE_CORNER_BOTTOM_RIGHT:
		return LAB_EDGES_BOTTOM_RIGHT;
	case LAB_NODE_CORNER_BOTTOM_LEFT:
		return LAB_EDGES_BOTTOM_LEFT;
	default:
		return LAB_EDGE_NONE;
	}
}
#include <assert.h>
#include <strings.h>
static_assert((int)LAB_EDGE_TOP == (int)WLR_EDGE_TOP
		&& (int)LAB_EDGE_BOTTOM == (int)WLR_EDGE_BOTTOM
		&& (int)LAB_EDGE_LEFT == (int)WLR_EDGE_LEFT
		&& (int)LAB_EDGE_RIGHT == (int)WLR_EDGE_RIGHT,
	"enum lab_edge does not match enum wlr_edges");
static_assert((int)LAB_EDGE_TOP == (int)WLR_DIRECTION_UP
		&& (int)LAB_EDGE_BOTTOM == (int)WLR_DIRECTION_DOWN
		&& (int)LAB_EDGE_LEFT == (int)WLR_DIRECTION_LEFT
		&& (int)LAB_EDGE_RIGHT == (int)WLR_DIRECTION_RIGHT,
	"enum lab_edge does not match enum wlr_direction");
enum lab_edge
lab_edge_parse(const char *direction, bool tiled, bool any)
{
	if (!direction) {
		return LAB_EDGE_NONE;
	}
	if (!strcasecmp(direction, "left")) {
		return LAB_EDGE_LEFT;
	} else if (!strcasecmp(direction, "up")) {
		return LAB_EDGE_TOP;
	} else if (!strcasecmp(direction, "right")) {
		return LAB_EDGE_RIGHT;
	} else if (!strcasecmp(direction, "down")) {
		return LAB_EDGE_BOTTOM;
	}
	if (any) {
		if (!strcasecmp(direction, "any")) {
			return LAB_EDGE_ANY;
		}
	}
	if (tiled) {
		if (!strcasecmp(direction, "center")) {
			return LAB_EDGE_CENTER;
		} else if (!strcasecmp(direction, "up-left")) {
			return LAB_EDGES_TOP_LEFT;
		} else if (!strcasecmp(direction, "up-right")) {
			return LAB_EDGES_TOP_RIGHT;
		} else if (!strcasecmp(direction, "down-left")) {
			return LAB_EDGES_BOTTOM_LEFT;
		} else if (!strcasecmp(direction, "down-right")) {
			return LAB_EDGES_BOTTOM_RIGHT;
		}
	}
	return LAB_EDGE_NONE;
}
bool
lab_edge_is_cardinal(enum lab_edge edge)
{
	switch (edge) {
	case LAB_EDGE_TOP:
	case LAB_EDGE_BOTTOM:
	case LAB_EDGE_LEFT:
	case LAB_EDGE_RIGHT:
		return true;
	default:
		return false;
	}
}
enum lab_edge
lab_edge_invert(enum lab_edge edge)
{
	switch (edge) {
	case LAB_EDGE_LEFT:
		return LAB_EDGE_RIGHT;
	case LAB_EDGE_RIGHT:
		return LAB_EDGE_LEFT;
	case LAB_EDGE_TOP:
		return LAB_EDGE_BOTTOM;
	case LAB_EDGE_BOTTOM:
		return LAB_EDGE_TOP;
	default:
		return LAB_EDGE_NONE;
	}
}
#include <assert.h>
struct border_scene {
	struct wlr_scene_tree *tree;
	struct wlr_scene_rect *top, *bottom, *left, *right;
};
static void
LABWC_src_common_lab_scene_rect_c_handle_node_destroy(struct wl_listener *listener, void *data)
{
	struct lab_scene_rect *rect = wl_container_of(listener, rect, node_destroy);
	wl_list_remove(&rect->node_destroy.link);
	free(rect->borders);
	free(rect);
}
struct lab_scene_rect *
lab_scene_rect_create(struct wlr_scene_tree *parent,
		struct lab_scene_rect_options *opts)
{
	struct lab_scene_rect *rect = znew(*rect);
	rect->border_width = opts->border_width;
	rect->nr_borders = opts->nr_borders;
	rect->borders = znew_n(rect->borders[0], opts->nr_borders);
	rect->tree = lab_wlr_scene_tree_create(parent);
	if (opts->bg_color) {
		rect->fill = lab_wlr_scene_rect_create(rect->tree, 0, 0, opts->bg_color);
	}
	for (int i = 0; i < rect->nr_borders; i++) {
		struct border_scene *border = &rect->borders[i];
		float *color = opts->border_colors[i];
		border->tree = lab_wlr_scene_tree_create(rect->tree);
		border->top = lab_wlr_scene_rect_create(border->tree, 0, 0, color);
		border->right = lab_wlr_scene_rect_create(border->tree, 0, 0, color);
		border->bottom = lab_wlr_scene_rect_create(border->tree, 0, 0, color);
		border->left = lab_wlr_scene_rect_create(border->tree, 0, 0, color);
	}
	rect->node_destroy.notify = LABWC_src_common_lab_scene_rect_c_handle_node_destroy;
	wl_signal_add(&rect->tree->node.events.destroy, &rect->node_destroy);
	lab_scene_rect_set_size(rect, opts->width, opts->height);
	return rect;
}
static void
LABWC_src_common_lab_scene_rect_c_resize_border(struct border_scene *border, int border_width, int width, int height)
{
	if ((width < border_width * 2) || (height < border_width * 2)) {
		wlr_scene_node_set_enabled(&border->tree->node, false);
		return;
	}
	wlr_scene_node_set_enabled(&border->tree->node, true);
	wlr_scene_node_set_position(&border->top->node, 0, 0);
	wlr_scene_node_set_position(&border->bottom->node, 0, height - border_width);
	wlr_scene_node_set_position(&border->left->node, 0, border_width);
	wlr_scene_node_set_position(&border->right->node, width - border_width, border_width);
	wlr_scene_rect_set_size(border->top, width, border_width);
	wlr_scene_rect_set_size(border->bottom, width, border_width);
	wlr_scene_rect_set_size(border->left, border_width, height - border_width * 2);
	wlr_scene_rect_set_size(border->right, border_width, height - border_width * 2);
}
void
lab_scene_rect_set_size(struct lab_scene_rect *rect, int width, int height)
{
	assert(rect);
	int border_width = rect->border_width;
	for (int i = 0; i < rect->nr_borders; i++) {
		struct border_scene *border = &rect->borders[i];
		LABWC_src_common_lab_scene_rect_c_resize_border(border, border_width,
			width - 2 * border_width * i,
			height - 2 * border_width * i);
		wlr_scene_node_set_position(&border->tree->node,
			i * border_width, i * border_width);
	}
	if (rect->fill) {
		wlr_scene_rect_set_size(rect->fill, width, height);
	}
}
#include <assert.h>
struct wlr_surface *
lab_wlr_surface_from_node(struct wlr_scene_node *node)
{
	struct wlr_scene_buffer *buffer;
	struct wlr_scene_surface *scene_surface;
	if (node && node->type == WLR_SCENE_NODE_BUFFER) {
		buffer = wlr_scene_buffer_from_node(node);
		scene_surface = wlr_scene_surface_try_from_buffer(buffer);
		if (scene_surface) {
			return scene_surface->surface;
		}
	}
	return NULL;
}
struct wlr_scene_tree *
lab_wlr_scene_tree_create(struct wlr_scene_tree *parent)
{
	struct wlr_scene_tree *tree = wlr_scene_tree_create(parent);
	die_if_null(tree);
	return tree;
}
struct wlr_scene_rect *
lab_wlr_scene_rect_create(struct wlr_scene_tree *parent,
		int width, int height, const float color[static 4])
{
	struct wlr_scene_rect *rect =
		wlr_scene_rect_create(parent, width, height, color);
	die_if_null(rect);
	return rect;
}
struct wlr_scene_buffer *
lab_wlr_scene_buffer_create(struct wlr_scene_tree *parent,
		struct wlr_buffer *buffer)
{
	struct wlr_scene_buffer *scene_buffer =
		wlr_scene_buffer_create(parent, buffer);
	die_if_null(scene_buffer);
	return scene_buffer;
}
struct wlr_scene_node *
lab_wlr_scene_get_prev_node(struct wlr_scene_node *node)
{
	assert(node);
	struct wlr_scene_node *prev;
	prev = wl_container_of(node->link.prev, node, link);
	if (&prev->link == &node->parent->children) {
		return NULL;
	}
	return prev;
}
bool
lab_wlr_scene_output_commit(struct wlr_scene_output *scene_output,
		struct wlr_output_state *state)
{
	assert(scene_output);
	assert(state);
	struct wlr_output *wlr_output = scene_output->output;
	struct output *output = wlr_output->data;
	if (!wlr_scene_output_needs_frame(scene_output)) {
		return true;
	}
	if (!wlr_scene_output_build_state(scene_output, state, NULL)) {
		return false;
	}
	if (state->tearing_page_flip) {
		if (!wlr_output_test_state(wlr_output, state)) {
			state->tearing_page_flip = false;
		}
	}
	bool committed = wlr_output_commit_state(wlr_output, state);
	if (!committed && state->tearing_page_flip) {
		state->tearing_page_flip = false;
		committed = wlr_output_commit_state(wlr_output, state);
	}
	if (committed) {
		if (state == &output->pending) {
			wlr_output_state_finish(&output->pending);
			wlr_output_state_init(&output->pending);
		}
	} else {
		return false;
	}
	return true;
}
#include <cairo.h>
#include <glib.h>
void
draw_cairo_border(cairo_t *cairo, struct wlr_fbox fbox, double line_width)
{
	cairo_save(cairo);
	fbox.x += line_width / 2.0;
	fbox.y += line_width / 2.0;
	fbox.width -= line_width;
	fbox.height -= line_width;
	cairo_set_line_width(cairo, line_width);
	cairo_rectangle(cairo, fbox.x, fbox.y, fbox.width, fbox.height);
	cairo_stroke(cairo);
	cairo_restore(cairo);
}
void
set_cairo_color(cairo_t *cairo, const float *color)
{
	float alpha = color[3];
	if (alpha == 0.0f) {
		cairo_set_source_rgba(cairo, 0, 0, 0, 0);
		return;
	}
	cairo_set_source_rgba(cairo, color[0] / alpha, color[1] / alpha,
		color[2] / alpha, alpha);
}
cairo_pattern_t *
color_to_pattern(const float *color)
{
	float alpha = color[3];
	if (alpha == 0.0f) {
		return cairo_pattern_create_rgba(0, 0, 0, 0);
	}
	return cairo_pattern_create_rgba(color[0] / alpha, color[1] / alpha,
		color[2] / alpha, alpha);
}
bool
is_pattern_opaque(cairo_pattern_t *pattern)
{
	double alpha = 0;
	int stops = 0;
	if (cairo_pattern_get_rgba(pattern, NULL, NULL, NULL, &alpha)
			== CAIRO_STATUS_SUCCESS) {
		return (alpha >= 0.999);
	}
	if (cairo_pattern_get_color_stop_count(pattern, &stops)
			== CAIRO_STATUS_SUCCESS) {
		for (int s = 0; s < stops; s++) {
			cairo_pattern_get_color_stop_rgba(pattern, s,
				NULL, NULL, NULL, NULL, &alpha);
			if (alpha < 0.999) {
				return false;
			}
		}
		return true;
	}
	return false;
}
static int
LABWC_src_common_graphic_helpers_c_compare_xcolor_entry(const void *a, const void *b)
{
	return g_ascii_strcasecmp((const char *)a,
		color_names + ((const struct xcolor_entry *)b)->name_offset);
}
bool
lookup_named_color(const char *name, uint32_t *argb)
{
	const struct xcolor_entry *found = bsearch(name, xcolors, ARRAY_SIZE(xcolors),
		sizeof(struct xcolor_entry), LABWC_src_common_graphic_helpers_c_compare_xcolor_entry);
	if (!found) {
		return false;
	}
	*argb = 0xFF000000u | ((uint32_t)found->red << 16)
		| ((uint32_t)found->green << 8) | found->blue;
	return true;
}
#include <cairo.h>
#include <pango/pangocairo.h>
PangoFontDescription *
font_to_pango_desc(struct font *font)
{
	PangoFontDescription *desc = pango_font_description_new();
	pango_font_description_set_family(desc, font->name);
	pango_font_description_set_size(desc, font->size * PANGO_SCALE);
	pango_font_description_set_style(desc, font->slant);
	pango_font_description_set_weight(desc, font->weight);
	return desc;
}
static PangoRectangle
LABWC_src_common_font_c_font_extents(struct font *font, const char *string)
{
	PangoRectangle rect = { 0 };
	if (string_null_or_empty(string)) {
		return rect;
	}
	cairo_surface_t *surface;
	cairo_t *c;
	PangoLayout *layout;
	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1, 1);
	c = cairo_create(surface);
	layout = pango_cairo_create_layout(c);
	pango_context_set_round_glyph_positions(pango_layout_get_context(layout), false);
	PangoFontDescription *desc = font_to_pango_desc(font);
	pango_layout_set_font_description(layout, desc);
	pango_layout_set_text(layout, string, -1);
	pango_layout_set_single_paragraph_mode(layout, TRUE);
	pango_layout_set_width(layout, -1);
	pango_layout_set_ellipsize(layout, PANGO_ELLIPSIZE_MIDDLE);
	pango_layout_get_extents(layout, NULL, &rect);
	pango_extents_to_pixels(&rect, NULL);
	cairo_destroy(c);
	cairo_surface_destroy(surface);
	pango_font_description_free(desc);
	g_object_unref(layout);
	return rect;
}
int
font_height(struct font *font)
{
	PangoRectangle rectangle = LABWC_src_common_font_c_font_extents(font, "abcdefg");
	return rectangle.height;
}
int
font_width(struct font *font, const char *string)
{
	PangoRectangle rectangle = LABWC_src_common_font_c_font_extents(font, string);
	return rectangle.width;
}
void
font_get_buffer_size(int max_width, const char *text, struct font *font,
	int *width, int *height)
{
	PangoRectangle text_extents = LABWC_src_common_font_c_font_extents(font, text);
	if (max_width > 0 && text_extents.width > max_width) {
		text_extents.width = max_width;
	}
	*width = text_extents.width;
	*height = text_extents.height;
}
void
font_buffer_create(struct lab_data_buffer **buffer, int max_width,
	int height, const char *text, struct font *font, const float *color,
	cairo_pattern_t *bg_pattern, double scale, bool use_markup)
{
	if (string_null_or_empty(text)) {
		*buffer = NULL;
		return;
	}
	int width, computed_height;
	font_get_buffer_size(max_width, text, font, &width, &computed_height);
	if (height <= 0) {
		height = computed_height;
	}
	if (height <= 0 || width <= 0) {
		*buffer = NULL;
		return;
	}
	*buffer = buffer_create_cairo((uint32_t)width, (uint32_t)height, (float)scale);
	if (!*buffer) {
		return;
	}
	cairo_surface_t *surf = (*buffer)->surface;
	cairo_t *cairo = cairo_create(surf);
	bool opaque_bg = is_pattern_opaque(bg_pattern);
	if (opaque_bg) {
		cairo_set_source(cairo, bg_pattern);
		cairo_paint(cairo);
	}
	set_cairo_color(cairo, color);
	cairo_move_to(cairo, 0, (height - computed_height) / 2);
	PangoLayout *layout = pango_cairo_create_layout(cairo);
	pango_context_set_round_glyph_positions(pango_layout_get_context(layout), false);
	pango_layout_set_width(layout, width * PANGO_SCALE);
	if (use_markup) {
		pango_layout_set_markup(layout, text, -1);
	} else {
		pango_layout_set_text(layout, text, -1);
	}
	pango_layout_set_ellipsize(layout, PANGO_ELLIPSIZE_END);
	if (!opaque_bg) {
		cairo_font_options_t *opts = cairo_font_options_create();
		cairo_font_options_set_antialias(opts, CAIRO_ANTIALIAS_GRAY);
		PangoContext *ctx = pango_layout_get_context(layout);
		pango_cairo_context_set_font_options(ctx, opts);
		cairo_font_options_destroy(opts);
	}
	PangoFontDescription *desc = font_to_pango_desc(font);
	pango_layout_set_font_description(layout, desc);
	pango_font_description_free(desc);
	pango_cairo_update_layout(cairo, layout);
	pango_cairo_show_layout(cairo, layout);
	g_object_unref(layout);
	cairo_surface_flush(surf);
	cairo_destroy(cairo);
}
void
font_finish(void)
{
	pango_cairo_font_map_set_default(NULL);
}
bool
lab_set_contains(struct lab_set *set, uint32_t value)
{
	for (int i = 0; i < set->size; ++i) {
		if (set->values[i] == value) {
			return true;
		}
	}
	return false;
}
void
lab_set_add(struct lab_set *set, uint32_t value)
{
	if (lab_set_contains(set, value)) {
		return;
	}
	if (set->size >= LAB_SET_MAX_SIZE) {
		return;
	}
	set->values[set->size++] = value;
}
void
lab_set_remove(struct lab_set *set, uint32_t value)
{
	for (int i = 0; i < set->size; ++i) {
		if (set->values[i] == value) {
			--set->size;
			set->values[i] = set->values[set->size];
			return;
		}
	}
}
#include <assert.h>
#if HAVE_XWAYLAND
#endif
void
desktop_arrange_all_views(void)
{
	struct view *view;
	wl_list_for_each(view, &server.views, link) {
		if (!wlr_box_empty(&view->pending)) {
			view_adjust_for_layout_change(view);
		}
	}
}
static void
LABWC_src_desktop_c_set_or_offer_focus(struct view *view)
{
	struct seat *seat = &server.seat;
	switch (view_wants_focus(view)) {
	case VIEW_WANTS_FOCUS_ALWAYS:
		if (view->surface != seat->wlr_seat->keyboard_state.focused_surface) {
			seat_focus_surface(seat, view->surface);
		}
		break;
	case VIEW_WANTS_FOCUS_LIKELY:
	case VIEW_WANTS_FOCUS_UNLIKELY:
		if (view->surface != seat->wlr_seat->keyboard_state.focused_surface) {
			view_offer_focus(view);
		}
		break;
	case VIEW_WANTS_FOCUS_NEVER:
		break;
	}
}
static int
LABWC_src_desktop_c_handle_auto_raise_timer(void *data)
{
	(void)data;
	struct view *view = server.pending_auto_raise_view;
	server.pending_auto_raise_view = NULL;
	if (view && view->mapped) {
		view_move_to_front(view);
	}
	return 0;
}
void
desktop_cancel_pending_auto_raise(void)
{
	server.pending_auto_raise_view = NULL;
	if (server.pending_auto_raise_timer) {
		wl_event_source_timer_update(server.pending_auto_raise_timer, 0);
	}
}
static void
LABWC_src_desktop_c_schedule_delayed_auto_raise(struct view *view)
{
	server.pending_auto_raise_view = view;
	if (!server.pending_auto_raise_timer) {
		server.pending_auto_raise_timer =
			wl_event_loop_add_timer(server.wl_event_loop,
				LABWC_src_desktop_c_handle_auto_raise_timer, NULL);
	}
	wl_event_source_timer_update(server.pending_auto_raise_timer,
		(int)rc.raise_on_focus_delay_ms);
}
static void
LABWC_src_desktop_c_desktop_focus_view_internal(struct view *view, bool raise, bool allow_delay)
{
	assert(view);
	if (!view->surface) {
		return;
	}
	if (false) {
		return;
	}
	if (view->minimized) {
		view_minimize(view, false);
		return;
	}
	if (!view->mapped) {
		return;
	}
	if (!view->visible_on_all_workspaces) {
		workspaces_switch_to(view->workspace,   false);
	}
	desktop_cancel_pending_auto_raise();
	if (raise) {
		if (allow_delay && rc.raise_on_focus_delay_ms > 0) {
			LABWC_src_desktop_c_schedule_delayed_auto_raise(view);
		} else {
			view_move_to_front(view);
		}
	}
	struct view *dialog = view_get_modal_dialog(view);
	LABWC_src_desktop_c_set_or_offer_focus(dialog ? dialog : view);
	show_desktop_reset();
}
void
desktop_focus_view(struct view *view, bool raise)
{
	LABWC_src_desktop_c_desktop_focus_view_internal(view, raise,   false);
}
void
desktop_focus_view_or_surface(struct seat *seat, struct view *view,
		struct wlr_surface *surface, bool raise)
{
	assert(view || surface);
	if (view) {
		LABWC_src_desktop_c_desktop_focus_view_internal(view, raise,   true);
#if HAVE_XWAYLAND
	} else {
		struct wlr_xwayland_surface *xsurface =
			wlr_xwayland_surface_try_from_wlr_surface(surface);
		if (xsurface && wlr_xwayland_surface_override_redirect_wants_focus(xsurface)) {
			seat_focus_surface(seat, surface);
		}
#endif
	}
}
static struct view *
LABWC_src_desktop_c_desktop_topmost_focusable_view(void)
{
	struct view *view;
	for_each_view(view, &server.views,
			LAB_VIEW_CRITERIA_CURRENT_WORKSPACE) {
		if (!view->minimized) {
			return view;
		}
	}
	return NULL;
}
void
desktop_focus_topmost_view(void)
{
	struct view *view = LABWC_src_desktop_c_desktop_topmost_focusable_view();
	if (view) {
		desktop_focus_view(view,   true);
	} else {
		seat_focus_surface(&server.seat, NULL);
	}
}
void
desktop_focus_output(struct output *output)
{
	if (!output_is_usable(output) || server.input_mode
			!= LAB_INPUT_STATE_PASSTHROUGH) {
		return;
	}
	struct view *view;
	for_each_view(view, &server.views, LAB_VIEW_CRITERIA_CURRENT_WORKSPACE) {
		if (view->outputs & output->id_bit) {
			desktop_focus_view(view,   false);
			wlr_cursor_warp(server.seat.cursor, NULL,
				view->current.x + view->current.width / 2,
				view->current.y + view->current.height / 2);
			cursor_update_focus();
			return;
		}
	}
	struct wlr_box layout_box;
	wlr_output_layout_get_box(server.output_layout,
		output->wlr_output, &layout_box);
	wlr_cursor_warp(server.seat.cursor, NULL,
		layout_box.x + output->usable_area.x + output->usable_area.width / 2,
		layout_box.y + output->usable_area.y + output->usable_area.height / 2);
	cursor_update_focus();
}
void
desktop_update_top_layer_visibility(void)
{
	struct view *view;
	struct output *output;
	uint32_t top = ZWLR_LAYER_SHELL_V1_LAYER_TOP;
	wl_list_for_each(output, &server.outputs, link) {
		if (!output_is_usable(output)) {
			continue;
		}
		wlr_scene_node_set_enabled(&output->layer_tree[top]->node, true);
	}
	uint64_t outputs_covered = 0;
	for_each_view(view, &server.views, LAB_VIEW_CRITERIA_CURRENT_WORKSPACE) {
		if (view->minimized) {
			continue;
		}
		if (!output_is_usable(view->output)) {
			continue;
		}
		if (view->fullscreen && !(view->outputs & outputs_covered)) {
			wlr_scene_node_set_enabled(
				&view->output->layer_tree[top]->node, false);
		}
		outputs_covered |= view->outputs;
	}
}
static void
LABWC_src_desktop_c_avoid_edge_rounding_issues(struct cursor_context *ctx)
{
	if (!ctx->surface) {
		return;
	}
	int w = ctx->surface->current.width;
	int h = ctx->surface->current.height;
	if (ctx->sx > w - 1 && ctx->sx < w) {
		ctx->sx = w - 1;
	}
	if (ctx->sy > h - 1 && ctx->sy < h) {
		ctx->sy = h - 1;
	}
}
struct cursor_context
get_cursor_context(void)
{
	struct cursor_context ret = {.type = LAB_NODE_NONE};
	struct wlr_cursor *cursor = server.seat.cursor;
	if (server.seat.drag.active) {
		dnd_icons_show(&server.seat, false);
	}
	struct wlr_scene_node *node =
		wlr_scene_node_at(&server.scene->tree.node,
			cursor->x, cursor->y, &ret.sx, &ret.sy);
	if (server.seat.drag.active) {
		dnd_icons_show(&server.seat, true);
	}
	if (!node) {
		ret.type = LAB_NODE_ROOT;
		return ret;
	}
	ret.node = node;
	ret.surface = lab_wlr_surface_from_node(node);
	LABWC_src_desktop_c_avoid_edge_rounding_issues(&ret);
#if HAVE_XWAYLAND
	if (node->type == WLR_SCENE_NODE_BUFFER) {
		if (node->parent == server.unmanaged_tree) {
			ret.type = LAB_NODE_UNMANAGED;
			return ret;
		}
	}
#endif
	while (node) {
		struct node_descriptor *desc = node->data;
		if (desc) {
			switch (desc->type) {
			case LAB_NODE_VIEW:
			case LAB_NODE_XDG_POPUP:
				ret.view = desc->view;
				if (ret.surface) {
					ret.type = LAB_NODE_CLIENT;
				} else {
					ret.type = LAB_NODE_NONE;
				}
				return ret;
			case LAB_NODE_LAYER_SURFACE:
				ret.type = LAB_NODE_LAYER_SURFACE;
				return ret;
			case LAB_NODE_LAYER_POPUP:
			case LAB_NODE_SESSION_LOCK_SURFACE:
			case LAB_NODE_IME_POPUP:
				ret.type = LAB_NODE_CLIENT;
				return ret;
			case LAB_NODE_MENUITEM:
				ret.node = node;
				ret.type = LAB_NODE_MENUITEM;
				return ret;
			case LAB_NODE_BUTTON_FIRST...LAB_NODE_BUTTON_LAST:
			case LAB_NODE_SSD_ROOT:
			case LAB_NODE_TITLEBAR:
				ret.node = node;
				ret.view = desc->view;
				assert(ret.view);
				ret.type = ssd_get_resizing_type(ret.view->ssd, cursor);
				if (ret.type == LAB_NODE_NONE) {
					ret.type = desc->type;
				}
				return ret;
			default:
				break;
			}
		}
		node = node->parent ? &node->parent->node : NULL;
	}
	return ret;
}
#include <assert.h>
static void
LABWC_src_dnd_c_handle_drag_request(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, drag.events.request);
	struct wlr_seat_request_start_drag_event *event = data;
	if (wlr_seat_validate_pointer_grab_serial(
			seat->wlr_seat, event->origin, event->serial)) {
		wlr_seat_start_pointer_drag(seat->wlr_seat, event->drag,
			event->serial);
	} else {
		wlr_data_source_destroy(event->drag->source);
	}
}
static void
LABWC_src_dnd_c_handle_drag_start(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, drag.events.start);
	assert(!seat->drag.active);
	struct wlr_drag *drag = data;
	seat->drag.active = true;
	cursor_context_save(&seat->pressed, NULL);
	if (drag->icon) {
		wlr_scene_drag_icon_create(seat->drag.icons, drag->icon);
		wlr_scene_node_raise_to_top(&seat->drag.icons->node);
		wlr_scene_node_set_enabled(&seat->drag.icons->node, true);
	}
	wl_signal_add(&drag->events.destroy, &seat->drag.events.destroy);
}
static void
LABWC_src_dnd_c_handle_drag_destroy(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, drag.events.destroy);
	assert(seat->drag.active);
	seat->drag.active = false;
	wl_list_remove(&seat->drag.events.destroy.link);
	wlr_scene_node_set_enabled(&seat->drag.icons->node, false);
	if (!rc.focus_follow_mouse) {
		return;
	}
	struct cursor_context ctx = get_cursor_context();
	if (!ctx.surface) {
		return;
	}
	seat_focus_surface(seat, NULL);
	seat_focus_surface(seat, ctx.surface);
	if (ctx.view && rc.raise_on_focus) {
		view_move_to_front(ctx.view);
	}
}
void
dnd_init(struct seat *seat)
{
	seat->drag.icons = lab_wlr_scene_tree_create(&server.scene->tree);
	wlr_scene_node_set_enabled(&seat->drag.icons->node, false);
	seat->drag.events.request.notify = LABWC_src_dnd_c_handle_drag_request;
	seat->drag.events.start.notify = LABWC_src_dnd_c_handle_drag_start;
	seat->drag.events.destroy.notify = LABWC_src_dnd_c_handle_drag_destroy;
	wl_signal_add(&seat->wlr_seat->events.request_start_drag,
		&seat->drag.events.request);
	wl_signal_add(&seat->wlr_seat->events.start_drag, &seat->drag.events.start);
}
void
dnd_icons_show(struct seat *seat, bool show)
{
	wlr_scene_node_set_enabled(&seat->drag.icons->node, show);
}
void
dnd_icons_move(struct seat *seat, double x, double y)
{
	wlr_scene_node_set_position(&seat->drag.icons->node, (int)x, (int)y);
}
void dnd_finish(struct seat *seat)
{
	wlr_scene_node_destroy(&seat->drag.icons->node);
	wl_list_remove(&seat->drag.events.request.link);
	wl_list_remove(&seat->drag.events.start.link);
}
#include <assert.h>
#include <limits.h>
#include <pixman.h>
static void
LABWC_src_edges_c_edges_for_target_geometry(struct border *edges, struct view *view,
		struct wlr_box target)
{
	struct border border = ssd_get_margin(view->ssd);
	int eff_height = view->shaded ? 0 : target.height;
	edges->left = target.x - border.left - rc.gap;
	edges->top = target.y - border.top - rc.gap;
	edges->right = target.x + target.width + border.right + rc.gap;
	edges->bottom = target.y + eff_height + border.bottom + rc.gap;
}
void
edges_initialize(struct border *edges)
{
	assert(edges);
	edges->top = INT_MIN;
	edges->right = INT_MAX;
	edges->bottom = INT_MAX;
	edges->left = INT_MIN;
}
static inline struct edge
LABWC_src_edges_c_build_edge(struct border region, enum lab_edge direction, int pad)
{
	struct edge edge = { 0 };
	switch (direction) {
	case LAB_EDGE_LEFT:
		edge.offset = clipped_sub(region.left, pad);
		edge.min = region.top;
		edge.max = region.bottom;
		break;
	case LAB_EDGE_RIGHT:
		edge.offset = clipped_add(region.right, pad);
		edge.min = region.top;
		edge.max = region.bottom;
		break;
	case LAB_EDGE_TOP:
		edge.offset = clipped_sub(region.top, pad);
		edge.min = region.left;
		edge.max = region.right;
		break;
	case LAB_EDGE_BOTTOM:
		edge.offset = clipped_add(region.bottom, pad);
		edge.min = region.left;
		edge.max = region.right;
		break;
	default:
		abort();
	}
	return edge;
}
static inline bool
LABWC_src_edges_c_is_lesser(enum lab_edge direction)
{
	return direction == LAB_EDGE_LEFT || direction == LAB_EDGE_TOP;
}
static inline struct edge
LABWC_src_edges_c_build_visible_edge(struct border region, enum lab_edge direction,
		int pad, enum lab_edge edges_visible)
{
	struct edge edge = LABWC_src_edges_c_build_edge(region, direction, pad);
	if (!(edges_visible & direction)) {
		edge.offset = LABWC_src_edges_c_is_lesser(direction) ? INT_MIN : INT_MAX;
	}
	return edge;
}
static void
LABWC_src_edges_c_validate_single_region_edge(int *valid_edge,
		struct border view, struct border target,
		struct border region, edge_validator_t validator,
		enum lab_edge direction, enum lab_edge edges_visible)
{
	enum lab_edge opposing = lab_edge_invert(direction);
	assert(opposing != LAB_EDGE_NONE);
	validator(valid_edge,
		LABWC_src_edges_c_build_edge(view, direction, 0),
		LABWC_src_edges_c_build_edge(target, direction, 0),
		LABWC_src_edges_c_build_visible_edge(region, opposing, 0, edges_visible),
		LABWC_src_edges_c_build_visible_edge(region, direction, rc.gap, edges_visible),
		LABWC_src_edges_c_is_lesser(direction));
}
static void
LABWC_src_edges_c_validate_edges(struct border *valid_edges,
		struct border view, struct border target,
		struct border region, enum lab_edge edges_visible,
		edge_validator_t validator)
{
	LABWC_src_edges_c_validate_single_region_edge(&valid_edges->left,
		view, target, region, validator, LAB_EDGE_LEFT, edges_visible);
	LABWC_src_edges_c_validate_single_region_edge(&valid_edges->right,
		view, target, region, validator, LAB_EDGE_RIGHT, edges_visible);
	LABWC_src_edges_c_validate_single_region_edge(&valid_edges->top,
		view, target, region, validator, LAB_EDGE_TOP, edges_visible);
	LABWC_src_edges_c_validate_single_region_edge(&valid_edges->bottom,
		view, target, region, validator, LAB_EDGE_BOTTOM, edges_visible);
}
static void
LABWC_src_edges_c_validate_single_output_edge(int *valid_edge,
		struct border view, struct border target,
		struct border region, edge_validator_t validator,
		enum lab_edge direction)
{
	static struct border unbounded = {
		.top = INT_MIN,
		.right = INT_MAX,
		.bottom = INT_MAX,
		.left = INT_MIN,
	};
	validator(valid_edge,
		LABWC_src_edges_c_build_edge(view, direction, 0),
		LABWC_src_edges_c_build_edge(target, direction, 0),
		LABWC_src_edges_c_build_edge(region, direction, 0),
		LABWC_src_edges_c_build_edge(unbounded, direction, 0), LABWC_src_edges_c_is_lesser(direction));
}
static void
LABWC_src_edges_c_validate_output_edges(struct border *valid_edges,
		struct border view, struct border target,
		struct wlr_box usable, edge_validator_t validator)
{
	struct border output = {
		.top = usable.y,
		.right = usable.x + usable.width,
		.bottom = usable.y + usable.height,
		.left = usable.x,
	};
	LABWC_src_edges_c_validate_single_output_edge(&valid_edges->left,
		view, target, output, validator, LAB_EDGE_LEFT);
	LABWC_src_edges_c_validate_single_output_edge(&valid_edges->right,
		view, target, output, validator, LAB_EDGE_RIGHT);
	LABWC_src_edges_c_validate_single_output_edge(&valid_edges->top,
		view, target, output, validator, LAB_EDGE_TOP);
	LABWC_src_edges_c_validate_single_output_edge(&valid_edges->bottom,
		view, target, output, validator, LAB_EDGE_BOTTOM);
}
static enum lab_edge
LABWC_src_edges_c_compute_edges_visible(const struct wlr_box *view_size,
		const pixman_box32_t *view_rect,
		const pixman_region32_t *available)
{
	pixman_region32_t intersection;
	pixman_region32_init(&intersection);
	pixman_region32_intersect_rect(&intersection, available,
		view_size->x, view_size->y,
		(unsigned int)view_size->width,
		(unsigned int)view_size->height);
	int nrects;
	const pixman_box32_t *rects =
		pixman_region32_rectangles(&intersection, &nrects);
	enum lab_edge edges_visible = LAB_EDGE_NONE;
	for (int i = 0; i < nrects; i++) {
		if (rects[i].x1 == view_rect->x1) {
			edges_visible |= LAB_EDGE_LEFT;
		}
		if (rects[i].y1 == view_rect->y1) {
			edges_visible |= LAB_EDGE_TOP;
		}
		if (rects[i].x2 == view_rect->x2) {
			edges_visible |= LAB_EDGE_RIGHT;
		}
		if (rects[i].y2 == view_rect->y2) {
			edges_visible |= LAB_EDGE_BOTTOM;
		}
	}
	pixman_region32_fini(&intersection);
	return edges_visible;
}
static void
LABWC_src_edges_c_subtract_view_from_space(struct view *view, pixman_region32_t *available)
{
	struct wlr_box view_size = ssd_max_extents(view);
	pixman_box32_t view_rect = {
		.x1 = view_size.x,
		.y1 = view_size.y,
		.x2 = view_size.x + view_size.width,
		.y2 = view_size.y + view_size.height
	};
	pixman_region_overlap_t overlap =
		pixman_region32_contains_rectangle(available, &view_rect);
	switch (overlap) {
	case PIXMAN_REGION_IN:
		view->edges_visible = LAB_EDGES_ALL;
		break;
	case PIXMAN_REGION_OUT:
		view->edges_visible = LAB_EDGE_NONE;
		return;
	case PIXMAN_REGION_PART:
		view->edges_visible = LABWC_src_edges_c_compute_edges_visible(
			&view_size, &view_rect, available);
		break;
	}
	pixman_region32_t view_region;
	pixman_region32_init_rects(&view_region, &view_rect, 1);
	pixman_region32_subtract(available, available, &view_region);
	pixman_region32_fini(&view_region);
}
static void
LABWC_src_edges_c_subtract_node_tree(struct wlr_scene_tree *tree, pixman_region32_t *available,
		struct view *ignored_view)
{
	struct view *view;
	struct wlr_scene_node *node;
	struct node_descriptor *node_desc;
	wl_list_for_each_reverse(node, &tree->children, link) {
		if (!node->enabled) {
			continue;
		}
		node_desc = node->data;
		if (node_desc && node_desc->type == LAB_NODE_VIEW) {
			view = node_view_from_node(node);
			if (view != ignored_view) {
				LABWC_src_edges_c_subtract_view_from_space(view, available);
			}
		} else if (node->type == WLR_SCENE_NODE_TREE) {
			LABWC_src_edges_c_subtract_node_tree(wlr_scene_tree_from_node(node),
				available, ignored_view);
		}
	}
}
void
edges_calculate_visibility(struct view *ignored_view)
{
	pixman_region32_t region;
	pixman_region32_init(&region);
	struct output *output;
	struct wlr_box layout_box;
	wl_list_for_each(output, &server.outputs, link) {
		if (!output_is_usable(output)) {
			continue;
		}
		wlr_output_layout_get_box(server.output_layout,
			output->wlr_output, &layout_box);
		pixman_region32_union_rect(&region, &region,
			layout_box.x, layout_box.y,
			(unsigned int)layout_box.width,
			(unsigned int)layout_box.height);
	}
	LABWC_src_edges_c_subtract_node_tree(&server.scene->tree, &region, ignored_view);
	pixman_region32_fini(&region);
}
void
edges_find_neighbors(struct border *nearest_edges, struct view *view,
		struct wlr_box origin, struct wlr_box target,
		struct output *output, edge_validator_t validator, bool ignore_hidden)
{
	assert(view);
	assert(validator);
	assert(nearest_edges);
	if (!output_is_usable(view->output)) {
		return;
	}
	struct border view_edges = { 0 };
	struct border target_edges = { 0 };
	LABWC_src_edges_c_edges_for_target_geometry(&view_edges, view, origin);
	LABWC_src_edges_c_edges_for_target_geometry(&target_edges, view, target);
	struct view *v;
	for_each_view(v, &server.views, LAB_VIEW_CRITERIA_CURRENT_WORKSPACE) {
		if (v == view || v->minimized || !output_is_usable(v->output)) {
			continue;
		}
		enum lab_edge edges_visible =
			ignore_hidden ? v->edges_visible : LAB_EDGES_ALL;
		if (edges_visible == LAB_EDGE_NONE) {
			continue;
		}
		if (output && output != v->output && !view_on_output(v, output)) {
			continue;
		}
		if (view->output != v->output && !(view->outputs & v->outputs)) {
			continue;
		}
		struct border border = ssd_get_margin(v->ssd);
		struct border win_edges = {
			.top = v->current.y - border.top,
			.right = v->current.x + v->current.width + border.right,
			.bottom = v->current.y + border.bottom
				+ view_effective_height(v,   false),
			.left = v->current.x - border.left,
		};
		LABWC_src_edges_c_validate_edges(nearest_edges, view_edges,
			target_edges, win_edges, edges_visible, validator);
	}
}
void
edges_find_outputs(struct border *nearest_edges, struct view *view,
		struct wlr_box origin, struct wlr_box target,
		struct output *output, edge_validator_t validator)
{
	assert(view);
	assert(validator);
	assert(nearest_edges);
	if (!output_is_usable(view->output)) {
		return;
	}
	struct border view_edges = { 0 };
	struct border target_edges = { 0 };
	LABWC_src_edges_c_edges_for_target_geometry(&view_edges, view, origin);
	LABWC_src_edges_c_edges_for_target_geometry(&target_edges, view, target);
	struct output *o;
	wl_list_for_each(o, &server.outputs, link) {
		if (!output_is_usable(o)) {
			continue;
		}
		if (output && o != output) {
			continue;
		}
		struct wlr_box usable =
			output_usable_area_in_layout_coords(o);
		if (!box_intersects(&origin, &usable)
				&& !box_intersects(&target, &usable)) {
			continue;
		}
		LABWC_src_edges_c_validate_output_edges(nearest_edges,
			view_edges, target_edges, usable, validator);
	}
}
static void
LABWC_src_edges_c_adjust_move_coords_1d(int *edge, int lesser, int lesser_offset,
		int greater, int greater_offset, bool decreasing)
{
	int best = INT_MAX;
	if (BOUNDED_INT(lesser)) {
		best = clipped_add(lesser, lesser_offset);
	}
	if (BOUNDED_INT(greater)) {
		best = edge_get_best(best,
			clipped_sub(greater, greater_offset), decreasing);
	}
	if (BOUNDED_INT(best)) {
		*edge = best;
	}
}
void
edges_adjust_move_coords(struct view *view, struct border edges,
		int *x, int *y, bool use_pending)
{
	assert(view);
	struct border border = ssd_get_margin(view->ssd);
	struct wlr_box *view_geom =
		use_pending ? &view->pending : &view->current;
	if (view_geom->x != *x) {
		int lshift = border.left + rc.gap;
		int rshift = border.right + rc.gap + view_geom->width;
		LABWC_src_edges_c_adjust_move_coords_1d(x, edges.left, lshift,
			edges.right, rshift, *x < view_geom->x);
	}
	if (view_geom->y != *y) {
		int tshift = border.top + rc.gap;
		int bshift = border.bottom + rc.gap
			+ view_effective_height(view, use_pending);
		LABWC_src_edges_c_adjust_move_coords_1d(y, edges.top, tshift,
			edges.bottom, bshift, *y < view_geom->y);
	}
}
void
edges_adjust_resize_geom(struct view *view, struct border edges,
		enum lab_edge resize_edges, struct wlr_box *geom,
		bool use_pending)
{
	assert(view);
	struct border border = ssd_get_margin(view->ssd);
	struct wlr_box *view_geom =
		use_pending ? &view->pending : &view->current;
	if (resize_edges & LAB_EDGE_LEFT) {
		if (BOUNDED_INT(edges.left)) {
			geom->x = edges.left + border.left + rc.gap;
			geom->width = view_geom->width + view_geom->x - geom->x;
		}
	} else if (resize_edges & LAB_EDGE_RIGHT) {
		if (BOUNDED_INT(edges.right)) {
			geom->width = edges.right
				- view_geom->x - border.right - rc.gap;
		}
	}
	if (resize_edges & LAB_EDGE_TOP) {
		if (BOUNDED_INT(edges.top)) {
			geom->y = edges.top + border.top + rc.gap;
			geom->height = view_geom->height + view_geom->y - geom->y;
		}
	} else if (resize_edges & LAB_EDGE_BOTTOM) {
		if (BOUNDED_INT(edges.bottom)) {
			geom->height = edges.bottom
				- view_geom->y - border.bottom - rc.gap;
		}
	}
}
static double
LABWC_src_edges_c_linear_interp(int x, int x1, int y1, int x2, int y2)
{
	int rise = y2 - y1;
	if (rise == 0) {
		return y2;
	}
	int run = x2 - x1;
	if (run == 0) {
		return 0.5 * (y1 + y2);
	}
	int dx = x - x1;
	return y1 + dx * (rise / (double)run);
}
bool
edges_traverse_edge(struct edge current, struct edge target, struct edge obstacle)
{
	double lo =
		LABWC_src_edges_c_linear_interp(obstacle.offset,
			current.offset, current.min, target.offset, target.min);
	if (obstacle.max <= lo) {
		return false;
	}
	double hi =
		LABWC_src_edges_c_linear_interp(obstacle.offset,
			current.offset, current.max, target.offset, target.max);
	return obstacle.min < hi;
}
#include <assert.h>
#include <stdlib.h>
struct LABWC_src_idle_c_lab_idle_inhibitor {
	struct wlr_idle_inhibitor_v1 *wlr_inhibitor;
	struct wl_listener on_destroy;
};
struct LABWC_src_idle_c_lab_idle_manager {
	struct wlr_idle_notifier_v1 *ext;
	struct {
		struct wlr_idle_inhibit_manager_v1 *manager;
		struct wl_listener on_new_inhibitor;
		struct wl_listener on_destroy;
	} inhibitor;
};
static struct LABWC_src_idle_c_lab_idle_manager *LABWC_src_idle_c_manager;
static void
LABWC_src_idle_c_handle_idle_inhibitor_destroy(struct wl_listener *listener, void *data)
{
	struct LABWC_src_idle_c_lab_idle_inhibitor *idle_inhibitor = wl_container_of(listener,
		idle_inhibitor, on_destroy);
	if (LABWC_src_idle_c_manager) {
		bool still_inhibited =
			wl_list_length(&LABWC_src_idle_c_manager->inhibitor.manager->inhibitors) > 1;
		wlr_idle_notifier_v1_set_inhibited(LABWC_src_idle_c_manager->ext, still_inhibited);
	}
	wl_list_remove(&idle_inhibitor->on_destroy.link);
	free(idle_inhibitor);
}
static void
LABWC_src_idle_c_handle_idle_inhibitor_new(struct wl_listener *listener, void *data)
{
	assert(LABWC_src_idle_c_manager);
	struct wlr_idle_inhibitor_v1 *wlr_inhibitor = data;
	struct LABWC_src_idle_c_lab_idle_inhibitor *inhibitor = znew(*inhibitor);
	inhibitor->wlr_inhibitor = wlr_inhibitor;
	inhibitor->on_destroy.notify = LABWC_src_idle_c_handle_idle_inhibitor_destroy;
	wl_signal_add(&wlr_inhibitor->events.destroy, &inhibitor->on_destroy);
	wlr_idle_notifier_v1_set_inhibited(LABWC_src_idle_c_manager->ext, true);
}
static void
LABWC_src_idle_c_handle_inhibitor_manager_destroy(struct wl_listener *listener, void *data)
{
	wl_list_remove(&LABWC_src_idle_c_manager->inhibitor.on_new_inhibitor.link);
	wl_list_remove(&LABWC_src_idle_c_manager->inhibitor.on_destroy.link);
	zfree(LABWC_src_idle_c_manager);
}
void
idle_manager_create(struct wl_display *display)
{
	assert(!LABWC_src_idle_c_manager);
	LABWC_src_idle_c_manager = znew(*LABWC_src_idle_c_manager);
	LABWC_src_idle_c_manager->ext = wlr_idle_notifier_v1_create(display);
	LABWC_src_idle_c_manager->inhibitor.manager = wlr_idle_inhibit_v1_create(display);
	LABWC_src_idle_c_manager->inhibitor.on_new_inhibitor.notify = LABWC_src_idle_c_handle_idle_inhibitor_new;
	wl_signal_add(&LABWC_src_idle_c_manager->inhibitor.manager->events.new_inhibitor,
		&LABWC_src_idle_c_manager->inhibitor.on_new_inhibitor);
	LABWC_src_idle_c_manager->inhibitor.on_destroy.notify = LABWC_src_idle_c_handle_inhibitor_manager_destroy;
	wl_signal_add(&LABWC_src_idle_c_manager->inhibitor.manager->events.destroy,
		&LABWC_src_idle_c_manager->inhibitor.on_destroy);
}
void
idle_manager_notify_activity(struct wlr_seat *wlr_seat)
{
	if (!LABWC_src_idle_c_manager) {
		return;
	}
	wlr_idle_notifier_v1_notify_activity(LABWC_src_idle_c_manager->ext, wlr_seat);
}
#define handle_axis LABWC_src_input_cursor_c_handle_axis
#define handle_button LABWC_src_input_cursor_c_handle_button
#define handle_frame LABWC_src_input_cursor_c_handle_frame
#define handle_motion LABWC_src_input_cursor_c_handle_motion
#define handle_motion_absolute LABWC_src_input_cursor_c_handle_motion_absolute
#define handle_request_set_cursor LABWC_src_input_cursor_c_handle_request_set_cursor
#define handle_request_set_primary_selection LABWC_src_input_cursor_c_handle_request_set_primary_selection
#define handle_request_set_selection LABWC_src_input_cursor_c_handle_request_set_selection
#define handle_request_set_shape LABWC_src_input_cursor_c_handle_request_set_shape
#include <assert.h>
#include <time.h>
#if WLR_HAS_LIBINPUT_BACKEND
#endif
#define LAB_CURSOR_SHAPE_V1_VERSION 1
struct LABWC_src_input_cursor_c_constraint {
	struct seat *seat;
	struct wlr_pointer_constraint_v1 *constraint;
	struct wl_listener destroy;
};
static const char * const *LABWC_src_input_cursor_c_cursor_names = NULL;
static const char * const LABWC_src_input_cursor_c_cursors_xdg[] = {
	NULL,
	"default",
	"grab",
	"nw-resize",
	"n-resize",
	"ne-resize",
	"e-resize",
	"se-resize",
	"s-resize",
	"sw-resize",
	"w-resize"
};
static const char * const LABWC_src_input_cursor_c_cursors_x11[] = {
	NULL,
	"left_ptr",
	"grabbing",
	"top_left_corner",
	"top_side",
	"top_right_corner",
	"right_side",
	"bottom_right_corner",
	"bottom_side",
	"bottom_left_corner",
	"left_side"
};
static_assert(
	ARRAY_SIZE(LABWC_src_input_cursor_c_cursors_xdg) == LAB_CURSOR_COUNT,
	"XDG cursor names are out of sync");
static_assert(
	ARRAY_SIZE(LABWC_src_input_cursor_c_cursors_x11) == LAB_CURSOR_COUNT,
	"X11 cursor names are out of sync");
enum lab_cursors
cursor_get_from_edge(enum lab_edge resize_edges)
{
	switch (resize_edges) {
	case LAB_EDGES_TOP_LEFT:
		return LAB_CURSOR_RESIZE_NW;
	case LAB_EDGE_TOP:
		return LAB_CURSOR_RESIZE_N;
	case LAB_EDGES_TOP_RIGHT:
		return LAB_CURSOR_RESIZE_NE;
	case LAB_EDGE_RIGHT:
		return LAB_CURSOR_RESIZE_E;
	case LAB_EDGES_BOTTOM_RIGHT:
		return LAB_CURSOR_RESIZE_SE;
	case LAB_EDGE_BOTTOM:
		return LAB_CURSOR_RESIZE_S;
	case LAB_EDGES_BOTTOM_LEFT:
		return LAB_CURSOR_RESIZE_SW;
	case LAB_EDGE_LEFT:
		return LAB_CURSOR_RESIZE_W;
	default:
		return LAB_CURSOR_DEFAULT;
	}
}
static enum lab_cursors
LABWC_src_input_cursor_c_cursor_get_from_ssd(enum lab_node_type view_area)
{
	enum lab_edge resize_edges = node_type_to_edges(view_area);
	return cursor_get_from_edge(resize_edges);
}
static struct wlr_surface *
LABWC_src_input_cursor_c_get_toplevel(struct wlr_surface *surface)
{
	while (surface) {
		struct wlr_xdg_surface *xdg_surface =
			wlr_xdg_surface_try_from_wlr_surface(surface);
		if (!xdg_surface) {
			break;
		}
		switch (xdg_surface->role) {
		case WLR_XDG_SURFACE_ROLE_NONE:
			return NULL;
		case WLR_XDG_SURFACE_ROLE_TOPLEVEL:
			return surface;
		case WLR_XDG_SURFACE_ROLE_POPUP:
			surface = xdg_surface->popup->parent;
			continue;
		}
	}
	if (surface && wlr_layer_surface_v1_try_from_wlr_surface(surface)) {
		return surface;
	}
	return NULL;
}
static void
LABWC_src_input_cursor_c_handle_request_set_cursor(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, request_set_cursor);
	if (server.input_mode != LAB_INPUT_STATE_PASSTHROUGH) {
		return;
	}
	if (!seat->cursor_visible) {
		return;
	}
	struct wlr_seat_pointer_request_set_cursor_event *event = data;
	struct wlr_seat_client *focused_client =
		seat->wlr_seat->pointer_state.focused_client;
	if (focused_client == event->seat_client) {
		wlr_cursor_set_surface(seat->cursor, event->surface,
			event->hotspot_x, event->hotspot_y);
	}
}
static void
LABWC_src_input_cursor_c_handle_request_set_shape(struct wl_listener *listener, void *data)
{
	struct wlr_cursor_shape_manager_v1_request_set_shape_event *event = data;
	const char *shape_name = wlr_cursor_shape_v1_name(event->shape);
	struct seat *seat = wl_container_of(listener, seat, request_set_shape);
	struct wlr_seat_client *focused_client = seat->wlr_seat->pointer_state.focused_client;
	if (server.input_mode != LAB_INPUT_STATE_PASSTHROUGH) {
		return;
	}
	if (!seat->cursor_visible) {
		return;
	}
	if (event->seat_client != focused_client) {
		return;
	}
	wlr_cursor_set_xcursor(seat->cursor, seat->xcursor_manager, shape_name);
}
static void
LABWC_src_input_cursor_c_handle_request_set_selection(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(
		listener, seat, request_set_selection);
	struct wlr_seat_request_set_selection_event *event = data;
	wlr_seat_set_selection(seat->wlr_seat, event->source,
		event->serial);
}
static void
LABWC_src_input_cursor_c_handle_request_set_primary_selection(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(
		listener, seat, request_set_primary_selection);
	struct wlr_seat_request_set_primary_selection_event *event = data;
	wlr_seat_set_primary_selection(seat->wlr_seat, event->source,
		event->serial);
}
static void
LABWC_src_input_cursor_c_process_cursor_move(uint32_t time)
{
	struct view *view = server.grabbed_view;
	int x = (int)(server.grab_box.x
		+ (server.seat.cursor->x - server.grab_x));
	int y = (int)(server.grab_box.y
		+ (server.seat.cursor->y - server.grab_y));
	bool needs_untile = resistance_unsnap_apply(view, &x, &y);
	if (needs_untile) {
		struct wlr_box new_geo = {
			.width = view->natural_geometry.width,
			.height = view->natural_geometry.height,
		};
		interactive_anchor_to_cursor(&new_geo);
		view_set_shade(view, false);
		view_set_maximized(view, VIEW_AXIS_NONE);
		view_set_untiled(view);
		view_move_resize(view, new_geo);
		x = new_geo.x;
		y = new_geo.y;
	}
	resistance_move_apply(view, &x, &y);
	view_move(view, x, y);
	overlay_update(&server.seat);
}
static void
LABWC_src_input_cursor_c_process_cursor_resize(uint32_t time)
{
	static uint32_t last_resize_time = 0;
	static struct view *last_resize_view = NULL;
	assert(server.grabbed_view);
	if (server.grabbed_view == last_resize_view) {
		int32_t refresh = 0;
		if (output_is_usable(last_resize_view->output)) {
			refresh = last_resize_view->output->wlr_output->refresh;
		}
		if (refresh <= 0) {
			refresh = 250000;
		}
		if (time - last_resize_time < 1000000 / (uint32_t)refresh) {
			return;
		}
	}
	last_resize_time = time;
	last_resize_view = server.grabbed_view;
	double dx = server.seat.cursor->x - server.grab_x;
	double dy = server.seat.cursor->y - server.grab_y;
	struct view *view = server.grabbed_view;
	struct wlr_box new_view_geo = server.grab_box;
	if (server.resize_edges & LAB_EDGE_TOP) {
		new_view_geo.y = (int)(server.grab_box.y + dy);
		new_view_geo.height = (int)(server.grab_box.height - dy);
	} else if (server.resize_edges & LAB_EDGE_BOTTOM) {
		new_view_geo.height = (int)(server.grab_box.height + dy);
	}
	if (server.resize_edges & LAB_EDGE_LEFT) {
		new_view_geo.x = (int)(server.grab_box.x + dx);
		new_view_geo.width = (int)(server.grab_box.width - dx);
	} else if (server.resize_edges & LAB_EDGE_RIGHT) {
		new_view_geo.width = (int)(server.grab_box.width + dx);
	}
	resistance_resize_apply(view, &new_view_geo);
	view_adjust_size(view, &new_view_geo.width, &new_view_geo.height);
	if (server.resize_edges & LAB_EDGE_TOP) {
		new_view_geo.y = server.grab_box.y +
			server.grab_box.height - new_view_geo.height;
	}
	if (server.resize_edges & LAB_EDGE_LEFT) {
		new_view_geo.x = server.grab_box.x +
			server.grab_box.width - new_view_geo.width;
	}
	if (rc.resize_draw_contents) {
		view_move_resize(view, new_view_geo);
	} else {
		resize_outlines_update(view, new_view_geo);
	}
}
void
cursor_set(struct seat *seat, enum lab_cursors cursor)
{
	assert(cursor > LAB_CURSOR_CLIENT && cursor < LAB_CURSOR_COUNT);
	if (seat->server_cursor == cursor) {
		return;
	}
	if (seat->cursor_visible) {
		wlr_cursor_set_xcursor(seat->cursor, seat->xcursor_manager,
			LABWC_src_input_cursor_c_cursor_names[cursor]);
	}
	seat->server_cursor = cursor;
}
void
cursor_set_visible(struct seat *seat, bool visible)
{
	if (seat->cursor_visible == visible) {
		return;
	}
	seat->cursor_visible = visible;
	cursor_update_image(seat);
}
void
cursor_update_image(struct seat *seat)
{
	enum lab_cursors cursor = seat->server_cursor;
	if (!seat->cursor_visible) {
		wlr_cursor_unset_image(seat->cursor);
		return;
	}
	if (cursor == LAB_CURSOR_CLIENT) {
		if (seat->wlr_seat->pointer_state.focused_surface) {
			seat->server_cursor = LAB_CURSOR_DEFAULT;
			wlr_cursor_set_xcursor(seat->cursor, seat->xcursor_manager, "");
			wlr_seat_pointer_clear_focus(seat->wlr_seat);
			cursor_update_focus();
		}
		return;
	}
	wlr_cursor_unset_image(seat->cursor);
	wlr_cursor_set_xcursor(seat->cursor, seat->xcursor_manager,
		LABWC_src_input_cursor_c_cursor_names[cursor]);
}
static void
LABWC_src_input_cursor_c_clear_cursor_context(struct cursor_context_saved *saved_ctx)
{
	if (saved_ctx->node_destroy.notify) {
		wl_list_remove(&saved_ctx->node_destroy.link);
	}
	if (saved_ctx->surface_destroy.notify) {
		wl_list_remove(&saved_ctx->surface_destroy.link);
	}
	if (saved_ctx->view_destroy.notify) {
		wl_list_remove(&saved_ctx->view_destroy.link);
	}
	*saved_ctx = (struct cursor_context_saved) {0};
}
static void
LABWC_src_input_cursor_c_handle_ctx_node_destroy(struct wl_listener *listener, void *data)
{
	struct cursor_context_saved *saved_ctx =
		wl_container_of(listener, saved_ctx, node_destroy);
	LABWC_src_input_cursor_c_clear_cursor_context(saved_ctx);
}
static void
LABWC_src_input_cursor_c_handle_ctx_surface_destroy(struct wl_listener *listener, void *data)
{
	struct cursor_context_saved *saved_ctx =
		wl_container_of(listener, saved_ctx, surface_destroy);
	LABWC_src_input_cursor_c_clear_cursor_context(saved_ctx);
}
static void
LABWC_src_input_cursor_c_handle_ctx_view_destroy(struct wl_listener *listener, void *data)
{
	struct cursor_context_saved *saved_ctx =
		wl_container_of(listener, saved_ctx, view_destroy);
	LABWC_src_input_cursor_c_clear_cursor_context(saved_ctx);
}
void
cursor_context_save(struct cursor_context_saved *saved_ctx,
		const struct cursor_context *ctx)
{
	assert(saved_ctx);
	LABWC_src_input_cursor_c_clear_cursor_context(saved_ctx);
	if (!ctx) {
		return;
	}
	saved_ctx->ctx = *ctx;
	if (ctx->node) {
		saved_ctx->node_destroy.notify = LABWC_src_input_cursor_c_handle_ctx_node_destroy;
		wl_signal_add(&ctx->node->events.destroy, &saved_ctx->node_destroy);
	}
	if (ctx->surface) {
		saved_ctx->surface_destroy.notify = LABWC_src_input_cursor_c_handle_ctx_surface_destroy;
		wl_signal_add(&ctx->surface->events.destroy, &saved_ctx->surface_destroy);
	}
	if (ctx->view) {
		saved_ctx->view_destroy.notify = LABWC_src_input_cursor_c_handle_ctx_view_destroy;
		wl_signal_add(&ctx->view->events.destroy, &saved_ctx->view_destroy);
	}
}
static bool
LABWC_src_input_cursor_c_update_pressed_surface(struct seat *seat, const struct cursor_context *ctx)
{
	if (!wlr_seat_pointer_has_grab(seat->wlr_seat)) {
		return false;
	}
	if (seat->pressed.ctx.surface && ctx->surface != seat->pressed.ctx.surface) {
		struct wlr_surface *toplevel = LABWC_src_input_cursor_c_get_toplevel(ctx->surface);
		if (toplevel && toplevel == LABWC_src_input_cursor_c_get_toplevel(seat->pressed.ctx.surface)) {
			cursor_context_save(&seat->pressed, ctx);
			return true;
		}
	}
	return false;
}
static void
LABWC_src_input_cursor_c_cursor_update_common(const struct cursor_context *ctx,
		struct cursor_context *notified_ctx)
{
	struct seat *seat = &server.seat;
	struct wlr_seat *wlr_seat = seat->wlr_seat;
	ssd_update_hovered_button(ctx->node);
	if (server.input_mode != LAB_INPUT_STATE_PASSTHROUGH) {
		return;
	}
	if (seat->pressed.ctx.surface && ctx->surface != seat->pressed.ctx.surface
			&& !LABWC_src_input_cursor_c_update_pressed_surface(seat, ctx)
			&& !seat->drag.active) {
		if (notified_ctx) {
			int lx, ly;
			wlr_scene_node_coords(seat->pressed.ctx.node, &lx, &ly);
			*notified_ctx = seat->pressed.ctx;
			notified_ctx->sx = server.seat.cursor->x - lx;
			notified_ctx->sy = server.seat.cursor->y - ly;
		}
		return;
	}
	if (notified_ctx) {
		*notified_ctx = *ctx;
	}
	if (ctx->surface) {
		wlr_seat_pointer_notify_enter(wlr_seat, ctx->surface,
			ctx->sx, ctx->sy);
		seat->server_cursor = LAB_CURSOR_CLIENT;
	} else {
		wlr_seat_pointer_notify_clear_focus(wlr_seat);
		if (!seat->drag.active) {
			enum lab_cursors cursor = LABWC_src_input_cursor_c_cursor_get_from_ssd(ctx->type);
			if (ctx->view && ctx->view->shaded && cursor > LAB_CURSOR_GRAB) {
				cursor = LAB_CURSOR_DEFAULT;
			}
			cursor_set(seat, cursor);
		}
	}
}
enum lab_edge
cursor_get_resize_edges(struct wlr_cursor *cursor, const struct cursor_context *ctx)
{
	enum lab_edge resize_edges = node_type_to_edges(ctx->type);
	if (ctx->view && !resize_edges) {
		struct wlr_box box = ctx->view->current;
		resize_edges |=
			(int)cursor->x < box.x + box.width / 2 ?
				LAB_EDGE_LEFT : LAB_EDGE_RIGHT;
		resize_edges |=
			(int)cursor->y < box.y + box.height / 2 ?
				LAB_EDGE_TOP : LAB_EDGE_BOTTOM;
	}
	return resize_edges;
}
bool
cursor_process_motion(uint32_t time, double *sx, double *sy)
{
	if (server.input_mode == LAB_INPUT_STATE_MOVE) {
		LABWC_src_input_cursor_c_process_cursor_move(time);
		return false;
	} else if (server.input_mode == LAB_INPUT_STATE_RESIZE) {
		LABWC_src_input_cursor_c_process_cursor_resize(time);
		return false;
	}
	struct cursor_context ctx = get_cursor_context();
	struct seat *seat = &server.seat;
	if (ctx.type == LAB_NODE_MENUITEM) {
		menu_process_cursor_motion(ctx.node);
		cursor_set(&server.seat, LAB_CURSOR_DEFAULT);
		return false;
	}
	if (seat->drag.active) {
		dnd_icons_move(seat, seat->cursor->x, seat->cursor->y);
	}
	struct mousebind *mousebind;
	wl_list_for_each(mousebind, &rc.mousebinds, link) {
		if (ctx.type == LAB_NODE_CLIENT
				&& view_inhibits_actions(ctx.view, &mousebind->actions)) {
			continue;
		}
		if (mousebind->mouse_event == MOUSE_ACTION_DRAG
				&& mousebind->pressed_in_context) {
			mousebind->pressed_in_context = false;
			actions_run(seat->pressed.ctx.view,
				&mousebind->actions, &seat->pressed.ctx);
		}
	}
	struct cursor_context notified_ctx = {0};
	LABWC_src_input_cursor_c_cursor_update_common(&ctx, &notified_ctx);
	if (rc.focus_follow_mouse) {
		bool entering = false;
		if (notified_ctx.view) {
			entering = notified_ctx.view
					!= seat->last_cursor_ctx.ctx.view;
		} else if (notified_ctx.surface) {
			entering = notified_ctx.surface
					!= seat->last_cursor_ctx.ctx.surface;
		}
		if (entering) {
			desktop_focus_view_or_surface(seat, notified_ctx.view,
				notified_ctx.surface, rc.raise_on_focus);
		}
	}
	cursor_context_save(&seat->last_cursor_ctx, &notified_ctx);
	*sx = notified_ctx.sx;
	*sy = notified_ctx.sy;
	return notified_ctx.surface;
}
static void
LABWC_src_input_cursor_c__cursor_update_focus(void)
{
	struct cursor_context ctx = get_cursor_context();
	if ((ctx.view || ctx.surface) && rc.focus_follow_mouse
			&& !rc.focus_follow_mouse_requires_movement) {
		desktop_focus_view_or_surface(&server.seat, ctx.view,
			ctx.surface, rc.raise_on_focus);
	}
	LABWC_src_input_cursor_c_cursor_update_common(&ctx, NULL);
}
void
cursor_update_focus(void)
{
	static bool updating_focus = false;
	if (!updating_focus) {
		updating_focus = true;
		LABWC_src_input_cursor_c__cursor_update_focus();
		updating_focus = false;
	}
}
static void
LABWC_src_input_cursor_c_warp_cursor_to_constraint_hint(struct seat *seat,
		struct wlr_pointer_constraint_v1 *constraint)
{
	if (!server.active_view) {
		return;
	}
	if (constraint->current.committed
			& WLR_POINTER_CONSTRAINT_V1_STATE_CURSOR_HINT) {
		double sx = constraint->current.cursor_hint.x;
		double sy = constraint->current.cursor_hint.y;
		wlr_cursor_warp(seat->cursor, NULL,
			server.active_view->current.x + sx,
			server.active_view->current.y + sy);
		wlr_seat_pointer_warp(seat->wlr_seat, sx, sy);
	}
}
static void
LABWC_src_input_cursor_c_handle_constraint_commit(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, constraint_commit);
	struct wlr_pointer_constraint_v1 *constraint = seat->current_constraint;
	(void)constraint;
	assert(constraint);
}
static void
LABWC_src_input_cursor_c_handle_constraint_destroy(struct wl_listener *listener, void *data)
{
	struct LABWC_src_input_cursor_c_constraint *constraint = wl_container_of(listener, constraint,
		destroy);
	struct wlr_pointer_constraint_v1 *wlr_constraint = constraint->constraint;
	struct seat *seat = constraint->seat;
	wl_list_remove(&constraint->destroy.link);
	if (seat->current_constraint == wlr_constraint) {
		LABWC_src_input_cursor_c_warp_cursor_to_constraint_hint(seat, wlr_constraint);
		if (seat->constraint_commit.link.next) {
			wl_list_remove(&seat->constraint_commit.link);
		}
		wl_list_init(&seat->constraint_commit.link);
		seat->current_constraint = NULL;
	}
	free(constraint);
}
void
create_constraint(struct wl_listener *listener, void *data)
{
	struct wlr_pointer_constraint_v1 *wlr_constraint = data;
	struct LABWC_src_input_cursor_c_constraint *constraint = znew(*constraint);
	constraint->constraint = wlr_constraint;
	constraint->seat = &server.seat;
	constraint->destroy.notify = LABWC_src_input_cursor_c_handle_constraint_destroy;
	wl_signal_add(&wlr_constraint->events.destroy, &constraint->destroy);
	struct view *view = server.active_view;
	if (view && view->surface == wlr_constraint->surface) {
		constrain_cursor(wlr_constraint);
	}
}
void
constrain_cursor(struct wlr_pointer_constraint_v1
		*constraint)
{
	struct seat *seat = &server.seat;
	if (seat->current_constraint == constraint) {
		return;
	}
	wl_list_remove(&seat->constraint_commit.link);
	if (seat->current_constraint) {
		if (!constraint) {
			LABWC_src_input_cursor_c_warp_cursor_to_constraint_hint(seat, seat->current_constraint);
		}
		wlr_pointer_constraint_v1_send_deactivated(
			seat->current_constraint);
	}
	seat->current_constraint = constraint;
	if (!constraint) {
		wl_list_init(&seat->constraint_commit.link);
		return;
	}
	wlr_pointer_constraint_v1_send_activated(constraint);
	seat->constraint_commit.notify = LABWC_src_input_cursor_c_handle_constraint_commit;
	wl_signal_add(&constraint->surface->events.commit,
		&seat->constraint_commit);
}
static void
LABWC_src_input_cursor_c_apply_constraint(struct seat *seat, struct wlr_pointer *pointer, double *x, double *y)
{
	if (!server.active_view) {
		return;
	}
	if (!seat->current_constraint
			|| pointer->base.type != WLR_INPUT_DEVICE_POINTER
			|| seat->current_constraint->type
				!= WLR_POINTER_CONSTRAINT_V1_CONFINED) {
		return;
	}
	double sx = seat->cursor->x;
	double sy = seat->cursor->y;
	sx -= server.active_view->current.x;
	sy -= server.active_view->current.y;
	double sx_confined, sy_confined;
	if (!wlr_region_confine(&seat->current_constraint->region, sx, sy,
			sx + *x, sy + *y, &sx_confined, &sy_confined)) {
		return;
	}
	*x = sx_confined - sx;
	*y = sy_confined - sy;
}
static bool
LABWC_src_input_cursor_c_cursor_locked(struct seat *seat, struct wlr_pointer *pointer)
{
	return seat->current_constraint
		&& pointer->base.type == WLR_INPUT_DEVICE_POINTER
		&& seat->current_constraint->type == WLR_POINTER_CONSTRAINT_V1_LOCKED
		&& seat->current_constraint->surface
			== seat->wlr_seat->pointer_state.focused_surface;
}
static void
LABWC_src_input_cursor_c_preprocess_cursor_motion(struct seat *seat, struct wlr_pointer *pointer,
		uint32_t time_msec, double dx, double dy)
{
	if (LABWC_src_input_cursor_c_cursor_locked(seat, pointer)) {
		return;
	}
	LABWC_src_input_cursor_c_apply_constraint(seat, pointer, &dx, &dy);
	wlr_cursor_move(seat->cursor, &pointer->base, dx, dy);
	double sx, sy;
	bool notify = cursor_process_motion(time_msec, &sx, &sy);
	if (notify) {
		wlr_seat_pointer_notify_motion(seat->wlr_seat, time_msec, sx, sy);
	}
}
static double LABWC_src_input_cursor_c_get_natural_scroll_factor(struct wlr_input_device *wlr_input_device)
{
#if WLR_HAS_LIBINPUT_BACKEND
	if (wlr_input_device_is_libinput(wlr_input_device)) {
		struct libinput_device *libinput_device =
			wlr_libinput_get_device_handle(wlr_input_device);
		if (libinput_device_config_scroll_get_natural_scroll_enabled(libinput_device)) {
			return -1.0;
		}
	}
#endif
	return 1.0;
}
static void
LABWC_src_input_cursor_c_handle_motion(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, on_cursor.motion);
	struct wlr_pointer_motion_event *event = data;
	idle_manager_notify_activity(seat->wlr_seat);
	cursor_set_visible(seat,   true);
	if (seat->cursor_scroll_wheel_emulation) {
		enum wl_pointer_axis orientation;
		double delta;
		if (fabs(event->delta_x) > fabs(event->delta_y)) {
			orientation = WL_POINTER_AXIS_HORIZONTAL_SCROLL;
			delta = event->delta_x;
		} else {
			orientation = WL_POINTER_AXIS_VERTICAL_SCROLL;
			delta = event->delta_y;
		}
		double motion_to_scroll_factor = 0.04;
		double scroll_factor = motion_to_scroll_factor *
			LABWC_src_input_cursor_c_get_natural_scroll_factor(&event->pointer->base);
		double pointer_axis_step = 15.0;
		cursor_emulate_axis(seat, &event->pointer->base,
			orientation,
			pointer_axis_step * scroll_factor * delta, 0,
			WL_POINTER_AXIS_SOURCE_CONTINUOUS, event->time_msec);
	} else {
		wlr_relative_pointer_manager_v1_send_relative_motion(
			server.relative_pointer_manager,
			seat->wlr_seat, (uint64_t)event->time_msec * 1000,
			event->delta_x, event->delta_y, event->unaccel_dx,
			event->unaccel_dy);
		LABWC_src_input_cursor_c_preprocess_cursor_motion(seat, event->pointer,
			event->time_msec, event->delta_x, event->delta_y);
	}
}
static void
LABWC_src_input_cursor_c_handle_motion_absolute(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, on_cursor.motion_absolute);
	struct wlr_pointer_motion_absolute_event *event = data;
	idle_manager_notify_activity(seat->wlr_seat);
	cursor_set_visible(seat,   true);
	double lx, ly;
	wlr_cursor_absolute_to_layout_coords(seat->cursor,
		&event->pointer->base, event->x, event->y, &lx, &ly);
	double dx = lx - seat->cursor->x;
	double dy = ly - seat->cursor->y;
	LABWC_src_input_cursor_c_preprocess_cursor_motion(seat, event->pointer,
		event->time_msec, dx, dy);
}
static void
LABWC_src_input_cursor_c_process_release_mousebinding(struct cursor_context *ctx, uint32_t button)
{
	if (false) {
		return;
	}
	struct mousebind *mousebind;
	uint32_t modifiers = keyboard_get_all_modifiers(&server.seat);
	wl_list_for_each(mousebind, &rc.mousebinds, link) {
		if (ctx->type == LAB_NODE_CLIENT
				&& view_inhibits_actions(ctx->view, &mousebind->actions)) {
			continue;
		}
		if (node_type_contains(mousebind->context, ctx->type)
				&& mousebind->button == button
				&& modifiers == mousebind->modifiers) {
			switch (mousebind->mouse_event) {
			case MOUSE_ACTION_RELEASE:
				break;
			case MOUSE_ACTION_CLICK:
				if (mousebind->pressed_in_context) {
					break;
				}
				continue;
			default:
				continue;
			}
			actions_run(ctx->view, &mousebind->actions, ctx);
		}
	}
}
static bool
LABWC_src_input_cursor_c_is_double_click(long double_click_speed, uint32_t button,
		struct cursor_context *ctx)
{
	static enum lab_node_type last_type;
	static uint32_t last_button;
	static struct view *last_view;
	static struct timespec last_click;
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	long ms = (now.tv_sec - last_click.tv_sec) * 1000 +
		(now.tv_nsec - last_click.tv_nsec) / 1000000;
	last_click = now;
	if (last_button != button || last_view != ctx->view
			|| last_type != ctx->type) {
		last_button = button;
		last_view = ctx->view;
		last_type = ctx->type;
		return false;
	}
	if (ms < double_click_speed && ms >= 0) {
		last_button = 0;
		last_view = NULL;
		last_type = LAB_NODE_NONE;
		return true;
	}
	return false;
}
static bool
LABWC_src_input_cursor_c_process_press_mousebinding(struct cursor_context *ctx,
		uint32_t button)
{
	if (false) {
		return false;
	}
	struct mousebind *mousebind;
	bool double_click = LABWC_src_input_cursor_c_is_double_click(rc.doubleclick_time, button, ctx);
	bool consumed_by_frame_context = false;
	uint32_t modifiers = keyboard_get_all_modifiers(&server.seat);
	wl_list_for_each(mousebind, &rc.mousebinds, link) {
		if (ctx->type == LAB_NODE_CLIENT
				&& view_inhibits_actions(ctx->view, &mousebind->actions)) {
			continue;
		}
		if (node_type_contains(mousebind->context, ctx->type)
				&& mousebind->button == button
				&& modifiers == mousebind->modifiers) {
			switch (mousebind->mouse_event) {
			case MOUSE_ACTION_DRAG:
			case MOUSE_ACTION_CLICK:
				if (!double_click) {
					consumed_by_frame_context |=
						mousebind->context == LAB_NODE_FRAME;
					consumed_by_frame_context |=
						mousebind->context == LAB_NODE_ALL;
					mousebind->pressed_in_context = true;
				}
				continue;
			case MOUSE_ACTION_DOUBLECLICK:
				if (!double_click) {
					continue;
				}
				break;
			case MOUSE_ACTION_PRESS:
				break;
			default:
				continue;
			}
			consumed_by_frame_context |= mousebind->context == LAB_NODE_FRAME;
			consumed_by_frame_context |= mousebind->context == LAB_NODE_ALL;
			actions_run(ctx->view, &mousebind->actions, ctx);
		}
	}
	return consumed_by_frame_context;
}
static struct wlr_layer_surface_v1 *
LABWC_src_input_cursor_c_get_root_layer(struct wlr_surface *wlr_surface)
{
	assert(wlr_surface);
	struct wlr_subsurface *subsurface =
		wlr_subsurface_try_from_wlr_surface(wlr_surface);
	if (subsurface) {
		if (subsurface->parent) {
			return LABWC_src_input_cursor_c_get_root_layer(subsurface->parent);
		} else {
			return NULL;
		}
	} else {
		return wlr_layer_surface_v1_try_from_wlr_surface(wlr_surface);
	}
}
static uint32_t LABWC_src_input_cursor_c_press_msec;
bool
cursor_process_button_press(struct seat *seat, uint32_t button, uint32_t time_msec)
{
	struct cursor_context ctx = get_cursor_context();
	LABWC_src_input_cursor_c_press_msec = time_msec;
	if (ctx.view || ctx.surface) {
		cursor_context_save(&seat->pressed, &ctx);
		interactive_set_grab_context(&ctx);
	}
	if (server.input_mode == LAB_INPUT_STATE_MENU) {
		LABWC_src_input_cursor_c_press_msec = 0;
		lab_set_add(&seat->bound_buttons, button);
		return false;
	}
	if (ctx.type == LAB_NODE_LAYER_SURFACE) {
		struct wlr_layer_surface_v1 *layer = LABWC_src_input_cursor_c_get_root_layer(ctx.surface);
		if (layer && layer->current.keyboard_interactive) {
			layer_try_set_focus(seat, layer);
		}
#if HAVE_XWAYLAND
	} else if (ctx.type == LAB_NODE_UNMANAGED) {
		desktop_focus_view_or_surface(seat, NULL, ctx.surface,
			  false);
#endif
	}
	if (ctx.type != LAB_NODE_CLIENT && ctx.type != LAB_NODE_LAYER_SURFACE
			&& wlr_seat_pointer_has_grab(seat->wlr_seat)) {
		wlr_seat_pointer_end_grab(seat->wlr_seat);
		lab_set_add(&seat->bound_buttons, button);
		return false;
	}
	bool consumed_by_frame_context =
		LABWC_src_input_cursor_c_process_press_mousebinding(&ctx, button);
	if (ctx.surface && !consumed_by_frame_context) {
		return true;
	}
	lab_set_add(&seat->bound_buttons, button);
	return false;
}
bool
cursor_process_button_release(struct seat *seat, uint32_t button,
		uint32_t time_msec)
{
	struct cursor_context ctx = get_cursor_context();
	struct wlr_surface *pressed_surface = seat->pressed.ctx.surface;
	const bool notify = !lab_set_contains(&seat->bound_buttons, button);
	cursor_context_save(&seat->pressed, NULL);
	if (server.input_mode == LAB_INPUT_STATE_MENU) {
		if (time_msec - LABWC_src_input_cursor_c_press_msec > rc.menu_ignore_button_release_period) {
			if (ctx.type == LAB_NODE_MENUITEM) {
				menu_call_selected_actions();
			} else {
				menu_close();
				cursor_update_focus();
			}
		}
		return notify;
	}
	if (server.input_mode != LAB_INPUT_STATE_PASSTHROUGH) {
		return notify;
	}
	if (pressed_surface && ctx.surface != pressed_surface) {
		return notify;
	}
	LABWC_src_input_cursor_c_process_release_mousebinding(&ctx, button);
	return notify;
}
bool
cursor_finish_button_release(struct seat *seat, uint32_t button)
{
	struct mousebind *mousebind;
	wl_list_for_each(mousebind, &rc.mousebinds, link) {
		if (mousebind->button == button) {
			mousebind->pressed_in_context = false;
		}
	}
	lab_set_remove(&seat->bound_buttons, button);
	if (server.input_mode == LAB_INPUT_STATE_MOVE
			|| server.input_mode == LAB_INPUT_STATE_RESIZE) {
		if (resize_outlines_enabled(server.grabbed_view)) {
			resize_outlines_finish(server.grabbed_view);
		}
		interactive_finish(server.grabbed_view);
		return true;
	} else if (server.grabbed_view) {
		interactive_cancel(server.grabbed_view);
	}
	return false;
}
static void
LABWC_src_input_cursor_c_handle_button(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, on_cursor.button);
	struct wlr_pointer_button_event *event = data;
	idle_manager_notify_activity(seat->wlr_seat);
	cursor_set_visible(seat,   true);
	bool notify;
	switch (event->state) {
	case WL_POINTER_BUTTON_STATE_PRESSED:
		notify = cursor_process_button_press(seat, event->button,
			event->time_msec);
		if (notify) {
			wlr_seat_pointer_notify_button(seat->wlr_seat, event->time_msec,
				event->button, event->state);
		}
		break;
	case WL_POINTER_BUTTON_STATE_RELEASED:
		notify = cursor_process_button_release(seat, event->button,
			event->time_msec);
		if (notify) {
			wlr_seat_pointer_notify_button(seat->wlr_seat, event->time_msec,
				event->button, event->state);
		}
		cursor_finish_button_release(seat, event->button);
		break;
	}
}
struct LABWC_src_input_cursor_c_scroll_info {
	int direction;
	bool run_action;
};
static struct LABWC_src_input_cursor_c_scroll_info
LABWC_src_input_cursor_c_compare_delta(double delta, double delta_discrete, struct accumulated_scroll *accum)
{
	struct LABWC_src_input_cursor_c_scroll_info info = {0};
	if (delta_discrete) {
		info.direction = delta_discrete > 0 ? 1 : -1;
		accum->delta_discrete += delta_discrete;
		if (fabs(accum->delta_discrete) >= 120.0) {
			accum->delta_discrete = fmod(accum->delta_discrete, 120.0);
			info.run_action = true;
		}
	} else {
		if (delta == 0) {
			accum->delta = 0;
			return info;
		}
		info.direction = delta > 0 ? 1 : -1;
		accum->delta += delta;
		if (fabs(accum->delta) >= 10.0) {
			accum->delta = fmod(accum->delta, 10.0);
			info.run_action = true;
		}
	}
	return info;
}
static bool
LABWC_src_input_cursor_c_process_cursor_axis(enum wl_pointer_axis orientation,
		double delta, double delta_discrete)
{
	struct cursor_context ctx = get_cursor_context();
	uint32_t modifiers = keyboard_get_all_modifiers(&server.seat);
	enum direction direction = LAB_DIRECTION_INVALID;
	struct LABWC_src_input_cursor_c_scroll_info info = LABWC_src_input_cursor_c_compare_delta(delta, delta_discrete,
		&server.seat.accumulated_scrolls[orientation]);
	if (orientation == WL_POINTER_AXIS_HORIZONTAL_SCROLL) {
		if (info.direction < 0) {
			direction = LAB_DIRECTION_LEFT;
		} else if (info.direction > 0) {
			direction = LAB_DIRECTION_RIGHT;
		}
	} else if (orientation == WL_POINTER_AXIS_VERTICAL_SCROLL) {
		if (info.direction < 0) {
			direction = LAB_DIRECTION_UP;
		} else if (info.direction > 0) {
			direction = LAB_DIRECTION_DOWN;
		}
	} else {
	}
	bool consumed = false;
	if (direction != LAB_DIRECTION_INVALID) {
		struct mousebind *mousebind;
		wl_list_for_each(mousebind, &rc.mousebinds, link) {
			if (ctx.type == LAB_NODE_CLIENT
					&& view_inhibits_actions(ctx.view, &mousebind->actions)) {
				continue;
			}
			if (node_type_contains(mousebind->context, ctx.type)
					&& mousebind->direction == direction
					&& modifiers == mousebind->modifiers
					&& mousebind->mouse_event == MOUSE_ACTION_SCROLL) {
				consumed |= mousebind->context == LAB_NODE_FRAME;
				consumed |= mousebind->context == LAB_NODE_ALL;
				if (info.run_action) {
					actions_run(ctx.view, &mousebind->actions, &ctx);
				}
			}
		}
	}
	if (ctx.surface && !consumed) {
		LABWC_src_input_cursor_c_cursor_update_common(&ctx, NULL);
		return true;
	}
	return false;
}
static void
LABWC_src_input_cursor_c_handle_axis(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, on_cursor.axis);
	struct wlr_pointer_axis_event *event = data;
	idle_manager_notify_activity(seat->wlr_seat);
	cursor_set_visible(seat,   true);
	assert(event->pointer->base.type == WLR_INPUT_DEVICE_POINTER);
	struct input *input = event->pointer->base.data;
	double scroll_factor = input->scroll_factor;
	bool notify = LABWC_src_input_cursor_c_process_cursor_axis(event->orientation,
		event->delta, event->delta_discrete);
	if (notify) {
		wlr_seat_pointer_notify_axis(seat->wlr_seat, event->time_msec,
			event->orientation, scroll_factor * event->delta,
			(int32_t)round(scroll_factor * event->delta_discrete),
			event->source, event->relative_direction);
	}
}
static void
LABWC_src_input_cursor_c_handle_frame(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, on_cursor.frame);
	wlr_seat_pointer_notify_frame(seat->wlr_seat);
}
void
cursor_emulate_axis(struct seat *seat, struct wlr_input_device *device,
		enum wl_pointer_axis orientation, double delta, double delta_discrete,
		enum wl_pointer_axis_source source, uint32_t time_msec)
{
	struct input *input = device->data;
	double scroll_factor = 1.0;
	if (device->type == WLR_INPUT_DEVICE_POINTER) {
		scroll_factor = input->scroll_factor;
	}
	bool notify = LABWC_src_input_cursor_c_process_cursor_axis(orientation, delta, delta_discrete);
	if (notify) {
		wlr_seat_pointer_notify_axis(seat->wlr_seat, time_msec,
			orientation, scroll_factor * delta,
			(int32_t)round(scroll_factor * delta_discrete),
			source, WL_POINTER_AXIS_RELATIVE_DIRECTION_IDENTICAL);
	}
	wlr_seat_pointer_notify_frame(seat->wlr_seat);
}
void
cursor_emulate_move(struct seat *seat, struct wlr_input_device *device,
		double dx, double dy, uint32_t time_msec)
{
	if (!dx && !dy) {
		return;
	}
	wlr_relative_pointer_manager_v1_send_relative_motion(
		server.relative_pointer_manager,
		seat->wlr_seat, (uint64_t)time_msec * 1000,
		dx, dy, dx, dy);
	wlr_cursor_move(seat->cursor, device, dx, dy);
	double sx, sy;
	bool notify = cursor_process_motion(time_msec, &sx, &sy);
	if (notify) {
		wlr_seat_pointer_notify_motion(seat->wlr_seat, time_msec, sx, sy);
	}
	wlr_seat_pointer_notify_frame(seat->wlr_seat);
}
void
cursor_emulate_move_absolute(struct seat *seat, struct wlr_input_device *device,
		double x, double y, uint32_t time_msec)
{
	double lx, ly;
	wlr_cursor_absolute_to_layout_coords(seat->cursor,
		device, x, y, &lx, &ly);
	double dx = lx - seat->cursor->x;
	double dy = ly - seat->cursor->y;
	cursor_emulate_move(seat, device, dx, dy, time_msec);
}
void
cursor_emulate_button(struct seat *seat, uint32_t button,
		enum wl_pointer_button_state state, uint32_t time_msec)
{
	bool notify;
	switch (state) {
	case WL_POINTER_BUTTON_STATE_PRESSED:
		notify = cursor_process_button_press(seat, button, time_msec);
		if (notify) {
			wlr_seat_pointer_notify_button(seat->wlr_seat, time_msec, button, state);
		}
		break;
	case WL_POINTER_BUTTON_STATE_RELEASED:
		notify = cursor_process_button_release(seat, button, time_msec);
		if (notify) {
			wlr_seat_pointer_notify_button(seat->wlr_seat, time_msec, button, state);
		}
		cursor_finish_button_release(seat, button);
		break;
	}
	wlr_seat_pointer_notify_frame(seat->wlr_seat);
}
static void
LABWC_src_input_cursor_c_cursor_load(struct seat *seat)
{
	const char *xcursor_theme = getenv("XCURSOR_THEME");
	const char *xcursor_size = getenv("XCURSOR_SIZE");
	uint32_t size = xcursor_size ? (uint32_t)atoi(xcursor_size) : 24;
	if (seat->xcursor_manager) {
		wlr_xcursor_manager_destroy(seat->xcursor_manager);
	}
	seat->xcursor_manager = wlr_xcursor_manager_create(xcursor_theme, size);
	wlr_xcursor_manager_load(seat->xcursor_manager, 1);
	if (wlr_xcursor_manager_get_xcursor(seat->xcursor_manager,
			LABWC_src_input_cursor_c_cursors_xdg[LAB_CURSOR_GRAB], 1)) {
		LABWC_src_input_cursor_c_cursor_names = LABWC_src_input_cursor_c_cursors_xdg;
	} else {
		LABWC_src_input_cursor_c_cursor_names = LABWC_src_input_cursor_c_cursors_x11;
	}
}
void
cursor_reload(struct seat *seat)
{
	LABWC_src_input_cursor_c_cursor_load(seat);
	cursor_update_image(seat);
}
void
cursor_init(struct seat *seat)
{
	LABWC_src_input_cursor_c_cursor_load(seat);
	cursor_set(seat, LAB_CURSOR_DEFAULT);
	dnd_init(seat);
	CONNECT_SIGNAL(seat->cursor, &seat->on_cursor, motion);
	CONNECT_SIGNAL(seat->cursor, &seat->on_cursor, motion_absolute);
	CONNECT_SIGNAL(seat->cursor, &seat->on_cursor, button);
	CONNECT_SIGNAL(seat->cursor, &seat->on_cursor, axis);
	CONNECT_SIGNAL(seat->cursor, &seat->on_cursor, frame);
	gestures_init(seat);
	CONNECT_SIGNAL(seat->wlr_seat, seat, request_set_cursor);
	struct wlr_cursor_shape_manager_v1 *cursor_shape_manager =
		wlr_cursor_shape_manager_v1_create(server.wl_display,
			LAB_CURSOR_SHAPE_V1_VERSION);
	if (!cursor_shape_manager) {
		exit(EXIT_FAILURE);
	}
	CONNECT_SIGNAL(cursor_shape_manager, seat, request_set_shape);
	CONNECT_SIGNAL(seat->wlr_seat, seat, request_set_selection);
	CONNECT_SIGNAL(seat->wlr_seat, seat, request_set_primary_selection);
}
void cursor_finish(struct seat *seat)
{
	wl_list_remove(&seat->on_cursor.motion.link);
	wl_list_remove(&seat->on_cursor.motion_absolute.link);
	wl_list_remove(&seat->on_cursor.button.link);
	wl_list_remove(&seat->on_cursor.axis.link);
	wl_list_remove(&seat->on_cursor.frame.link);
	gestures_finish(seat);
	wl_list_remove(&seat->request_set_cursor.link);
	wl_list_remove(&seat->request_set_shape.link);
	wl_list_remove(&seat->request_set_selection.link);
	wl_list_remove(&seat->request_set_primary_selection.link);
	wlr_xcursor_manager_destroy(seat->xcursor_manager);
	wlr_cursor_destroy(seat->cursor);
	dnd_finish(seat);
}
#undef handle_axis
#undef handle_button
#undef handle_frame
#undef handle_motion
#undef handle_motion_absolute
#undef handle_request_set_cursor
#undef handle_request_set_primary_selection
#undef handle_request_set_selection
#undef handle_request_set_shape
#undef LAB_CURSOR_SHAPE_V1_VERSION
#define handle_hold_begin LABWC_src_input_gestures_c_handle_hold_begin
#define handle_hold_end LABWC_src_input_gestures_c_handle_hold_end
#define handle_pinch_begin LABWC_src_input_gestures_c_handle_pinch_begin
#define handle_pinch_end LABWC_src_input_gestures_c_handle_pinch_end
#define handle_pinch_update LABWC_src_input_gestures_c_handle_pinch_update
#define handle_swipe_begin LABWC_src_input_gestures_c_handle_swipe_begin
#define handle_swipe_end LABWC_src_input_gestures_c_handle_swipe_end
#define handle_swipe_update LABWC_src_input_gestures_c_handle_swipe_update
static void
LABWC_src_input_gestures_c_handle_pinch_begin(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, pinch_begin);
	struct wlr_pointer_pinch_begin_event *event = data;
	idle_manager_notify_activity(seat->wlr_seat);
	cursor_set_visible(seat,   true);
	wlr_pointer_gestures_v1_send_pinch_begin(seat->pointer_gestures,
		seat->wlr_seat, event->time_msec, event->fingers);
}
static void
LABWC_src_input_gestures_c_handle_pinch_update(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, pinch_update);
	struct wlr_pointer_pinch_update_event *event = data;
	idle_manager_notify_activity(seat->wlr_seat);
	cursor_set_visible(seat,   true);
	wlr_pointer_gestures_v1_send_pinch_update(seat->pointer_gestures,
		seat->wlr_seat, event->time_msec, event->dx, event->dy,
		event->scale, event->rotation);
}
static void
LABWC_src_input_gestures_c_handle_pinch_end(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, pinch_end);
	struct wlr_pointer_pinch_end_event *event = data;
	idle_manager_notify_activity(seat->wlr_seat);
	cursor_set_visible(seat,   true);
	wlr_pointer_gestures_v1_send_pinch_end(seat->pointer_gestures,
		seat->wlr_seat, event->time_msec, event->cancelled);
}
static void
LABWC_src_input_gestures_c_handle_swipe_begin(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, swipe_begin);
	struct wlr_pointer_swipe_begin_event *event = data;
	idle_manager_notify_activity(seat->wlr_seat);
	cursor_set_visible(seat,   true);
	wlr_pointer_gestures_v1_send_swipe_begin(seat->pointer_gestures,
		seat->wlr_seat, event->time_msec, event->fingers);
}
static void
LABWC_src_input_gestures_c_handle_swipe_update(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, swipe_update);
	struct wlr_pointer_swipe_update_event *event = data;
	idle_manager_notify_activity(seat->wlr_seat);
	cursor_set_visible(seat,   true);
	wlr_pointer_gestures_v1_send_swipe_update(seat->pointer_gestures,
		seat->wlr_seat, event->time_msec, event->dx, event->dy);
}
static void
LABWC_src_input_gestures_c_handle_swipe_end(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, swipe_end);
	struct wlr_pointer_swipe_end_event *event = data;
	idle_manager_notify_activity(seat->wlr_seat);
	cursor_set_visible(seat,   true);
	wlr_pointer_gestures_v1_send_swipe_end(seat->pointer_gestures,
		seat->wlr_seat, event->time_msec, event->cancelled);
}
static void
LABWC_src_input_gestures_c_handle_hold_begin(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, hold_begin);
	struct wlr_pointer_hold_begin_event *event = data;
	idle_manager_notify_activity(seat->wlr_seat);
	cursor_set_visible(seat,   true);
	wlr_pointer_gestures_v1_send_hold_begin(seat->pointer_gestures,
		seat->wlr_seat, event->time_msec, event->fingers);
}
static void
LABWC_src_input_gestures_c_handle_hold_end(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, hold_end);
	struct wlr_pointer_hold_end_event *event = data;
	idle_manager_notify_activity(seat->wlr_seat);
	cursor_set_visible(seat,   true);
	wlr_pointer_gestures_v1_send_hold_end(seat->pointer_gestures,
		seat->wlr_seat, event->time_msec, event->cancelled);
}
void
gestures_init(struct seat *seat)
{
	seat->pointer_gestures = wlr_pointer_gestures_v1_create(server.wl_display);
	CONNECT_SIGNAL(seat->cursor, seat, pinch_begin);
	CONNECT_SIGNAL(seat->cursor, seat, pinch_update);
	CONNECT_SIGNAL(seat->cursor, seat, pinch_end);
	CONNECT_SIGNAL(seat->cursor, seat, swipe_begin);
	CONNECT_SIGNAL(seat->cursor, seat, swipe_update);
	CONNECT_SIGNAL(seat->cursor, seat, swipe_end);
	CONNECT_SIGNAL(seat->cursor, seat, hold_begin);
	CONNECT_SIGNAL(seat->cursor, seat, hold_end);
}
void
gestures_finish(struct seat *seat)
{
	wl_list_remove(&seat->pinch_begin.link);
	wl_list_remove(&seat->pinch_update.link);
	wl_list_remove(&seat->pinch_end.link);
	wl_list_remove(&seat->swipe_begin.link);
	wl_list_remove(&seat->swipe_update.link);
	wl_list_remove(&seat->swipe_end.link);
	wl_list_remove(&seat->hold_begin.link);
	wl_list_remove(&seat->hold_end.link);
}
#undef handle_hold_begin
#undef handle_hold_end
#undef handle_pinch_begin
#undef handle_pinch_end
#undef handle_pinch_update
#undef handle_swipe_begin
#undef handle_swipe_end
#undef handle_swipe_update
#include <assert.h>
#define SAME_CLIENT(wlr_obj1, wlr_obj2) \
	(wl_resource_get_client((wlr_obj1)->resource) \
	== wl_resource_get_client((wlr_obj2)->resource))
static bool
LABWC_src_input_ime_c_is_keyboard_emulated_by_input_method(struct wlr_keyboard *keyboard,
		struct wlr_input_method_v2 *input_method)
{
	if (!keyboard || !input_method) {
		return false;
	}
	struct wlr_virtual_keyboard_v1 *virtual_keyboard =
		wlr_input_device_get_virtual_keyboard(&keyboard->base);
	return virtual_keyboard && SAME_CLIENT(virtual_keyboard, input_method);
}
static struct wlr_input_method_keyboard_grab_v2 *
LABWC_src_input_ime_c_get_keyboard_grab(struct keyboard *keyboard)
{
	struct wlr_input_method_v2 *input_method =
		keyboard->base.seat->input_method_relay->input_method;
	if (!input_method || !input_method->keyboard_grab) {
		return NULL;
	}
	if (LABWC_src_input_ime_c_is_keyboard_emulated_by_input_method(
			keyboard->wlr_keyboard, input_method)) {
		return NULL;
	}
	return input_method->keyboard_grab;
}
bool
input_method_keyboard_grab_forward_modifiers(struct keyboard *keyboard)
{
	struct wlr_input_method_keyboard_grab_v2 *keyboard_grab =
		LABWC_src_input_ime_c_get_keyboard_grab(keyboard);
	if (keyboard_grab) {
		wlr_input_method_keyboard_grab_v2_set_keyboard(keyboard_grab,
			keyboard->wlr_keyboard);
		wlr_input_method_keyboard_grab_v2_send_modifiers(keyboard_grab,
			&keyboard->wlr_keyboard->modifiers);
		return true;
	} else {
		return false;
	}
}
bool
input_method_keyboard_grab_forward_key(struct keyboard *keyboard,
		struct wlr_keyboard_key_event *event)
{
	struct wlr_input_method_keyboard_grab_v2 *keyboard_grab =
		LABWC_src_input_ime_c_get_keyboard_grab(keyboard);
	if (keyboard_grab) {
		wlr_input_method_keyboard_grab_v2_set_keyboard(keyboard_grab,
			keyboard->wlr_keyboard);
		wlr_input_method_keyboard_grab_v2_send_key(keyboard_grab,
			event->time_msec, event->keycode, event->state);
		return true;
	} else {
		return false;
	}
}
static struct text_input *
LABWC_src_input_ime_c_get_active_text_input(struct input_method_relay *relay)
{
	if (!relay->input_method) {
		return NULL;
	}
	struct text_input *text_input;
	wl_list_for_each(text_input, &relay->text_inputs, link) {
		if (text_input->input->focused_surface
				&& text_input->input->current_enabled) {
			return text_input;
		}
	}
	return NULL;
}
static void
LABWC_src_input_ime_c_update_active_text_input(struct input_method_relay *relay)
{
	struct text_input *active_text_input = LABWC_src_input_ime_c_get_active_text_input(relay);
	if (relay->input_method && relay->active_text_input
			!= active_text_input) {
		if (active_text_input) {
			wlr_input_method_v2_send_activate(relay->input_method);
		} else {
			wlr_input_method_v2_send_deactivate(relay->input_method);
		}
		wlr_input_method_v2_send_done(relay->input_method);
	}
	relay->active_text_input = active_text_input;
}
static void
LABWC_src_input_ime_c_update_text_inputs_focused_surface(struct input_method_relay *relay)
{
	struct text_input *text_input;
	wl_list_for_each(text_input, &relay->text_inputs, link) {
		struct wlr_text_input_v3 *input = text_input->input;
		struct wlr_surface *new_focused_surface;
		if (relay->input_method && relay->focused_surface
				&& SAME_CLIENT(input, relay->focused_surface)) {
			new_focused_surface = relay->focused_surface;
		} else {
			new_focused_surface = NULL;
		}
		if (input->focused_surface == new_focused_surface) {
			continue;
		}
		if (input->focused_surface) {
			wlr_text_input_v3_send_leave(input);
		}
		if (new_focused_surface) {
			wlr_text_input_v3_send_enter(input, new_focused_surface);
		}
	}
}
static void
LABWC_src_input_ime_c_update_popup_position(struct input_method_popup *popup)
{
	struct input_method_relay *relay = popup->relay;
	struct text_input *text_input = relay->active_text_input;
	if (!text_input || !relay->focused_surface
			|| !popup->popup_surface->surface->mapped) {
		return;
	}
	struct wlr_box cursor_rect;
	struct wlr_xdg_surface *xdg_surface =
		wlr_xdg_surface_try_from_wlr_surface(relay->focused_surface);
	struct wlr_layer_surface_v1 *layer_surface =
		wlr_layer_surface_v1_try_from_wlr_surface(relay->focused_surface);
	if ((text_input->input->current.features
			& WLR_TEXT_INPUT_V3_FEATURE_CURSOR_RECTANGLE)
			&& (xdg_surface || layer_surface)) {
		cursor_rect = text_input->input->current.cursor_rectangle;
		struct wlr_scene_tree *tree = relay->focused_surface->data;
		int lx, ly;
		wlr_scene_node_coords(&tree->node, &lx, &ly);
		cursor_rect.x += lx;
		cursor_rect.y += ly;
		if (xdg_surface) {
			cursor_rect.x -= xdg_surface->geometry.x;
			cursor_rect.y -= xdg_surface->geometry.y;
		}
	} else {
		cursor_rect = (struct wlr_box){0};
	}
	struct output *output =
		output_nearest_to(cursor_rect.x, cursor_rect.y);
	if (!output_is_usable(output)) {
		return;
	}
	struct wlr_box output_box;
	wlr_output_layout_get_box(
		server.output_layout, output->wlr_output, &output_box);
	struct wlr_xdg_positioner_rules rules = {
		.anchor_rect = cursor_rect,
		.anchor = XDG_POSITIONER_ANCHOR_BOTTOM_LEFT,
		.gravity = XDG_POSITIONER_GRAVITY_BOTTOM_RIGHT,
		.constraint_adjustment =
			XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_FLIP_Y
			| XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_SLIDE_X,
		.size = {
			.width = popup->popup_surface->surface->current.width,
			.height = popup->popup_surface->surface->current.height,
		},
	};
	struct wlr_box popup_box;
	wlr_xdg_positioner_rules_get_geometry(&rules, &popup_box);
	wlr_xdg_positioner_rules_unconstrain_box(&rules, &output_box, &popup_box);
	wlr_scene_node_set_position(
		&popup->tree->node, popup_box.x, popup_box.y);
	wlr_scene_node_raise_to_top(&relay->popup_tree->node);
	wlr_input_popup_surface_v2_send_text_input_rectangle(
		popup->popup_surface, &(struct wlr_box){
			.x = cursor_rect.x - popup_box.x,
			.y = cursor_rect.y - popup_box.y,
			.width = cursor_rect.width,
			.height = cursor_rect.height,
		});
}
static void
LABWC_src_input_ime_c_update_popups_position(struct input_method_relay *relay)
{
	struct input_method_popup *popup;
	wl_list_for_each(popup, &relay->popups, link) {
		LABWC_src_input_ime_c_update_popup_position(popup);
	}
}
static void
LABWC_src_input_ime_c_handle_input_method_commit(struct wl_listener *listener, void *data)
{
	struct input_method_relay *relay =
		wl_container_of(listener, relay, input_method_commit);
	struct wlr_input_method_v2 *input_method = relay->input_method;
	struct text_input *text_input = relay->active_text_input;
	if (!text_input) {
		return;
	}
	if (input_method->current.preedit.text) {
		wlr_text_input_v3_send_preedit_string(
			text_input->input,
			input_method->current.preedit.text,
			input_method->current.preedit.cursor_begin,
			input_method->current.preedit.cursor_end);
	}
	if (input_method->current.commit_text) {
		wlr_text_input_v3_send_commit_string(
			text_input->input,
			input_method->current.commit_text);
	}
	if (input_method->current.delete.before_length
			|| input_method->current.delete.after_length) {
		wlr_text_input_v3_send_delete_surrounding_text(
			text_input->input,
			input_method->current.delete.before_length,
			input_method->current.delete.after_length);
	}
	wlr_text_input_v3_send_done(text_input->input);
}
static void
LABWC_src_input_ime_c_handle_keyboard_grab_destroy(struct wl_listener *listener, void *data)
{
	struct input_method_relay *relay =
		wl_container_of(listener, relay, keyboard_grab_destroy);
	struct wlr_input_method_keyboard_grab_v2 *keyboard_grab =
		relay->input_method->keyboard_grab;
	assert(keyboard_grab);
	wl_list_remove(&relay->keyboard_grab_destroy.link);
	if (keyboard_grab->keyboard) {
		wlr_seat_keyboard_notify_modifiers(
			keyboard_grab->input_method->seat,
			&keyboard_grab->keyboard->modifiers);
	}
}
static void
LABWC_src_input_ime_c_handle_input_method_grab_keyboard(struct wl_listener *listener, void *data)
{
	struct input_method_relay *relay = wl_container_of(listener, relay,
		input_method_grab_keyboard);
	struct wlr_input_method_keyboard_grab_v2 *keyboard_grab = data;
	struct wlr_keyboard *active_keyboard =
		wlr_seat_get_keyboard(relay->seat->wlr_seat);
	if (!LABWC_src_input_ime_c_is_keyboard_emulated_by_input_method(
			active_keyboard, relay->input_method)) {
		wlr_input_method_keyboard_grab_v2_set_keyboard(
			keyboard_grab, active_keyboard);
	}
	relay->keyboard_grab_destroy.notify = LABWC_src_input_ime_c_handle_keyboard_grab_destroy;
	wl_signal_add(&keyboard_grab->events.destroy,
		&relay->keyboard_grab_destroy);
}
static void
LABWC_src_input_ime_c_handle_input_method_destroy(struct wl_listener *listener, void *data)
{
	struct input_method_relay *relay =
		wl_container_of(listener, relay, input_method_destroy);
	wl_list_remove(&relay->input_method_commit.link);
	wl_list_remove(&relay->input_method_grab_keyboard.link);
	wl_list_remove(&relay->input_method_new_popup_surface.link);
	wl_list_remove(&relay->input_method_destroy.link);
	relay->input_method = NULL;
	LABWC_src_input_ime_c_update_text_inputs_focused_surface(relay);
	LABWC_src_input_ime_c_update_active_text_input(relay);
}
static void
LABWC_src_input_ime_c_handle_popup_surface_destroy(struct wl_listener *listener, void *data)
{
	struct input_method_popup *popup =
		wl_container_of(listener, popup, destroy);
	wlr_scene_node_destroy(&popup->tree->node);
	wl_list_remove(&popup->destroy.link);
	wl_list_remove(&popup->commit.link);
	wl_list_remove(&popup->link);
	free(popup);
}
static void
LABWC_src_input_ime_c_handle_popup_surface_commit(struct wl_listener *listener, void *data)
{
	struct input_method_popup *popup =
		wl_container_of(listener, popup, commit);
	LABWC_src_input_ime_c_update_popup_position(popup);
}
static void
LABWC_src_input_ime_c_handle_input_method_new_popup_surface(struct wl_listener *listener, void *data)
{
	struct input_method_relay *relay = wl_container_of(listener, relay,
		input_method_new_popup_surface);
	struct input_method_popup *popup = znew(*popup);
	popup->popup_surface = data;
	popup->relay = relay;
	popup->destroy.notify = LABWC_src_input_ime_c_handle_popup_surface_destroy;
	wl_signal_add(&popup->popup_surface->events.destroy, &popup->destroy);
	popup->commit.notify = LABWC_src_input_ime_c_handle_popup_surface_commit;
	wl_signal_add(&popup->popup_surface->surface->events.commit,
		&popup->commit);
	popup->tree = wlr_scene_subsurface_tree_create(
		relay->popup_tree, popup->popup_surface->surface);
	die_if_null(popup->tree);
	node_descriptor_create(&popup->tree->node, LAB_NODE_IME_POPUP,
		  NULL,   NULL);
	wl_list_insert(&relay->popups, &popup->link);
	LABWC_src_input_ime_c_update_popup_position(popup);
}
static void
LABWC_src_input_ime_c_handle_new_input_method(struct wl_listener *listener, void *data)
{
	struct input_method_relay *relay =
		wl_container_of(listener, relay, new_input_method);
	struct wlr_input_method_v2 *input_method = data;
	if (relay->seat->wlr_seat != input_method->seat) {
		return;
	}
	if (relay->input_method) {
		wlr_input_method_v2_send_unavailable(input_method);
		return;
	}
	relay->input_method = input_method;
	relay->input_method_commit.notify = LABWC_src_input_ime_c_handle_input_method_commit;
	wl_signal_add(&relay->input_method->events.commit,
		&relay->input_method_commit);
	relay->input_method_grab_keyboard.notify =
		LABWC_src_input_ime_c_handle_input_method_grab_keyboard;
	wl_signal_add(&relay->input_method->events.grab_keyboard,
		&relay->input_method_grab_keyboard);
	relay->input_method_destroy.notify = LABWC_src_input_ime_c_handle_input_method_destroy;
	wl_signal_add(&relay->input_method->events.destroy,
		&relay->input_method_destroy);
	relay->input_method_new_popup_surface.notify =
		LABWC_src_input_ime_c_handle_input_method_new_popup_surface;
	wl_signal_add(&relay->input_method->events.new_popup_surface,
		&relay->input_method_new_popup_surface);
	LABWC_src_input_ime_c_update_text_inputs_focused_surface(relay);
	LABWC_src_input_ime_c_update_active_text_input(relay);
}
static void
LABWC_src_input_ime_c_send_state_to_input_method(struct input_method_relay *relay)
{
	assert(relay->active_text_input && relay->input_method);
	struct wlr_input_method_v2 *input_method = relay->input_method;
	struct wlr_text_input_v3 *input = relay->active_text_input->input;
	if (input->active_features
			& WLR_TEXT_INPUT_V3_FEATURE_SURROUNDING_TEXT) {
		wlr_input_method_v2_send_surrounding_text(input_method,
			input->current.surrounding.text,
			input->current.surrounding.cursor,
			input->current.surrounding.anchor);
	}
	wlr_input_method_v2_send_text_change_cause(input_method,
		input->current.text_change_cause);
	if (input->active_features & WLR_TEXT_INPUT_V3_FEATURE_CONTENT_TYPE) {
		wlr_input_method_v2_send_content_type(input_method,
			input->current.content_type.hint,
			input->current.content_type.purpose);
	}
	wlr_input_method_v2_send_done(input_method);
}
static void
LABWC_src_input_ime_c_handle_text_input_enable(struct wl_listener *listener, void *data)
{
	struct text_input *text_input =
		wl_container_of(listener, text_input, enable);
	struct input_method_relay *relay = text_input->relay;
	LABWC_src_input_ime_c_update_active_text_input(relay);
	if (relay->active_text_input == text_input) {
		LABWC_src_input_ime_c_update_popups_position(relay);
		LABWC_src_input_ime_c_send_state_to_input_method(relay);
	}
}
static void
LABWC_src_input_ime_c_handle_text_input_disable(struct wl_listener *listener, void *data)
{
	struct text_input *text_input =
		wl_container_of(listener, text_input, disable);
	LABWC_src_input_ime_c_update_active_text_input(text_input->relay);
}
static void
LABWC_src_input_ime_c_handle_text_input_commit(struct wl_listener *listener, void *data)
{
	struct text_input *text_input =
		wl_container_of(listener, text_input, commit);
	struct input_method_relay *relay = text_input->relay;
	if (relay->active_text_input == text_input) {
		LABWC_src_input_ime_c_update_popups_position(relay);
		LABWC_src_input_ime_c_send_state_to_input_method(relay);
	}
}
static void
LABWC_src_input_ime_c_handle_text_input_destroy(struct wl_listener *listener, void *data)
{
	struct text_input *text_input =
		wl_container_of(listener, text_input, destroy);
	wl_list_remove(&text_input->enable.link);
	wl_list_remove(&text_input->disable.link);
	wl_list_remove(&text_input->commit.link);
	wl_list_remove(&text_input->destroy.link);
	wl_list_remove(&text_input->link);
	LABWC_src_input_ime_c_update_active_text_input(text_input->relay);
	free(text_input);
}
static void
LABWC_src_input_ime_c_handle_new_text_input(struct wl_listener *listener, void *data)
{
	struct input_method_relay *relay =
		wl_container_of(listener, relay, new_text_input);
	struct wlr_text_input_v3 *wlr_text_input = data;
	if (relay->seat->wlr_seat != wlr_text_input->seat) {
		return;
	}
	struct text_input *text_input = znew(*text_input);
	text_input->input = wlr_text_input;
	text_input->relay = relay;
	wl_list_insert(&relay->text_inputs, &text_input->link);
	text_input->enable.notify = LABWC_src_input_ime_c_handle_text_input_enable;
	wl_signal_add(&text_input->input->events.enable, &text_input->enable);
	text_input->disable.notify = LABWC_src_input_ime_c_handle_text_input_disable;
	wl_signal_add(&text_input->input->events.disable, &text_input->disable);
	text_input->commit.notify = LABWC_src_input_ime_c_handle_text_input_commit;
	wl_signal_add(&text_input->input->events.commit, &text_input->commit);
	text_input->destroy.notify = LABWC_src_input_ime_c_handle_text_input_destroy;
	wl_signal_add(&text_input->input->events.destroy, &text_input->destroy);
	LABWC_src_input_ime_c_update_text_inputs_focused_surface(relay);
}
static void
LABWC_src_input_ime_c_handle_focused_surface_destroy(struct wl_listener *listener, void *data)
{
	struct input_method_relay *relay =
		wl_container_of(listener, relay, focused_surface_destroy);
	assert(!data || relay->focused_surface == data);
	input_method_relay_set_focus(relay, NULL);
}
struct input_method_relay *
input_method_relay_create(struct seat *seat)
{
	struct input_method_relay *relay = znew(*relay);
	relay->seat = seat;
	wl_list_init(&relay->text_inputs);
	wl_list_init(&relay->popups);
	relay->popup_tree = lab_wlr_scene_tree_create(&server.scene->tree);
	relay->new_text_input.notify = LABWC_src_input_ime_c_handle_new_text_input;
	wl_signal_add(&server.text_input_manager->events.new_text_input,
		&relay->new_text_input);
	relay->new_input_method.notify = LABWC_src_input_ime_c_handle_new_input_method;
	wl_signal_add(&server.input_method_manager->events.new_input_method,
		&relay->new_input_method);
	relay->focused_surface_destroy.notify = LABWC_src_input_ime_c_handle_focused_surface_destroy;
	return relay;
}
void
input_method_relay_finish(struct input_method_relay *relay)
{
	wl_list_remove(&relay->new_text_input.link);
	wl_list_remove(&relay->new_input_method.link);
	free(relay);
}
void
input_method_relay_set_focus(struct input_method_relay *relay,
		struct wlr_surface *surface)
{
	if (relay->focused_surface == surface) {
		return;
	}
	if (relay->focused_surface) {
		wl_list_remove(&relay->focused_surface_destroy.link);
	}
	relay->focused_surface = surface;
	if (surface) {
		wl_signal_add(&surface->events.destroy,
			&relay->focused_surface_destroy);
	}
	LABWC_src_input_ime_c_update_text_inputs_focused_surface(relay);
	LABWC_src_input_ime_c_update_active_text_input(relay);
}
#undef SAME_CLIENT
void
input_handlers_init(struct seat *seat)
{
	cursor_init(seat);
	keyboard_group_init(seat);
}
void
input_handlers_finish(struct seat *seat)
{
	cursor_finish(seat);
	keyboard_group_finish(seat);
}
#include <stdbool.h>
#include <stdint.h>
#include <xkbcommon/xkbcommon.h>
static struct lab_set LABWC_src_input_key_state_c_pressed, LABWC_src_input_key_state_c_bound, LABWC_src_input_key_state_c_pressed_sent;
static bool LABWC_src_input_key_state_c_show_debug_indicator;
static struct indicator_state {
	struct wlr_scene_tree *tree;
	struct scaled_font_buffer *sfb_pressed;
	struct scaled_font_buffer *sfb_bound;
	struct scaled_font_buffer *sfb_pressed_sent;
	struct scaled_font_buffer *sfb_modifiers;
	struct xkb_keymap *keymap;
} LABWC_src_input_key_state_c_indicator_state;
static const char *
LABWC_src_input_key_state_c_keycode_to_keyname(struct xkb_keymap *keymap, uint32_t keycode)
{
	const xkb_keysym_t *syms;
	int syms_len = xkb_keymap_key_get_syms_by_level(keymap, keycode + 8, 0, 0, &syms);
	if (!syms_len) {
		return NULL;
	}
	static char buf[256];
	if (!xkb_keysym_get_name(syms[0], buf, sizeof(buf))) {
		return NULL;
	}
	return buf;
}
static const char *
LABWC_src_input_key_state_c_modifier_to_name(uint32_t modifier)
{
	switch (modifier) {
	case WLR_MODIFIER_SHIFT:
		return "S";
	case WLR_MODIFIER_CAPS:
		return "caps";
	case WLR_MODIFIER_CTRL:
		return "C";
	case WLR_MODIFIER_ALT:
		return "A";
	case WLR_MODIFIER_MOD2:
		return "numlock";
	case WLR_MODIFIER_MOD3:
		return "H";
	case WLR_MODIFIER_LOGO:
		return "W";
	case WLR_MODIFIER_MOD5:
		return "M";
	default:
		return "?";
	}
}
static void
LABWC_src_input_key_state_c_init_indicator(struct indicator_state *state)
{
	state->tree = wlr_scene_tree_create(&server.scene->tree);
	wlr_scene_node_set_enabled(&state->tree->node, false);
	state->sfb_pressed = scaled_font_buffer_create(state->tree);
	wlr_scene_node_set_position(&state->sfb_pressed->scene_buffer->node, 0, 0);
	state->sfb_bound = scaled_font_buffer_create(state->tree);
	wlr_scene_node_set_position(&state->sfb_bound->scene_buffer->node, 0, 20);
	state->sfb_pressed_sent = scaled_font_buffer_create(state->tree);
	wlr_scene_node_set_position(&state->sfb_pressed_sent->scene_buffer->node, 0, 40);
	state->sfb_modifiers = scaled_font_buffer_create(state->tree);
	wlr_scene_node_set_position(&state->sfb_modifiers->scene_buffer->node, 0, 60);
	struct xkb_rule_names rules = { 0 };
	struct xkb_context *context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
	state->keymap = xkb_map_new_from_names(context, &rules, XKB_KEYMAP_COMPILE_NO_FLAGS);
}
static void
LABWC_src_input_key_state_c_update_key_indicator_callback(void *data)
{
	struct seat *seat = data;
	uint32_t all_modifiers = keyboard_get_all_modifiers(seat);
	float black[4] = {0, 0, 0, 1};
	float white[4] = {1, 1, 1, 1};
	if (!LABWC_src_input_key_state_c_indicator_state.tree) {
		LABWC_src_input_key_state_c_init_indicator(&LABWC_src_input_key_state_c_indicator_state);
	}
	if (LABWC_src_input_key_state_c_show_debug_indicator) {
		wlr_scene_node_set_enabled(&LABWC_src_input_key_state_c_indicator_state.tree->node, true);
	} else {
		wlr_scene_node_set_enabled(&LABWC_src_input_key_state_c_indicator_state.tree->node, false);
		return;
	}
	struct buf buf = BUF_INIT;
	buf_add(&buf, "pressed=");
	for (int i = 0; i < LABWC_src_input_key_state_c_pressed.size; i++) {
		const char *keyname = LABWC_src_input_key_state_c_keycode_to_keyname(LABWC_src_input_key_state_c_indicator_state.keymap,
					LABWC_src_input_key_state_c_pressed.values[i]);
		buf_add_fmt(&buf, "%s (%d), ", keyname, LABWC_src_input_key_state_c_pressed.values[i]);
	}
	scaled_font_buffer_update(LABWC_src_input_key_state_c_indicator_state.sfb_pressed, buf.data,
		-1, &rc.font_osd, black, white);
	buf_clear(&buf);
	buf_add(&buf, "bound=");
	for (int i = 0; i < LABWC_src_input_key_state_c_bound.size; i++) {
		const char *keyname = LABWC_src_input_key_state_c_keycode_to_keyname(LABWC_src_input_key_state_c_indicator_state.keymap,
					LABWC_src_input_key_state_c_bound.values[i]);
		buf_add_fmt(&buf, "%s (%d), ", keyname, LABWC_src_input_key_state_c_bound.values[i]);
	}
	scaled_font_buffer_update(LABWC_src_input_key_state_c_indicator_state.sfb_bound, buf.data,
		-1, &rc.font_osd, black, white);
	buf_clear(&buf);
	buf_add(&buf, "pressed_sent=");
	for (int i = 0; i < LABWC_src_input_key_state_c_pressed_sent.size; i++) {
		const char *keyname = LABWC_src_input_key_state_c_keycode_to_keyname(LABWC_src_input_key_state_c_indicator_state.keymap,
					LABWC_src_input_key_state_c_pressed_sent.values[i]);
		buf_add_fmt(&buf, "%s (%d), ", keyname, LABWC_src_input_key_state_c_pressed_sent.values[i]);
	}
	scaled_font_buffer_update(LABWC_src_input_key_state_c_indicator_state.sfb_pressed_sent, buf.data, -1,
		&rc.font_osd, black, white);
	buf_clear(&buf);
	buf_add(&buf, "modifiers=");
	for (int i = 0; i <= 7; i++) {
		uint32_t mod = 1 << i;
		if (all_modifiers & mod) {
			buf_add_fmt(&buf, "%s, ", LABWC_src_input_key_state_c_modifier_to_name(mod));
		}
	}
	buf_add_fmt(&buf, "(%d)", all_modifiers);
	scaled_font_buffer_update(LABWC_src_input_key_state_c_indicator_state.sfb_modifiers, buf.data, -1,
		&rc.font_osd, black, white);
	buf_reset(&buf);
}
void
key_state_indicator_update(struct seat *seat)
{
	if (!LABWC_src_input_key_state_c_show_debug_indicator) {
		return;
	}
	wl_event_loop_add_idle(server.wl_event_loop,
		LABWC_src_input_key_state_c_update_key_indicator_callback, seat);
}
void
key_state_indicator_toggle(void)
{
	LABWC_src_input_key_state_c_show_debug_indicator = !LABWC_src_input_key_state_c_show_debug_indicator;
}
uint32_t *
key_state_pressed_sent_keycodes(void)
{
	LABWC_src_input_key_state_c_pressed_sent = LABWC_src_input_key_state_c_pressed;
	for (int i = 0; i < LABWC_src_input_key_state_c_bound.size; ++i) {
		lab_set_remove(&LABWC_src_input_key_state_c_pressed_sent, LABWC_src_input_key_state_c_bound.values[i]);
	}
	return LABWC_src_input_key_state_c_pressed_sent.values;
}
int
key_state_nr_pressed_sent_keycodes(void)
{
	return LABWC_src_input_key_state_c_pressed_sent.size;
}
void
key_state_set_pressed(uint32_t keycode, bool is_pressed)
{
	if (is_pressed) {
		lab_set_add(&LABWC_src_input_key_state_c_pressed, keycode);
	} else {
		lab_set_remove(&LABWC_src_input_key_state_c_pressed, keycode);
	}
}
void
key_state_store_pressed_key_as_bound(uint32_t keycode)
{
	lab_set_add(&LABWC_src_input_key_state_c_bound, keycode);
}
bool
key_state_corresponding_press_event_was_bound(uint32_t keycode)
{
	return lab_set_contains(&LABWC_src_input_key_state_c_bound, keycode);
}
void
key_state_bound_key_remove(uint32_t keycode)
{
	lab_set_remove(&LABWC_src_input_key_state_c_bound, keycode);
}
int
key_state_nr_bound_keys(void)
{
	return LABWC_src_input_key_state_c_bound.size;
}
#define handle_key LABWC_src_input_keyboard_c_handle_key
#define handle_modifiers LABWC_src_input_keyboard_c_handle_modifiers
#include <assert.h>
#include <stdlib.h>
#if WLR_HAS_SESSION
#endif
enum LABWC_src_input_keyboard_c_lab_key_handled {
	LAB_KEY_HANDLED_FALSE = 0,
	LAB_KEY_HANDLED_TRUE = 1,
	LAB_KEY_HANDLED_TRUE_AND_VT_CHANGED,
};
struct LABWC_src_input_keyboard_c_keysyms {
	const xkb_keysym_t *syms;
	int nr_syms;
};
struct LABWC_src_input_keyboard_c_keyinfo {
	xkb_keycode_t xkb_keycode;
	struct LABWC_src_input_keyboard_c_keysyms translated;
	struct LABWC_src_input_keyboard_c_keysyms raw;
	uint32_t modifiers;
	bool is_modifier;
};
static bool LABWC_src_input_keyboard_c_should_cancel_cycling_on_next_key_release;
static struct keybind *LABWC_src_input_keyboard_c_cur_keybind;
void
keyboard_reset_current_keybind(void)
{
	LABWC_src_input_keyboard_c_cur_keybind = NULL;
}
static void
LABWC_src_input_keyboard_c_change_vt(unsigned int vt)
{
#if WLR_HAS_SESSION
	wlr_session_change_vt(server.session, vt);
#endif
}
uint32_t
keyboard_get_all_modifiers(struct seat *seat)
{
	struct input *input;
	uint32_t modifiers = wlr_keyboard_get_modifiers(&seat->keyboard_group->keyboard);
	wl_list_for_each(input, &seat->inputs, link) {
		if (input->wlr_input_device->type != WLR_INPUT_DEVICE_KEYBOARD) {
			continue;
		}
		struct keyboard *kb = wl_container_of(input, kb, base);
		if (kb->is_virtual) {
			modifiers |= wlr_keyboard_get_modifiers(kb->wlr_keyboard);
		}
	}
	return modifiers;
}
static struct wlr_seat_client *
LABWC_src_input_keyboard_c_seat_client_from_keyboard_resource(struct wl_resource *resource)
{
	return wl_resource_get_user_data(resource);
}
static void
LABWC_src_input_keyboard_c_broadcast_modifiers_to_unfocused_clients(struct wlr_seat *wlr_seat,
		const struct keyboard *keyboard,
		const struct wlr_keyboard_modifiers *modifiers)
{
	if (keyboard->is_virtual) {
		return;
	}
	struct wlr_seat_client *client;
	wl_list_for_each(client, &wlr_seat->clients, link) {
		if (client == wlr_seat->keyboard_state.focused_client) {
			continue;
		}
		uint32_t serial = wlr_seat_client_next_serial(client);
		struct wl_resource *resource;
		wl_resource_for_each(resource, &client->keyboards) {
			if (!LABWC_src_input_keyboard_c_seat_client_from_keyboard_resource(resource)) {
				continue;
			}
			if (!modifiers) {
				wl_keyboard_send_modifiers(resource, serial, 0,
					0, 0, 0);
			} else {
				wl_keyboard_send_modifiers(resource, serial,
					modifiers->depressed, modifiers->latched,
					modifiers->locked, modifiers->group);
			}
		}
	}
}
static void
LABWC_src_input_keyboard_c_handle_modifiers(struct wl_listener *listener, void *data)
{
	struct keyboard *keyboard = wl_container_of(listener, keyboard, modifiers);
	struct seat *seat = keyboard->base.seat;
	struct wlr_keyboard *wlr_keyboard = keyboard->wlr_keyboard;
	key_state_indicator_update(seat);
	if (server.input_mode == LAB_INPUT_STATE_MOVE) {
		seat->region_prevent_snap = false;
		overlay_update(seat);
	}
	bool cycling = false;
	if ((cycling || seat->workspace_osd_shown_by_modifier)
			&& !keyboard_get_all_modifiers(seat)) {
		if (cycling) {
			if (key_state_nr_bound_keys()) {
				LABWC_src_input_keyboard_c_should_cancel_cycling_on_next_key_release = true;
			} else {
				LABWC_src_input_keyboard_c_should_cancel_cycling_on_next_key_release = false;
			}
		}
		if (seat->workspace_osd_shown_by_modifier) {
			workspaces_osd_hide(seat);
		}
	}
	if (!input_method_keyboard_grab_forward_modifiers(keyboard)) {
		wlr_seat_keyboard_notify_modifiers(seat->wlr_seat,
			&wlr_keyboard->modifiers);
		LABWC_src_input_keyboard_c_broadcast_modifiers_to_unfocused_clients(seat->wlr_seat,
			keyboard, &wlr_keyboard->modifiers);
	}
}
static struct keybind *
LABWC_src_input_keyboard_c_match_keybinding_for_sym(uint32_t modifiers,
		xkb_keysym_t sym, xkb_keycode_t xkb_keycode)
{
	struct keybind *keybind;
	wl_list_for_each(keybind, &rc.keybinds, link) {
		if (modifiers ^ keybind->modifiers) {
			continue;
		}
		if (!(keybind->override_inhibition)) {
			if (view_inhibits_actions(server.active_view, &keybind->actions)) {
				continue;
			}
		}
		if (sym == XKB_KEY_NoSymbol) {
			if (keybind_contains_keycode(keybind, xkb_keycode)) {
				return keybind;
			}
		} else {
			if (keybind_contains_keysym(keybind,
					xkb_keysym_to_lower(sym))) {
				return keybind;
			}
		}
	}
	return NULL;
}
static struct keybind *
LABWC_src_input_keyboard_c_match_keybinding(struct LABWC_src_input_keyboard_c_keyinfo *keyinfo,
		bool is_virtual)
{
	if (!is_virtual) {
		struct keybind *keybind = LABWC_src_input_keyboard_c_match_keybinding_for_sym(
			keyinfo->modifiers, XKB_KEY_NoSymbol, keyinfo->xkb_keycode);
		if (keybind) {
			return keybind;
		}
	}
	for (int i = 0; i < keyinfo->translated.nr_syms; i++) {
		struct keybind *keybind =
			LABWC_src_input_keyboard_c_match_keybinding_for_sym(keyinfo->modifiers,
				keyinfo->translated.syms[i], keyinfo->xkb_keycode);
		if (keybind) {
			return keybind;
		}
	}
	for (int i = 0; i < keyinfo->raw.nr_syms; i++) {
		struct keybind *keybind =
			LABWC_src_input_keyboard_c_match_keybinding_for_sym(keyinfo->modifiers,
				keyinfo->raw.syms[i], keyinfo->xkb_keycode);
		if (keybind) {
			return keybind;
		}
	}
	return NULL;
}
static bool
LABWC_src_input_keyboard_c_is_modifier_key(xkb_keysym_t sym)
{
	switch (sym) {
	case XKB_KEY_Shift_L:   case XKB_KEY_Shift_R:
	case XKB_KEY_Alt_L:     case XKB_KEY_Alt_R:
	case XKB_KEY_Control_L: case XKB_KEY_Control_R:
	case XKB_KEY_Super_L:   case XKB_KEY_Super_R:
	case XKB_KEY_Hyper_L:   case XKB_KEY_Hyper_R:
	case XKB_KEY_Meta_L:    case XKB_KEY_Meta_R:
	case XKB_KEY_Mode_switch:
	case XKB_KEY_ISO_Level3_Shift:
	case XKB_KEY_ISO_Level5_Shift:
		return true;
	default:
		return false;
	}
}
static bool
LABWC_src_input_keyboard_c_is_modifier(struct wlr_keyboard *wlr_keyboard, uint32_t evdev_keycode)
{
	const xkb_keysym_t *syms = NULL;
	int nr_syms = xkb_state_key_get_syms(wlr_keyboard->xkb_state,
		evdev_keycode + 8, &syms);
	for (int i = 0; i < nr_syms; i++) {
		if (LABWC_src_input_keyboard_c_is_modifier_key(syms[i])) {
			return true;
		}
	}
	return false;
}
static struct LABWC_src_input_keyboard_c_keyinfo
LABWC_src_input_keyboard_c_get_keyinfo(struct wlr_keyboard *wlr_keyboard, uint32_t evdev_keycode)
{
	struct LABWC_src_input_keyboard_c_keyinfo keyinfo = {0};
	keyinfo.xkb_keycode = evdev_keycode + 8;
	keyinfo.translated.nr_syms = xkb_state_key_get_syms(
		wlr_keyboard->xkb_state, keyinfo.xkb_keycode,
		&keyinfo.translated.syms);
	xkb_layout_index_t layout_index = xkb_state_key_get_layout(
		wlr_keyboard->xkb_state, keyinfo.xkb_keycode);
	keyinfo.raw.nr_syms = xkb_keymap_key_get_syms_by_level(
		wlr_keyboard->keymap, keyinfo.xkb_keycode, layout_index, 0,
		&keyinfo.raw.syms);
	keyinfo.modifiers = wlr_keyboard_get_modifiers(wlr_keyboard);
	keyinfo.is_modifier = false;
	for (int i = 0; i < keyinfo.translated.nr_syms; i++) {
		keyinfo.is_modifier |=
			LABWC_src_input_keyboard_c_is_modifier_key(keyinfo.translated.syms[i]);
	}
	return keyinfo;
}
static enum LABWC_src_input_keyboard_c_lab_key_handled
LABWC_src_input_keyboard_c_handle_key_release(uint32_t evdev_keycode)
{
	if (!key_state_corresponding_press_event_was_bound(evdev_keycode)) {
		return LAB_KEY_HANDLED_FALSE;
	}
	if (LABWC_src_input_keyboard_c_should_cancel_cycling_on_next_key_release) {
		LABWC_src_input_keyboard_c_should_cancel_cycling_on_next_key_release = false;
		if (server.active_view && server.active_view->link.next != &server.views) {
			struct wl_list *next = server.active_view->link.next;
			struct view *v = wl_container_of(next, v, link);
			desktop_focus_view(v, true);
		} else if (!wl_list_empty(&server.views)) {
			struct view *v = wl_container_of(server.views.next, v, link);
			desktop_focus_view(v, true);
		}
	}
	key_state_bound_key_remove(evdev_keycode);
	return LAB_KEY_HANDLED_TRUE;
}
static bool
LABWC_src_input_keyboard_c_handle_change_vt_key(struct keyboard *keyboard,
		struct LABWC_src_input_keyboard_c_keysyms *translated)
{
	for (int i = 0; i < translated->nr_syms; i++) {
		unsigned int vt =
			translated->syms[i] - XKB_KEY_XF86Switch_VT_1 + 1;
		if (vt >= 1 && vt <= 12) {
			keyboard_cancel_keybind_repeat(keyboard);
			LABWC_src_input_keyboard_c_change_vt(vt);
			return true;
		}
	}
	return false;
}
static void
LABWC_src_input_keyboard_c_handle_menu_keys(struct LABWC_src_input_keyboard_c_keysyms *syms)
{
	assert(server.input_mode == LAB_INPUT_STATE_MENU);
	for (int i = 0; i < syms->nr_syms; i++) {
		switch (syms->syms[i]) {
		case XKB_KEY_Down:
			menu_item_select_next();
			break;
		case XKB_KEY_Up:
			menu_item_select_previous();
			break;
		case XKB_KEY_Right:
			menu_submenu_enter();
			break;
		case XKB_KEY_Left:
			menu_submenu_leave();
			break;
		case XKB_KEY_Return:
		case XKB_KEY_KP_Enter:
			if (!menu_call_selected_actions()) {
				menu_submenu_enter();
			};
			break;
		case XKB_KEY_Escape:
			menu_close();
			cursor_update_focus();
			break;
		default: {
			uint32_t accelerator = xkb_keysym_to_utf32(syms->syms[i]);
			if (accelerator == 0) {
				continue;
			}
			if (menu_item_select_by_accelerator(accelerator)) {
				menu_call_selected_actions();
			}
			break;
		}}
		break;
	}
}
static enum LABWC_src_input_keyboard_c_lab_key_handled
LABWC_src_input_keyboard_c_handle_compositor_keybindings(struct keyboard *keyboard,
		struct wlr_keyboard_key_event *event)
{
	struct wlr_keyboard *wlr_keyboard = keyboard->wlr_keyboard;
	struct LABWC_src_input_keyboard_c_keyinfo keyinfo = LABWC_src_input_keyboard_c_get_keyinfo(wlr_keyboard, event->keycode);
	bool locked = server.session_lock_manager->locked;
	key_state_set_pressed(event->keycode,
		event->state == WL_KEYBOARD_KEY_STATE_PRESSED);
	if (event->state == WL_KEYBOARD_KEY_STATE_RELEASED) {
		if (LABWC_src_input_keyboard_c_cur_keybind && LABWC_src_input_keyboard_c_cur_keybind->on_release) {
			key_state_bound_key_remove(event->keycode);
			if (locked && !LABWC_src_input_keyboard_c_cur_keybind->allow_when_locked) {
				LABWC_src_input_keyboard_c_cur_keybind = NULL;
				return LAB_KEY_HANDLED_TRUE;
			}
			actions_run(NULL, &LABWC_src_input_keyboard_c_cur_keybind->actions, NULL);
			return LAB_KEY_HANDLED_TRUE;
		} else {
			return LABWC_src_input_keyboard_c_handle_key_release(event->keycode);
		}
	}
	if (LABWC_src_input_keyboard_c_handle_change_vt_key(keyboard, &keyinfo.translated)) {
		key_state_store_pressed_key_as_bound(event->keycode);
		return LAB_KEY_HANDLED_TRUE_AND_VT_CHANGED;
	}
	if (!locked) {
		if (server.input_mode == LAB_INPUT_STATE_MENU) {
			key_state_store_pressed_key_as_bound(event->keycode);
			LABWC_src_input_keyboard_c_handle_menu_keys(&keyinfo.translated);
			return LAB_KEY_HANDLED_TRUE;
		} else if (false) {
		}
	}
	LABWC_src_input_keyboard_c_cur_keybind = LABWC_src_input_keyboard_c_match_keybinding(&keyinfo, keyboard->is_virtual);
	if (LABWC_src_input_keyboard_c_cur_keybind && (!locked || LABWC_src_input_keyboard_c_cur_keybind->allow_when_locked)) {
		key_state_store_pressed_key_as_bound(event->keycode);
		if (!LABWC_src_input_keyboard_c_cur_keybind->on_release) {
			actions_run(NULL, &LABWC_src_input_keyboard_c_cur_keybind->actions, NULL);
		}
		return LAB_KEY_HANDLED_TRUE;
	}
	return LAB_KEY_HANDLED_FALSE;
}
static int
LABWC_src_input_keyboard_c_handle_keybind_repeat(void *data)
{
	struct keyboard *keyboard = data;
	assert(keyboard->keybind_repeat);
	assert(keyboard->keybind_repeat_rate > 0);
	struct wlr_keyboard_key_event event = {
		.keycode = keyboard->keybind_repeat_keycode,
		.state = WL_KEYBOARD_KEY_STATE_PRESSED
	};
	LABWC_src_input_keyboard_c_handle_compositor_keybindings(keyboard, &event);
	int next_repeat_ms = 1000 / keyboard->keybind_repeat_rate;
	wl_event_source_timer_update(keyboard->keybind_repeat,
		next_repeat_ms);
	return 0;
}
static void
LABWC_src_input_keyboard_c_start_keybind_repeat(struct keyboard *keyboard,
		struct wlr_keyboard_key_event *event)
{
	struct wlr_keyboard *wlr_keyboard = keyboard->wlr_keyboard;
	assert(!keyboard->keybind_repeat);
	if (wlr_keyboard->repeat_info.rate > 0
			&& wlr_keyboard->repeat_info.delay > 0) {
		keyboard->keybind_repeat_keycode = event->keycode;
		keyboard->keybind_repeat_rate = wlr_keyboard->repeat_info.rate;
		keyboard->keybind_repeat = wl_event_loop_add_timer(
			server.wl_event_loop, LABWC_src_input_keyboard_c_handle_keybind_repeat, keyboard);
		wl_event_source_timer_update(keyboard->keybind_repeat,
			wlr_keyboard->repeat_info.delay);
	}
}
void
keyboard_cancel_keybind_repeat(struct keyboard *keyboard)
{
	if (keyboard->keybind_repeat) {
		wl_event_source_remove(keyboard->keybind_repeat);
		keyboard->keybind_repeat = NULL;
	}
}
static void
LABWC_src_input_keyboard_c_handle_key(struct wl_listener *listener, void *data)
{
	struct keyboard *keyboard = wl_container_of(listener, keyboard, key);
	struct seat *seat = keyboard->base.seat;
	struct wlr_keyboard_key_event *event = data;
	struct wlr_seat *wlr_seat = seat->wlr_seat;
	key_state_indicator_update(seat);
	idle_manager_notify_activity(seat->wlr_seat);
	keyboard_cancel_keybind_repeat(keyboard);
	enum LABWC_src_input_keyboard_c_lab_key_handled handled =
		LABWC_src_input_keyboard_c_handle_compositor_keybindings(keyboard, event);
	if (handled == LAB_KEY_HANDLED_TRUE_AND_VT_CHANGED) {
		return;
	}
	if (handled) {
		if (!LABWC_src_input_keyboard_c_is_modifier(keyboard->wlr_keyboard, event->keycode)
				&& event->state == WL_KEYBOARD_KEY_STATE_PRESSED) {
			LABWC_src_input_keyboard_c_start_keybind_repeat(keyboard, event);
		}
	} else if (!input_method_keyboard_grab_forward_key(keyboard, event)) {
		wlr_seat_set_keyboard(wlr_seat, keyboard->wlr_keyboard);
		wlr_seat_keyboard_notify_key(wlr_seat, event->time_msec,
			event->keycode, event->state);
	}
}
void
keyboard_set_numlock(struct wlr_keyboard *keyboard)
{
	if (rc.kb_numlock_enable == LAB_STATE_UNSPECIFIED) {
		return;
	}
	xkb_mod_index_t num_idx =
		xkb_map_mod_get_index(keyboard->keymap, XKB_MOD_NAME_NUM);
	if (num_idx == XKB_MOD_INVALID) {
		return;
	}
	xkb_mod_mask_t locked = keyboard->modifiers.locked;
	if (rc.kb_numlock_enable == LAB_STATE_ENABLED) {
		locked |= (xkb_mod_mask_t)1 << num_idx;
	} else if (rc.kb_numlock_enable == LAB_STATE_DISABLED) {
		locked &= ~((xkb_mod_mask_t)1 << num_idx);
	}
	wlr_keyboard_notify_modifiers(keyboard, keyboard->modifiers.depressed,
		keyboard->modifiers.latched, locked, keyboard->modifiers.group);
}
void
keyboard_update_layout(struct seat *seat, xkb_layout_index_t layout)
{
	assert(seat);
	struct input *input;
	struct keyboard *keyboard;
	struct wlr_keyboard *kb = NULL;
	wl_list_for_each(input, &seat->inputs, link) {
		if (input->wlr_input_device->type != WLR_INPUT_DEVICE_KEYBOARD) {
			continue;
		}
		keyboard = (struct keyboard *)input;
		if (keyboard->is_virtual) {
			continue;
		}
		kb = keyboard->wlr_keyboard;
		break;
	}
	if (!kb) {
		return;
	}
	if (kb->modifiers.group == layout) {
		return;
	}
	wlr_keyboard_notify_modifiers(kb, kb->modifiers.depressed,
		kb->modifiers.latched, kb->modifiers.locked, layout);
}
static void
LABWC_src_input_keyboard_c_reset_window_keyboard_layout_groups(void)
{
	if (!rc.kb_layout_per_window) {
		return;
	}
	struct view *view;
	for_each_view(view, &server.views, LAB_VIEW_CRITERIA_NONE) {
		view->keyboard_layout = 0;
	}
	struct view *active_view = server.active_view;
	if (!active_view) {
		return;
	}
	keyboard_update_layout(&server.seat, active_view->keyboard_layout);
}
static void
LABWC_src_input_keyboard_c_set_layout(struct wlr_keyboard *kb)
{
	static bool fallback_mode;
	struct xkb_rule_names rules = { 0 };
	enum xkb_context_flags ctx_flags = XKB_CONTEXT_NO_FLAGS;
#ifdef __ANDROID__
	ctx_flags |= XKB_CONTEXT_NO_SECURE_GETENV;
#endif
	struct xkb_context *context = xkb_context_new(ctx_flags);
	struct xkb_keymap *keymap = xkb_map_new_from_names(context, &rules,
		XKB_KEYMAP_COMPILE_NO_FLAGS);
	const char *layout = getenv("XKB_DEFAULT_LAYOUT");
	bool layout_empty = layout && !*layout;
	if (keymap && !layout_empty) {
		if (!wlr_keyboard_keymaps_match(kb->keymap, keymap)) {
			wlr_keyboard_set_keymap(kb, keymap);
			LABWC_src_input_keyboard_c_reset_window_keyboard_layout_groups();
		}
		xkb_keymap_unref(keymap);
	} else {
		if (!fallback_mode) {
			fallback_mode = true;
			setenv("XKB_DEFAULT_LAYOUT", "us", 1);
			LABWC_src_input_keyboard_c_set_layout(kb);
		}
	}
	xkb_context_unref(context);
}
void
keyboard_configure(struct seat *seat, struct wlr_keyboard *kb, bool is_virtual)
{
	if (!is_virtual) {
		LABWC_src_input_keyboard_c_set_layout(kb);
	}
	wlr_keyboard_set_repeat_info(kb, rc.repeat_rate, rc.repeat_delay);
	keybind_update_keycodes();
}
void
keyboard_group_init(struct seat *seat)
{
	if (seat->keyboard_group) {
		return;
	}
	seat->keyboard_group = wlr_keyboard_group_create();
	keyboard_configure(seat, &seat->keyboard_group->keyboard,
		  false);
}
void
keyboard_setup_handlers(struct keyboard *keyboard)
{
	CONNECT_SIGNAL(keyboard->wlr_keyboard, keyboard, key);
	CONNECT_SIGNAL(keyboard->wlr_keyboard, keyboard, modifiers);
}
void
keyboard_group_finish(struct seat *seat)
{
	if (seat->keyboard_group) {
		wlr_keyboard_group_destroy(seat->keyboard_group);
		seat->keyboard_group = NULL;
	}
}
#undef handle_key
#undef handle_modifiers
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>
#define LAB_LAYERSHELL_VERSION 4
static void
LABWC_src_layers_c_apply_override(struct output *output, struct wlr_box *usable_area)
{
	struct usable_area_override *override;
	wl_list_for_each(override, &rc.usable_area_overrides, link) {
		if (override->output && strcasecmp(override->output, output->wlr_output->name)) {
			continue;
		}
		usable_area->x += override->margin.left;
		usable_area->y += override->margin.top;
		usable_area->width -= override->margin.left + override->margin.right;
		usable_area->height -= override->margin.top + override->margin.bottom;
	}
}
static void
LABWC_src_layers_c_arrange_one_layer(const struct wlr_box *full_area, struct wlr_box *usable_area,
		struct wlr_scene_tree *tree, bool exclusive)
{
	struct wlr_scene_node *node;
	wl_list_for_each(node, &tree->children, link) {
		struct lab_layer_surface *surface = node_layer_surface_from_node(node);
		struct wlr_scene_layer_surface_v1 *scene = surface->scene_layer_surface;
		if (!scene->layer_surface->initialized) {
			continue;
		}
		if (surface->being_unmapped) {
			continue;
		}
		if (!!scene->layer_surface->current.exclusive_zone != exclusive) {
			continue;
		}
		wlr_scene_layer_surface_v1_configure(scene, full_area, usable_area);
	}
}
void
layers_arrange(struct output *output)
{
	assert(output);
	struct wlr_box full_area = { 0 };
	wlr_output_effective_resolution(output->wlr_output,
		&full_area.width, &full_area.height);
	struct wlr_box usable_area = full_area;
	LABWC_src_layers_c_apply_override(output, &usable_area);
	struct wlr_scene_output *scene_output =
		wlr_scene_get_scene_output(server.scene, output->wlr_output);
	if (!scene_output) {
		return;
	}
	for (int i = ARRAY_SIZE(output->layer_tree) - 1; i >= 0; i--) {
		struct wlr_scene_tree *layer = output->layer_tree[i];
		LABWC_src_layers_c_arrange_one_layer(&full_area, &usable_area, layer,   true);
	}
	for (size_t i = 0; i < ARRAY_SIZE(output->layer_tree); i++) {
		struct wlr_scene_tree *layer = output->layer_tree[i];
		LABWC_src_layers_c_arrange_one_layer(&full_area, &usable_area, layer,   false);
		wlr_scene_node_set_position(&layer->node, scene_output->x,
			scene_output->y);
	}
	output->usable_area = usable_area;
}
static void
LABWC_src_layers_c_handle_output_destroy(struct wl_listener *listener, void *data)
{
	struct lab_layer_surface *layer =
		wl_container_of(listener, layer, output_destroy);
	layer->scene_layer_surface->layer_surface->output = NULL;
	wlr_layer_surface_v1_destroy(layer->scene_layer_surface->layer_surface);
}
static inline bool
LABWC_src_layers_c_has_exclusive_interactivity(struct wlr_scene_layer_surface_v1 *scene)
{
	return scene->layer_surface->current.keyboard_interactive
		== ZWLR_LAYER_SURFACE_V1_KEYBOARD_INTERACTIVITY_EXCLUSIVE;
}
static void
LABWC_src_layers_c_try_to_focus_next_layer_or_toplevel(void)
{
	struct seat *seat = &server.seat;
	struct output *output = output_nearest_to_cursor();
	if (!output_is_usable(output)) {
		goto no_output;
	}
	enum zwlr_layer_shell_v1_layer overlay = ZWLR_LAYER_SHELL_V1_LAYER_OVERLAY;
	enum zwlr_layer_shell_v1_layer top = ZWLR_LAYER_SHELL_V1_LAYER_TOP;
	for (size_t i = overlay; i >= top; i--) {
		struct wlr_scene_tree *tree = output->layer_tree[i];
		struct wlr_scene_node *node;
		wl_list_for_each_reverse(node, &tree->children, link) {
			struct lab_layer_surface *layer = node_layer_surface_from_node(node);
			struct wlr_scene_layer_surface_v1 *scene = layer->scene_layer_surface;
			struct wlr_layer_surface_v1 *layer_surface = scene->layer_surface;
			if (!layer_surface->surface->mapped) {
				continue;
			}
			if (LABWC_src_layers_c_has_exclusive_interactivity(scene)) {
				seat_set_focus_layer(seat, layer_surface);
				return;
			}
		}
	}
no_output:
	if (seat->focused_layer) {
		seat_set_focus_layer(seat, NULL);
	}
}
static bool
LABWC_src_layers_c_focused_layer_has_exclusive_interactivity(struct seat *seat)
{
	if (!seat->focused_layer) {
		return false;
	}
	return seat->focused_layer->current.keyboard_interactive ==
		ZWLR_LAYER_SURFACE_V1_KEYBOARD_INTERACTIVITY_EXCLUSIVE;
}
static bool
LABWC_src_layers_c_has_precedence(struct seat *seat, enum zwlr_layer_shell_v1_layer layer)
{
	if (!seat->focused_layer) {
		return true;
	}
	if (!LABWC_src_layers_c_focused_layer_has_exclusive_interactivity(seat)) {
		return true;
	}
	if (layer >= seat->focused_layer->current.layer) {
		return true;
	}
	return false;
}
void
layer_try_set_focus(struct seat *seat, struct wlr_layer_surface_v1 *layer_surface)
{
	switch (layer_surface->current.keyboard_interactive) {
	case ZWLR_LAYER_SURFACE_V1_KEYBOARD_INTERACTIVITY_EXCLUSIVE:
		if (LABWC_src_layers_c_has_precedence(seat, layer_surface->current.layer)) {
			seat_set_focus_layer(seat, layer_surface);
		}
		break;
	case ZWLR_LAYER_SURFACE_V1_KEYBOARD_INTERACTIVITY_ON_DEMAND:
		if (!LABWC_src_layers_c_focused_layer_has_exclusive_interactivity(seat)) {
			seat_set_focus_layer(seat, layer_surface);
		}
		break;
	case ZWLR_LAYER_SURFACE_V1_KEYBOARD_INTERACTIVITY_NONE:
		if (seat->focused_layer == layer_surface) {
			LABWC_src_layers_c_try_to_focus_next_layer_or_toplevel();
		}
		break;
	}
}
static bool
LABWC_src_layers_c_is_on_demand(struct wlr_layer_surface_v1 *layer_surface)
{
	return layer_surface->current.keyboard_interactive ==
		ZWLR_LAYER_SURFACE_V1_KEYBOARD_INTERACTIVITY_ON_DEMAND;
}
static void
LABWC_src_layers_c_handle_surface_commit(struct wl_listener *listener, void *data)
{
	struct lab_layer_surface *layer =
		wl_container_of(listener, layer, surface_commit);
	struct wlr_layer_surface_v1 *layer_surface =
		layer->scene_layer_surface->layer_surface;
	struct wlr_output *wlr_output =
		layer->scene_layer_surface->layer_surface->output;
	if (!wlr_output) {
		return;
	}
	uint32_t committed = layer_surface->current.committed;
	struct output *output = (struct output *)wlr_output->data;
	if (committed & WLR_LAYER_SURFACE_V1_STATE_LAYER) {
		wlr_scene_node_reparent(&layer->scene_layer_surface->tree->node,
			output->layer_tree[layer_surface->current.layer]);
	}
	if (committed & WLR_LAYER_SURFACE_V1_STATE_KEYBOARD_INTERACTIVITY) {
		if (LABWC_src_layers_c_is_on_demand(layer_surface)) {
			struct seat *seat = &server.seat;
			if (seat->focused_layer == layer_surface) {
				LABWC_src_layers_c_try_to_focus_next_layer_or_toplevel();
			}
			goto out;
		}
		struct seat *seat = &server.seat;
		layer_try_set_focus(seat, layer_surface);
	}
out:
	if (committed || layer->mapped != layer_surface->surface->mapped) {
		layer->mapped = layer_surface->surface->mapped;
		output_update_usable_area(output);
		cursor_update_focus();
	}
}
static void
LABWC_src_layers_c_handle_node_destroy(struct wl_listener *listener, void *data)
{
	struct lab_layer_surface *layer =
		wl_container_of(listener, layer, node_destroy);
	struct seat *seat = &server.seat;
	if (layer->layer_surface == seat->focused_layer) {
		seat_set_focus_layer(seat, NULL);
	}
	layer->scene_layer_surface = NULL;
	struct wlr_xdg_popup *popup, *tmp;
	wl_list_for_each_safe(popup, tmp, &layer->layer_surface->popups, link) {
		wlr_xdg_popup_destroy(popup);
	}
	wl_list_remove(&layer->map.link);
	wl_list_remove(&layer->unmap.link);
	wl_list_remove(&layer->surface_commit.link);
	wl_list_remove(&layer->new_popup.link);
	wl_list_remove(&layer->output_destroy.link);
	wl_list_remove(&layer->node_destroy.link);
	free(layer);
}
static void
LABWC_src_layers_c_handle_unmap(struct wl_listener *listener, void *data)
{
	struct lab_layer_surface *layer = wl_container_of(listener, layer, unmap);
	struct wlr_layer_surface_v1 *layer_surface =
		layer->scene_layer_surface->layer_surface;
	layer->being_unmapped = true;
	if (layer_surface->output) {
		output_update_usable_area(layer_surface->output->data);
	}
	struct seat *seat = &server.seat;
	if (seat->focused_layer == layer_surface) {
		LABWC_src_layers_c_try_to_focus_next_layer_or_toplevel();
	}
	cursor_update_focus();
	layer->being_unmapped = false;
}
static bool
LABWC_src_layers_c_is_above_toplevels(struct wlr_layer_surface_v1 *layer_surface)
{
	return layer_surface->current.layer >= ZWLR_LAYER_SHELL_V1_LAYER_TOP;
}
static void
LABWC_src_layers_c_handle_map(struct wl_listener *listener, void *data)
{
	struct lab_layer_surface *layer = wl_container_of(listener, layer, map);
	struct wlr_layer_surface_v1 *layer_surface =
		layer->scene_layer_surface->layer_surface;
	struct wlr_output *wlr_output = layer_surface->output;
	if (wlr_output) {
		output_update_usable_area(wlr_output->data);
	}
	if (!LABWC_src_layers_c_is_above_toplevels(layer_surface) && LABWC_src_layers_c_is_on_demand(layer_surface)) {
		return;
	}
	struct seat *seat = &server.seat;
	layer_try_set_focus(seat, layer->scene_layer_surface->layer_surface);
}
static bool
LABWC_src_layers_c_surface_is_focused(struct seat *seat, struct wlr_surface *surface)
{
	return seat->wlr_seat->keyboard_state.focused_surface == surface;
}
static void
LABWC_src_layers_c_handle_popup_destroy(struct wl_listener *listener, void *data)
{
	struct lab_layer_popup *popup =
		wl_container_of(listener, popup, destroy);
	struct seat *seat = &server.seat;
	struct wlr_xdg_popup *_popup, *tmp;
	wl_list_for_each_safe(_popup, tmp, &popup->wlr_popup->base->popups, link) {
		wlr_xdg_popup_destroy(_popup);
	}
	wl_list_remove(&popup->destroy.link);
	wl_list_remove(&popup->new_popup.link);
	wl_list_remove(&popup->reposition.link);
	if (popup->commit.notify) {
		wl_list_remove(&popup->commit.link);
	}
	if (LABWC_src_layers_c_surface_is_focused(seat, popup->wlr_popup->base->surface)) {
		if (popup->parent_was_focused && popup->wlr_popup->parent) {
			seat_force_focus_surface(seat, popup->wlr_popup->parent);
		} else {
			desktop_focus_topmost_view();
		}
	}
	cursor_update_focus();
	free(popup);
}
static void
LABWC_src_layers_c_handle_popup_commit(struct wl_listener *listener, void *data)
{
	struct lab_layer_popup *popup =
		wl_container_of(listener, popup, commit);
	if (popup->wlr_popup->base->initial_commit) {
		wlr_xdg_popup_unconstrain_from_box(popup->wlr_popup,
			&popup->output_toplevel_sx_box);
		wl_list_remove(&popup->commit.link);
		popup->commit.notify = NULL;
		struct seat *seat = &server.seat;
		bool requesting_grab = !!popup->wlr_popup->seat;
		if (requesting_grab) {
			if (LABWC_src_layers_c_surface_is_focused(seat, popup->wlr_popup->parent)) {
				popup->parent_was_focused = true;
			}
			seat_force_focus_surface(seat, popup->wlr_popup->base->surface);
		}
	}
}
static void
LABWC_src_layers_c_handle_popup_reposition(struct wl_listener *listener, void *data)
{
	struct lab_layer_popup *popup =
		wl_container_of(listener, popup, reposition);
	wlr_xdg_popup_unconstrain_from_box(popup->wlr_popup,
		&popup->output_toplevel_sx_box);
}
static void LABWC_src_layers_c_handle_popup_new_popup(struct wl_listener *listener, void *data);
static struct lab_layer_popup *
LABWC_src_layers_c_create_popup(struct wlr_xdg_popup *wlr_popup,
		struct wlr_scene_tree *parent)
{
	struct lab_layer_popup *popup = znew(*popup);
	popup->wlr_popup = wlr_popup;
	popup->scene_tree =
		wlr_scene_xdg_surface_create(parent, wlr_popup->base);
	die_if_null(popup->scene_tree);
	wlr_popup->base->surface->data = popup->scene_tree;
	node_descriptor_create(&popup->scene_tree->node,
		LAB_NODE_LAYER_POPUP,   NULL, popup);
	popup->destroy.notify = LABWC_src_layers_c_handle_popup_destroy;
	wl_signal_add(&wlr_popup->events.destroy, &popup->destroy);
	popup->new_popup.notify = LABWC_src_layers_c_handle_popup_new_popup;
	wl_signal_add(&wlr_popup->base->events.new_popup, &popup->new_popup);
	popup->commit.notify = LABWC_src_layers_c_handle_popup_commit;
	wl_signal_add(&wlr_popup->base->surface->events.commit, &popup->commit);
	popup->reposition.notify = LABWC_src_layers_c_handle_popup_reposition;
	wl_signal_add(&wlr_popup->events.reposition, &popup->reposition);
	return popup;
}
static void
LABWC_src_layers_c_handle_popup_new_popup(struct wl_listener *listener, void *data)
{
	struct lab_layer_popup *lab_layer_popup =
		wl_container_of(listener, lab_layer_popup, new_popup);
	struct wlr_xdg_popup *wlr_popup = data;
	struct lab_layer_popup *new_popup = LABWC_src_layers_c_create_popup(wlr_popup,
		lab_layer_popup->scene_tree);
	new_popup->output_toplevel_sx_box =
		lab_layer_popup->output_toplevel_sx_box;
}
static void
LABWC_src_layers_c_move_popup_to_top_layer(struct lab_layer_surface *toplevel,
		struct lab_layer_popup *popup)
{
	struct wlr_output *wlr_output =
		toplevel->scene_layer_surface->layer_surface->output;
	struct output *output = (struct output *)wlr_output->data;
	struct wlr_box box = { 0 };
	wlr_output_layout_get_box(server.output_layout, wlr_output, &box);
	int lx = toplevel->scene_layer_surface->tree->node.x + box.x;
	int ly = toplevel->scene_layer_surface->tree->node.y + box.y;
	struct wlr_scene_node *node = &popup->scene_tree->node;
	wlr_scene_node_reparent(node, output->layer_popup_tree);
	wlr_scene_node_set_position(&output->layer_popup_tree->node, lx, ly);
}
static void
LABWC_src_layers_c_handle_new_popup(struct wl_listener *listener, void *data)
{
	struct lab_layer_surface *toplevel =
		wl_container_of(listener, toplevel, new_popup);
	struct wlr_xdg_popup *wlr_popup = data;
	struct wlr_scene_layer_surface_v1 *surface = toplevel->scene_layer_surface;
	struct output *output = surface->layer_surface->output->data;
	int lx, ly;
	wlr_scene_node_coords(&surface->tree->node, &lx, &ly);
	struct wlr_box output_box = { 0 };
	wlr_output_layout_get_box(server.output_layout,
		output->wlr_output, &output_box);
	struct wlr_box output_toplevel_sx_box = {
		.x = output_box.x - lx,
		.y = output_box.y - ly,
		.width = output_box.width,
		.height = output_box.height,
	};
	struct lab_layer_popup *popup =
		LABWC_src_layers_c_create_popup(wlr_popup, surface->tree);
	popup->output_toplevel_sx_box = output_toplevel_sx_box;
	if (surface->layer_surface->current.layer
			<= ZWLR_LAYER_SHELL_V1_LAYER_BOTTOM) {
		LABWC_src_layers_c_move_popup_to_top_layer(toplevel, popup);
	}
}
static void
LABWC_src_layers_c_handle_new_layer_surface(struct wl_listener *listener, void *data)
{
	struct wlr_layer_surface_v1 *layer_surface = data;
	if (!layer_surface->output) {
		struct output *output = output_nearest_to_cursor();
		if (!output || !output->scene_output) {
			wlr_layer_surface_v1_destroy(layer_surface);
			return;
		}
		layer_surface->output = output->wlr_output;
	}
	struct lab_layer_surface *surface = znew(*surface);
	surface->layer_surface = layer_surface;
	struct output *output = layer_surface->output->data;
	struct wlr_scene_tree *selected_layer =
		output->layer_tree[layer_surface->current.layer];
	surface->scene_layer_surface = wlr_scene_layer_surface_v1_create(
		selected_layer, layer_surface);
	die_if_null(surface->scene_layer_surface);
	layer_surface->surface->data = surface->scene_layer_surface->tree;
	node_descriptor_create(&surface->scene_layer_surface->tree->node,
		LAB_NODE_LAYER_SURFACE,   NULL, surface);
	surface->scene_layer_surface->layer_surface = layer_surface;
	surface->surface_commit.notify = LABWC_src_layers_c_handle_surface_commit;
	wl_signal_add(&layer_surface->surface->events.commit,
		&surface->surface_commit);
	surface->map.notify = LABWC_src_layers_c_handle_map;
	wl_signal_add(&layer_surface->surface->events.map, &surface->map);
	surface->unmap.notify = LABWC_src_layers_c_handle_unmap;
	wl_signal_add(&layer_surface->surface->events.unmap, &surface->unmap);
	surface->new_popup.notify = LABWC_src_layers_c_handle_new_popup;
	wl_signal_add(&layer_surface->events.new_popup, &surface->new_popup);
	surface->output_destroy.notify = LABWC_src_layers_c_handle_output_destroy;
	wl_signal_add(&layer_surface->output->events.destroy,
		&surface->output_destroy);
	surface->node_destroy.notify = LABWC_src_layers_c_handle_node_destroy;
	wl_signal_add(&surface->scene_layer_surface->tree->node.events.destroy,
		&surface->node_destroy);
}
void
layers_init(void)
{
	server.layer_shell = wlr_layer_shell_v1_create(server.wl_display,
		LAB_LAYERSHELL_VERSION);
	server.new_layer_surface.notify = LABWC_src_layers_c_handle_new_layer_surface;
	wl_signal_add(&server.layer_shell->events.new_surface,
		&server.new_layer_surface);
}
void
layers_finish(void)
{
	wl_list_remove(&server.new_layer_surface.link);
}
#undef LAB_LAYERSHELL_VERSION
#include <getopt.h>
#include <pango/pangocairo.h>
#include <signal.h>
#include <unistd.h>
struct rcxml rc = { 0 };
struct server server = { 0 };
static const struct option LABWC_src_main_c_long_options[] = {
	{"config-dir", required_argument, NULL, 'C'},
	{"exit", no_argument, NULL, 'e'},
	{"help", no_argument, NULL, 'h'},
	{"startup", required_argument, NULL, 's'},
	{"session", required_argument, NULL, 'S'},
	{"title", required_argument, NULL, 't'},
	{"version", no_argument, NULL, 'v'},
	{0, 0, 0, 0}
};
static const char LABWC_src_main_c_labwc_usage[] =
"Usage: jtlabwc [options...]\n"
"  -C, --config-dir <dir>   Specify config directory\n"
"  -e, --exit               Exit the compositor\n"
"  -h, --help               Show help message and quit\n"
"  -s, --startup <command>  Run command on startup\n"
"  -S, --session <command>  Run command on startup and terminate on exit\n"
"  -t, --title <fmtstr>     Specify title to use when running in a window\n"
"  -v, --version            Show version number and quit\n";
static void
LABWC_src_main_c_usage(void)
{
	printf("%s", LABWC_src_main_c_labwc_usage);
	exit(0);
}
static void
LABWC_src_main_c_print_version(void)
{
	#define FEATURE_ENABLED(feature) (HAVE_##feature ? "+" : "-")
	printf("jtlabwc %s (%sxwayland) wlroots-%d.%d.%d\n",
		LABWC_VERSION,
		FEATURE_ENABLED(XWAYLAND),
		wlr_version_get_major(),
		wlr_version_get_minor(),
		wlr_version_get_micro()
	);
	#undef FEATURE_ENABLED
}
static void
LABWC_src_main_c_die_on_detecting_suid(void)
{
	if (geteuid() != 0 && getegid() != 0) {
		return;
	}
	if (getuid() == geteuid() && getgid() == getegid()) {
		return;
	}
	exit(EXIT_FAILURE);
}
static void
LABWC_src_main_c_die_on_no_fonts(void)
{
	PangoContext *context = pango_font_map_create_context(
		pango_cairo_font_map_get_default());
	PangoLayout *layout = pango_layout_new(context);
	pango_layout_set_text(layout, "abcdefg", -1);
	int nr_unknown_glyphs = pango_layout_get_unknown_glyphs_count(layout);
	g_object_unref(layout);
	g_object_unref(context);
	if (nr_unknown_glyphs > 0) {
		exit(EXIT_FAILURE);
	}
	pango_cairo_font_map_set_default(NULL);
}
static void
LABWC_src_main_c_send_signal_to_labwc_pid(int signal)
{
	char *labwc_pid = getenv("LABWC_PID");
	if (!labwc_pid) {
		exit(EXIT_FAILURE);
	}
	int pid = atoi(labwc_pid);
	if (!pid) {
		exit(EXIT_FAILURE);
	}
	kill(pid, signal);
}
struct LABWC_src_main_c_idle_ctx {
	const char *primary_client;
	const char *startup_cmd;
};
static void
LABWC_src_main_c_idle_callback(void *data)
{
	struct LABWC_src_main_c_idle_ctx *ctx = data;
	if (ctx->primary_client) {
		server.primary_client_pid = spawn_primary_client(ctx->primary_client);
		if (server.primary_client_pid < 0) {
			wl_display_terminate(server.wl_display);
			return;
		}
	}
	session_autostart_init();
	if (ctx->startup_cmd) {
		spawn_async_no_shell(ctx->startup_cmd);
	}
}
int
main(int argc, char *argv[])
{
	char *startup_cmd = NULL;
	char *primary_client = NULL;
	server.wlr_version = _LAB_CALC_WLR_VERSION_NUM(
		wlr_version_get_major(),
		wlr_version_get_minor(),
		wlr_version_get_micro()
	);
	int c;
	while (1) {
		int index = 0;
		c = getopt_long(argc, argv, "C:ehs:S:t:v", LABWC_src_main_c_long_options, &index);
		if (c == -1) {
			break;
		}
		switch (c) {
		case 'C':
			rc.config_dir = optarg;
			break;
		case 'e':
			LABWC_src_main_c_send_signal_to_labwc_pid(SIGTERM);
			exit(0);
		case 's':
			startup_cmd = optarg;
			break;
		case 'S':
			primary_client = optarg;
			break;
		case 't':
			server.title_fmt = optarg;
			break;
		case 'v':
			LABWC_src_main_c_print_version();
			exit(0);
		case 'h':
		default:
			LABWC_src_main_c_usage();
		}
	}
	if (optind < argc) {
		LABWC_src_main_c_usage();
	}
	wlr_log_init(WLR_SILENT, NULL);
	LABWC_src_main_c_die_on_detecting_suid();
	LABWC_src_main_c_die_on_no_fonts();
	session_environment_init();
	rcxml_init();
	char pid[32];
	snprintf(pid, sizeof(pid), "%d", getpid());
	if (setenv("LABWC_PID", pid, true) < 0) {
	} else {
	}
	if (setenv("LABWC_VER", LABWC_VERSION, true) < 0) {
	} else {
	}
	if (!getenv("XDG_RUNTIME_DIR")) {
		exit(EXIT_FAILURE);
	}
	increase_nofile_limit();
	if (string_null_or_empty(server.title_fmt)) {
		server.title_fmt = "jtlabwc - %o";
	}
	server_init();
	server_start();
	struct theme theme = { 0 };
	theme_init(&theme);
	rc.theme = &theme;
	jtlabwc_control_init();
	menu_init();
	struct LABWC_src_main_c_idle_ctx idle_ctx = {
		.primary_client = primary_client,
		.startup_cmd = startup_cmd
	};
	wl_event_loop_add_idle(server.wl_event_loop, LABWC_src_main_c_idle_callback, &idle_ctx);
	wl_display_run(server.wl_display);
	session_shutdown();
	jtlabwc_control_finish();
	menu_finish();
	theme_finish(&theme);
	rcxml_finish();
	font_finish();
	server_finish();
	return 0;
}
#undef FEATURE_ENABLED
#include <assert.h>
#include <stdlib.h>
static void
LABWC_src_node_c_handle_node_destroy(struct wl_listener *listener, void *data)
{
	struct node_descriptor *node_descriptor =
		wl_container_of(listener, node_descriptor, destroy);
	if (node_type_contains(LAB_NODE_BUTTON, node_descriptor->type)) {
		ssd_button_free(node_descriptor->data);
	}
	wl_list_remove(&node_descriptor->destroy.link);
	free(node_descriptor);
}
void
node_descriptor_create(struct wlr_scene_node *scene_node,
		enum lab_node_type type, struct view *view, void *data)
{
	struct node_descriptor *node_descriptor = znew(*node_descriptor);
	node_descriptor->type = type;
	node_descriptor->view = view;
	node_descriptor->data = data;
	node_descriptor->destroy.notify = LABWC_src_node_c_handle_node_destroy;
	wl_signal_add(&scene_node->events.destroy, &node_descriptor->destroy);
	scene_node->data = node_descriptor;
}
struct view *
node_view_from_node(struct wlr_scene_node *wlr_scene_node)
{
	assert(wlr_scene_node->data);
	struct node_descriptor *node_descriptor = wlr_scene_node->data;
	return node_descriptor->view;
}
struct lab_layer_surface *
node_layer_surface_from_node(struct wlr_scene_node *wlr_scene_node)
{
	assert(wlr_scene_node->data);
	struct node_descriptor *node_descriptor = wlr_scene_node->data;
	assert(node_descriptor->type == LAB_NODE_LAYER_SURFACE);
	return (struct lab_layer_surface *)node_descriptor->data;
}
struct menuitem *
node_menuitem_from_node(struct wlr_scene_node *wlr_scene_node)
{
	assert(wlr_scene_node->data);
	struct node_descriptor *node_descriptor = wlr_scene_node->data;
	assert(node_descriptor->type == LAB_NODE_MENUITEM);
	return (struct menuitem *)node_descriptor->data;
}
struct ssd_button *
node_try_ssd_button_from_node(struct wlr_scene_node *wlr_scene_node)
{
	assert(wlr_scene_node->data);
	struct node_descriptor *node_descriptor = wlr_scene_node->data;
	if (node_type_contains(LAB_NODE_BUTTON, node_descriptor->type)) {
		return (struct ssd_button *)node_descriptor->data;
	}
	return NULL;
}
void
output_state_init(struct output *output)
{
	wlr_output_state_init(&output->pending);
	struct wlr_output_configuration_v1 *backup_config =
		wlr_output_configuration_v1_create();
	struct wlr_output_configuration_head_v1 *backup_head =
		wlr_output_configuration_head_v1_create(
			backup_config, output->wlr_output);
	wlr_output_head_v1_state_apply(&backup_head->state, &output->pending);
	wlr_output_configuration_v1_destroy(backup_config);
}
bool
output_state_commit(struct output *output)
{
	bool committed =
		wlr_output_commit_state(output->wlr_output, &output->pending);
	if (committed) {
		wlr_output_state_finish(&output->pending);
		wlr_output_state_init(&output->pending);
	} else {
	}
	return committed;
}
#include <stdlib.h>
static struct wlr_output *LABWC_src_output_virtual_c_fallback_output = NULL;
void
output_virtual_add(const char *output_name,
		struct wlr_output **store_wlr_output)
{
	if (output_name) {
		struct output *output;
		wl_list_for_each(output, &server.outputs, link) {
			if (wlr_output_is_headless(output->wlr_output) &&
					!strcmp(output->wlr_output->name, output_name)) {
				return;
			}
		}
	}
	wl_list_remove(&server.new_output.link);
	struct wlr_output *wlr_output = wlr_headless_add_output(
		server.headless.backend, 1920, 1080);
	if (!wlr_output) {
		goto restore_handler;
	}
	if (output_name) {
		wlr_output_set_name(wlr_output, output_name);
	}
	if (store_wlr_output) {
		*store_wlr_output = wlr_output;
	}
	if (server.new_output.notify) {
		server.new_output.notify(&server.new_output, wlr_output);
	}
restore_handler:
	wl_signal_add(&server.backend->events.new_output, &server.new_output);
}
void
output_virtual_remove(const char *output_name)
{
	struct output *output;
	wl_list_for_each(output, &server.outputs, link) {
		if (!wlr_output_is_headless(output->wlr_output)
				|| output->wlr_output == LABWC_src_output_virtual_c_fallback_output) {
			continue;
		}
		if (output_name) {
			if (!strcmp(output->wlr_output->name, output_name)) {
				wlr_output_destroy(output->wlr_output);
				return;
			}
		} else {
			wlr_output_destroy(output->wlr_output);
			return;
		}
	}
}
void
output_virtual_update_fallback(void)
{
	struct wl_list *layout_outputs = &server.output_layout->outputs;
	const char *fallback_output_name = getenv("LABWC_FALLBACK_OUTPUT");
	if (!LABWC_src_output_virtual_c_fallback_output && wl_list_empty(layout_outputs)
			&& !string_null_or_empty(fallback_output_name)) {
		output_virtual_add(fallback_output_name, &LABWC_src_output_virtual_c_fallback_output);
	} else if (LABWC_src_output_virtual_c_fallback_output && (wl_list_length(layout_outputs) > 1
			|| string_null_or_empty(fallback_output_name))) {
		struct wlr_output *wlr_output = LABWC_src_output_virtual_c_fallback_output;
		LABWC_src_output_virtual_c_fallback_output = NULL;
		wlr_output_destroy(wlr_output);
	}
}
#include <assert.h>
#include <glib.h>
#include <strings.h>
#if WLR_HAS_DRM_BACKEND
#else
	#define wlr_output_is_drm(output) (false)
#endif
#if WLR_HAS_SESSION
#endif
bool
output_get_tearing_allowance(struct output *output)
{
#ifdef FULLSCREEN_TEARING
	if (!rc.allow_tearing) {
		return false;
	}
	struct view *view = server.active_view;
	if (!view || view->output != output) {
		return false;
	}
	if (rc.allow_tearing == LAB_TEARING_ENABLED) {
		if (view->force_tearing == LAB_STATE_UNSPECIFIED) {
			return view->tearing_hint;
		} else {
			return view->force_tearing == LAB_STATE_ENABLED;
		}
	}
	if (!view->fullscreen) {
		return false;
	}
	if (view->force_tearing == LAB_STATE_UNSPECIFIED) {
		return view->tearing_hint ||
			rc.allow_tearing == LAB_TEARING_FULLSCREEN_FORCED;
	}
	return view->force_tearing == LAB_STATE_ENABLED;
#else
	(void)output;
	return false;
#endif
}
static void
LABWC_src_output_c_handle_output_frame(struct wl_listener *listener, void *data)
{
	struct output *output = wl_container_of(listener, output, frame);
	if (!output_is_usable(output)) {
		return;
	}
#if WLR_HAS_SESSION
	if (server.session && !server.session->active) {
		return;
	}
#endif
	struct wlr_scene_output *scene_output = output->scene_output;
	struct wlr_output_state *pending = &output->pending;
	pending->tearing_page_flip = output_get_tearing_allowance(output);
	lab_wlr_scene_output_commit(scene_output, pending);
	struct timespec now = { 0 };
	clock_gettime(CLOCK_MONOTONIC, &now);
	wlr_scene_output_send_frame_done(output->scene_output, &now);
}
static void
LABWC_src_output_c_handle_output_destroy(struct wl_listener *listener, void *data)
{
	struct output *output = wl_container_of(listener, output, destroy);
	struct seat *seat = &server.seat;
	regions_evacuate_output(output);
	regions_destroy(seat, &output->regions);
	if (seat->overlay.active.output == output) {
		overlay_finish(seat);
	}
	if (output->scene_output) {
		wlr_scene_output_destroy(output->scene_output);
		output->scene_output = NULL;
	}
	wl_list_remove(&output->link);
	wl_list_remove(&output->frame.link);
	wl_list_remove(&output->destroy.link);
	wl_list_remove(&output->request_state.link);
	seat_output_layout_changed(seat);
	for (size_t i = 0; i < ARRAY_SIZE(output->layer_tree); i++) {
		wlr_scene_node_destroy(&output->layer_tree[i]->node);
	}
	wlr_scene_node_destroy(&output->layer_popup_tree->node);
	wlr_scene_node_destroy(&output->session_lock_tree->node);
	if (output->workspace_osd) {
		wlr_scene_node_destroy(&output->workspace_osd->node);
		output->workspace_osd = NULL;
	}
	struct view *view;
	wl_list_for_each(view, &server.views, link) {
		if (view->output == output) {
			view_on_output_destroy(view);
		}
	}
	wlr_output_state_finish(&output->pending);
	output->wlr_output->data = NULL;
	if (wl_list_empty(&server.outputs) && wlr_output_is_wl(output->wlr_output)) {
		wl_display_terminate(server.wl_display);
	}
	free(output);
}
static void
LABWC_src_output_c_handle_output_request_state(struct wl_listener *listener, void *data)
{
	struct output *output = wl_container_of(listener, output, request_state);
	const struct wlr_output_event_request_state *event = data;
	if (event->state->committed == WLR_OUTPUT_STATE_MODE) {
		switch (event->state->mode_type) {
		case WLR_OUTPUT_STATE_MODE_FIXED:
			wlr_output_state_set_mode(&output->pending,
				event->state->mode);
			break;
		case WLR_OUTPUT_STATE_MODE_CUSTOM:
			wlr_output_state_set_custom_mode(&output->pending,
				event->state->custom_mode.width,
				event->state->custom_mode.height,
				event->state->custom_mode.refresh);
			break;
		}
		wlr_output_schedule_frame(output->wlr_output);
		return;
	}
	if (!wlr_output_commit_state(output->wlr_output, event->state)) {
	}
}
static void LABWC_src_output_c_do_output_layout_change(void);
static void
LABWC_src_output_c_add_output_to_layout(struct output *output)
{
	struct wlr_output *wlr_output = output->wlr_output;
	struct wlr_output_layout_output *layout_output =
		wlr_output_layout_add_auto(server.output_layout, wlr_output);
	if (!layout_output) {
		return;
	}
	if (!output->scene_output) {
		output->scene_output =
			wlr_scene_output_create(server.scene, wlr_output);
		if (!output->scene_output) {
			return;
		}
		wlr_scene_output_layout_add_output(server.scene_layout,
			layout_output, output->scene_output);
	}
	wlr_ext_workspace_group_handle_v1_output_enter(
		server.workspaces.ext_group, output->wlr_output);
	regions_reconfigure_output(output);
	if (server.session_lock_manager->locked) {
		session_lock_output_create(server.session_lock_manager, output);
	}
}
static bool
LABWC_src_output_c_output_test_auto(struct wlr_output *wlr_output, struct wlr_output_state *state,
		bool is_client_request)
{
	if (is_client_request && (state->mode
			|| state->custom_mode.width
			|| state->custom_mode.height
			|| state->custom_mode.refresh)) {
		if (state->mode) {
		} else {
		}
		return wlr_output_test_state(wlr_output, state);
	}
	if (rc.reuse_output_mode && wlr_output->current_mode) {
		if (wlr_output_test_state(wlr_output, state)) {
			return true;
		}
	}
	struct wlr_output_mode *preferred_mode =
		wlr_output_preferred_mode(wlr_output);
	if (preferred_mode) {
		wlr_output_state_set_mode(state, preferred_mode);
		if (wlr_output_test_state(wlr_output, state)) {
			return true;
		}
	}
	struct wlr_output_mode *mode;
	wl_list_for_each(mode, &wlr_output->modes, link) {
		if (mode == preferred_mode) {
			continue;
		}
		wlr_output_state_set_mode(state, mode);
		if (wlr_output_test_state(wlr_output, state)) {
			return true;
		}
	}
	state->committed &= ~(uint32_t)WLR_OUTPUT_STATE_MODE;
	return wlr_output_test_state(wlr_output, state);
}
static void
LABWC_src_output_c_configure_new_output(struct output *output)
{
	struct wlr_output *wlr_output = output->wlr_output;
	wlr_output_state_set_enabled(&output->pending, true);
	if (!LABWC_src_output_c_output_test_auto(wlr_output, &output->pending,
			  false)) {
		wlr_output_state_set_enabled(&output->pending, false);
		return;
	}
	server.pending_output_layout_change++;
	LABWC_src_output_c_add_output_to_layout(output);
	server.pending_output_layout_change--;
	wlr_output_schedule_frame(wlr_output);
	lab_wlr_scene_output_commit(output->scene_output, &output->pending);
	wlr_output_effective_resolution(wlr_output,
		&output->usable_area.width, &output->usable_area.height);
}
static uint64_t
LABWC_src_output_c_get_unused_output_id_bit(void)
{
	uint64_t used_id_bits = 0;
	struct output *output;
	wl_list_for_each(output, &server.outputs, link) {
		used_id_bits |= output->id_bit;
	}
	if (used_id_bits == UINT64_MAX) {
		return 0;
	}
	uint64_t id_bit = server.next_output_id_bit;
	assert(__builtin_popcountll(id_bit) == 1);
	while ((id_bit & used_id_bits) != 0) {
		id_bit = (id_bit << 1) | (id_bit >> 63);
	}
	server.next_output_id_bit = (id_bit << 1) | (id_bit >> 63);
	return id_bit;
}
static void
LABWC_src_output_c_handle_new_output(struct wl_listener *listener, void *data)
{
	struct wlr_output *wlr_output = data;
	struct output *output;
	wl_list_for_each(output, &server.outputs, link) {
		if (output->wlr_output == wlr_output) {
			return;
		}
	}
	uint64_t id_bit = LABWC_src_output_c_get_unused_output_id_bit();
	if (!id_bit) {
		return;
	}
	if (wlr_output_is_wl(wlr_output)) {
		GString *title = g_string_new(server.title_fmt);
		g_string_replace(title, "%o", wlr_output->name, 0);
		wlr_wl_output_set_title(wlr_output, title->str);
		g_string_free(title, TRUE);
		wlr_wl_output_set_app_id(wlr_output, "jtlabwc");
	}
#if WLR_HAS_DRM_BACKEND
	if (server.drm_lease_manager && wlr_output_is_drm(wlr_output)) {
		wlr_drm_lease_v1_manager_offer_output(
			server.drm_lease_manager, wlr_output);
	}
#endif
	if (wlr_output->non_desktop) {
		return;
	}
	if (!wlr_output_init_render(wlr_output, server.allocator,
			server.renderer)) {
		return;
	}
	output = znew(*output);
	output->wlr_output = wlr_output;
	wlr_output->data = output;
	output->id_bit = id_bit;
	output_state_init(output);
	wl_list_insert(&server.outputs, &output->link);
	output->destroy.notify = LABWC_src_output_c_handle_output_destroy;
	wl_signal_add(&wlr_output->events.destroy, &output->destroy);
	output->frame.notify = LABWC_src_output_c_handle_output_frame;
	wl_signal_add(&wlr_output->events.frame, &output->frame);
	output->request_state.notify = LABWC_src_output_c_handle_output_request_state;
	wl_signal_add(&wlr_output->events.request_state, &output->request_state);
	wl_list_init(&output->regions);
	for (size_t i = 0; i < ARRAY_SIZE(output->layer_tree); i++) {
		output->layer_tree[i] =
			lab_wlr_scene_tree_create(&server.scene->tree);
	}
	output->layer_popup_tree = lab_wlr_scene_tree_create(&server.scene->tree);
	output->session_lock_tree = lab_wlr_scene_tree_create(&server.scene->tree);
	wlr_scene_node_lower_to_bottom(&output->layer_tree[1]->node);
	wlr_scene_node_lower_to_bottom(&output->layer_tree[0]->node);
	struct wlr_scene_node *menu_node = &server.menu_tree->node;
	wlr_scene_node_place_below(&output->layer_tree[2]->node, menu_node);
	wlr_scene_node_place_below(&output->layer_tree[3]->node, menu_node);
	wlr_scene_node_place_below(&output->layer_popup_tree->node, menu_node);
	wlr_scene_node_raise_to_top(&output->session_lock_tree->node);
	if (rc.auto_enable_outputs || !wlr_output_is_drm(wlr_output)) {
		LABWC_src_output_c_configure_new_output(output);
	}
	LABWC_src_output_c_do_output_layout_change();
}
static void LABWC_src_output_c_output_manager_init(void);
void
output_init(void)
{
	server.gamma_control_manager_v1 =
		wlr_gamma_control_manager_v1_create(server.wl_display);
	wlr_scene_set_gamma_control_manager_v1(server.scene,
		server.gamma_control_manager_v1);
	server.new_output.notify = LABWC_src_output_c_handle_new_output;
	wl_signal_add(&server.backend->events.new_output, &server.new_output);
	server.output_layout = wlr_output_layout_create(server.wl_display);
	if (!server.output_layout) {
		exit(EXIT_FAILURE);
	}
	server.scene_layout = wlr_scene_attach_output_layout(server.scene,
		server.output_layout);
	if (!server.scene_layout) {
		exit(EXIT_FAILURE);
	}
	wlr_xdg_output_manager_v1_create(server.wl_display,
		server.output_layout);
	wl_list_init(&server.outputs);
	server.next_output_id_bit = (1 << 0);
	LABWC_src_output_c_output_manager_init();
}
static void LABWC_src_output_c_output_manager_finish(void);
void
output_finish(void)
{
	wl_list_remove(&server.new_output.link);
	LABWC_src_output_c_output_manager_finish();
}
static void
LABWC_src_output_c_output_update_for_layout_change(void)
{
	output_update_all_usable_areas(  true);
	session_lock_update_for_layout_change();
	wlr_cursor_move(server.seat.cursor, NULL, 0, 0);
	cursor_update_image(&server.seat);
}
static bool
LABWC_src_output_c_output_config_apply(struct wlr_output_configuration_v1 *config)
{
	bool success = true;
	server.pending_output_layout_change++;
	struct wlr_output_configuration_head_v1 *head;
	wl_list_for_each(head, &config->heads, link) {
		struct wlr_output *o = head->state.output;
		struct output *output = output_from_wlr_output(o);
		struct wlr_output_state *os = &output->pending;
		bool output_enabled = head->state.enabled;
		wlr_output_state_set_enabled(os, output_enabled);
		if (output_enabled) {
			if (head->state.mode) {
				wlr_output_state_set_mode(os, head->state.mode);
			} else {
				wlr_output_state_set_custom_mode(os,
					head->state.custom_mode.width,
					head->state.custom_mode.height,
					head->state.custom_mode.refresh);
			}
			(void)LABWC_src_output_c_output_test_auto(o, os,
				  true);
			wlr_output_state_set_scale(os, head->state.scale);
			wlr_output_state_set_transform(os, head->state.transform);
		}
		if (!output_state_commit(output)) {
			success = false;
			break;
		}
		bool was_in_layout =
			!!wlr_output_layout_get(server.output_layout, o);
		if (output_enabled) {
			if (!was_in_layout) {
				LABWC_src_output_c_add_output_to_layout(output);
			}
			struct wlr_box pos = {0};
			wlr_output_layout_get_box(server.output_layout, o, &pos);
			if (pos.x != head->state.x || pos.y != head->state.y) {
				wlr_output_layout_add(server.output_layout, o,
					head->state.x, head->state.y);
			}
		} else if (was_in_layout) {
			regions_evacuate_output(output);
			wlr_ext_workspace_group_handle_v1_output_leave(
				server.workspaces.ext_group, output->wlr_output);
			wlr_scene_output_destroy(output->scene_output);
			wlr_output_layout_remove(server.output_layout, o);
			output->scene_output = NULL;
		}
	}
	server.pending_output_layout_change--;
	LABWC_src_output_c_do_output_layout_change();
	return success;
}
static bool
LABWC_src_output_c_verify_output_config_v1(const struct wlr_output_configuration_v1 *config)
{
	const char *err_msg = NULL;
	struct wlr_output_configuration_head_v1 *head;
	wl_list_for_each(head, &config->heads, link) {
		if (!head->state.enabled) {
			continue;
		}
		if (!head->state.mode) {
			int32_t refresh = head->state.custom_mode.refresh;
			if (wlr_output_is_wl(head->state.output) && refresh != 0) {
				err_msg = "Wayland backend refresh rates unsupported";
				goto custom_mode_failed;
			}
		}
		struct wlr_output_state output_state;
		wlr_output_state_init(&output_state);
		wlr_output_head_v1_state_apply(&head->state, &output_state);
		if (!LABWC_src_output_c_output_test_auto(head->state.output, &output_state,
				  true)) {
			wlr_output_state_finish(&output_state);
			return false;
		}
		wlr_output_state_finish(&output_state);
	}
	return true;
custom_mode_failed:
	assert(err_msg);
	return false;
}
static void
LABWC_src_output_c_handle_output_manager_test(struct wl_listener *listener, void *data)
{
	struct wlr_output_configuration_v1 *config = data;
	if (LABWC_src_output_c_verify_output_config_v1(config)) {
		wlr_output_configuration_v1_send_succeeded(config);
	} else {
		wlr_output_configuration_v1_send_failed(config);
	}
	wlr_output_configuration_v1_destroy(config);
}
static void
LABWC_src_output_c_handle_output_manager_apply(struct wl_listener *listener, void *data)
{
	struct wlr_output_configuration_v1 *config = data;
	bool config_is_good = LABWC_src_output_c_verify_output_config_v1(config);
	if (config_is_good && LABWC_src_output_c_output_config_apply(config)) {
		wlr_output_configuration_v1_send_succeeded(config);
	} else {
		wlr_output_configuration_v1_send_failed(config);
	}
	wlr_output_configuration_v1_destroy(config);
	struct output *output;
	wl_list_for_each(output, &server.outputs, link) {
		wlr_xcursor_manager_load(server.seat.xcursor_manager,
			output->wlr_output->scale);
	}
	cursor_update_focus();
	cursor_update_image(&server.seat);
}
static struct
wlr_output_configuration_v1 *LABWC_src_output_c_create_output_config(void)
{
	struct wlr_output_configuration_v1 *config =
		wlr_output_configuration_v1_create();
	if (!config) {
		return NULL;
	}
	struct output *output;
	wl_list_for_each(output, &server.outputs, link) {
		struct wlr_output_configuration_head_v1 *head =
			wlr_output_configuration_head_v1_create(config,
				output->wlr_output);
		if (!head) {
			wlr_output_configuration_v1_destroy(config);
			return NULL;
		}
		if (output_is_usable(output)) {
			head->state.x = output->scene_output->x;
			head->state.y = output->scene_output->y;
		}
	}
	return config;
}
static void
LABWC_src_output_c_do_output_layout_change(void)
{
	if (!server.pending_output_layout_change) {
		struct wlr_output_configuration_v1 *config =
			LABWC_src_output_c_create_output_config();
		if (config) {
			wlr_output_manager_v1_set_configuration(
				server.output_manager, config);
		} else {
		}
		LABWC_src_output_c_output_update_for_layout_change();
		seat_output_layout_changed(&server.seat);
	}
}
static void
LABWC_src_output_c_handle_output_layout_change(struct wl_listener *listener, void *data)
{
	server.pending_output_layout_change++;
	output_virtual_update_fallback();
	server.pending_output_layout_change--;
	LABWC_src_output_c_do_output_layout_change();
}
static void
LABWC_src_output_c_output_manager_init(void)
{
	server.output_manager = wlr_output_manager_v1_create(server.wl_display);
	server.output_layout_change.notify = LABWC_src_output_c_handle_output_layout_change;
	wl_signal_add(&server.output_layout->events.change,
		&server.output_layout_change);
	server.output_manager_apply.notify = LABWC_src_output_c_handle_output_manager_apply;
	wl_signal_add(&server.output_manager->events.apply,
		&server.output_manager_apply);
	server.output_manager_test.notify = LABWC_src_output_c_handle_output_manager_test;
	wl_signal_add(&server.output_manager->events.test,
		&server.output_manager_test);
}
static void
LABWC_src_output_c_output_manager_finish(void)
{
	wl_list_remove(&server.output_layout_change.link);
	wl_list_remove(&server.output_manager_apply.link);
	wl_list_remove(&server.output_manager_test.link);
}
struct output *
output_from_wlr_output(struct wlr_output *wlr_output)
{
	struct output *output;
	wl_list_for_each(output, &server.outputs, link) {
		if (output->wlr_output == wlr_output) {
			return output;
		}
	}
	return NULL;
}
struct output *
output_from_name(const char *name)
{
	struct output *output;
	wl_list_for_each(output, &server.outputs, link) {
		if (!output_is_usable(output) || !output->wlr_output->name) {
			continue;
		}
		if (!strcasecmp(name, output->wlr_output->name)) {
			return output;
		}
	}
	return NULL;
}
struct output *
output_nearest_to(int lx, int ly)
{
	double closest_x, closest_y;
	wlr_output_layout_closest_point(server.output_layout, NULL, lx, ly,
		&closest_x, &closest_y);
	return output_from_wlr_output(wlr_output_layout_output_at(server.output_layout,
			closest_x, closest_y));
}
struct output *
output_nearest_to_cursor(void)
{
	return output_nearest_to((int)server.seat.cursor->x,
		(int)server.seat.cursor->y);
}
struct output *
output_get_adjacent(struct output *output, enum lab_edge edge, bool wrap)
{
	if (!output_is_usable(output)) {
		return NULL;
	}
	if (!lab_edge_is_cardinal(edge)) {
		return NULL;
	}
	struct wlr_box box = output_usable_area_in_layout_coords(output);
	int lx = box.x + box.width / 2;
	int ly = box.y + box.height / 2;
	struct wlr_output *new_output = NULL;
	struct wlr_output *current_output = output->wlr_output;
	struct wlr_output_layout *layout = server.output_layout;
	new_output = wlr_output_layout_adjacent_output(layout,
		(enum wlr_direction)edge, current_output, lx, ly);
	if (wrap && !new_output) {
		enum lab_edge opposite = lab_edge_invert(edge);
		new_output = wlr_output_layout_farthest_output(layout,
			(enum wlr_direction)opposite, current_output, lx, ly);
	}
	if (!new_output || new_output == current_output) {
		return NULL;
	}
	output = output_from_wlr_output(new_output);
	if (!output_is_usable(output)) {
		return NULL;
	}
	return output;
}
bool
output_is_usable(struct output *output)
{
	return output && output->wlr_output->enabled && output->scene_output;
}
static bool
LABWC_src_output_c_update_usable_area(struct output *output)
{
	struct wlr_box old = output->usable_area;
	layers_arrange(output);
#if HAVE_XWAYLAND
	struct view *view;
	wl_list_for_each(view, &server.views, link) {
		if (view->mapped && view->type == LAB_XWAYLAND_VIEW) {
			xwayland_adjust_usable_area(view,
				server.output_layout,
				output->wlr_output, &output->usable_area);
		}
	}
#endif
	return !wlr_box_equal(&old, &output->usable_area);
}
void
output_update_usable_area(struct output *output)
{
	if (LABWC_src_output_c_update_usable_area(output)) {
		regions_update_geometry(output);
#if HAVE_XWAYLAND
		xwayland_update_workarea();
#endif
		desktop_arrange_all_views();
	}
}
void
output_update_all_usable_areas(bool layout_changed)
{
	bool usable_area_changed = false;
	struct output *output;
	wl_list_for_each(output, &server.outputs, link) {
		if (LABWC_src_output_c_update_usable_area(output)) {
			usable_area_changed = true;
			regions_update_geometry(output);
		} else if (layout_changed) {
			regions_update_geometry(output);
		}
	}
	if (usable_area_changed || layout_changed) {
#if HAVE_XWAYLAND
		xwayland_update_workarea();
#endif
		desktop_arrange_all_views();
	}
}
struct wlr_box
output_usable_area_in_layout_coords(struct output *output)
{
	if (!output_is_usable(output)) {
		return (struct wlr_box){0};
	}
	struct wlr_box box = output->usable_area;
	double ox = 0, oy = 0;
	wlr_output_layout_output_coords(server.output_layout,
		output->wlr_output, &ox, &oy);
	box.x = (int)(box.x - ox);
	box.y = (int)(box.y - oy);
	return box;
}
void
handle_output_power_manager_set_mode(struct wl_listener *listener, void *data)
{
	struct wlr_output_power_v1_set_mode_event *event = data;
	struct output *output = event->output->data;
	assert(output);
	switch (event->mode) {
	case ZWLR_OUTPUT_POWER_V1_MODE_OFF:
		if (!event->output->enabled) {
			return;
		}
		wlr_output_state_set_enabled(&output->pending, false);
		output_state_commit(output);
		break;
	case ZWLR_OUTPUT_POWER_V1_MODE_ON:
		if (event->output->enabled) {
			return;
		}
		wlr_output_state_set_enabled(&output->pending, true);
		output_state_commit(output);
		cursor_update_image(&server.seat);
		break;
	}
}
#undef wlr_output_is_drm
#include <assert.h>
static void
LABWC_src_overlay_c_show_overlay(struct seat *seat, struct theme_snapping_overlay *overlay_theme,
		struct wlr_box *box)
{
	struct view *view = server.grabbed_view;
	assert(view);
	assert(!seat->overlay.rect);
	struct lab_scene_rect_options opts = {
		.width = box->width,
		.height = box->height,
	};
	if (overlay_theme->bg_enabled) {
		opts.bg_color = overlay_theme->bg_color;
	}
	float *border_colors[3] = {
		overlay_theme->border_color[0],
		overlay_theme->border_color[1],
		overlay_theme->border_color[2],
	};
	if (overlay_theme->border_enabled) {
		opts.border_colors = border_colors;
		opts.nr_borders = 3;
		opts.border_width = overlay_theme->border_width;
	}
	seat->overlay.rect =
		lab_scene_rect_create(server.workspace_tree, &opts);
	struct wlr_scene_node *node = &seat->overlay.rect->tree->node;
	wlr_scene_node_raise_to_top(node);
	wlr_scene_node_set_position(node, box->x, box->y);
}
static void
LABWC_src_overlay_c_show_region_overlay(struct seat *seat, struct region *region)
{
	if (region == seat->overlay.active.region) {
		return;
	}
	overlay_finish(seat);
	seat->overlay.active.region = region;
	struct wlr_box geo = view_get_region_snap_box(NULL, region);
	LABWC_src_overlay_c_show_overlay(seat, &rc.theme->snapping_overlay_region, &geo);
}
static struct wlr_box
LABWC_src_overlay_c_get_edge_snap_box(enum lab_edge edge, struct output *output)
{
	if (edge == LAB_EDGE_TOP && rc.snap_top_maximize) {
		return output_usable_area_in_layout_coords(output);
	} else {
		return view_get_edge_snap_box(NULL, output, edge);
	}
}
static int
LABWC_src_overlay_c_handle_edge_overlay_timeout(void *data)
{
	struct seat *seat = data;
	assert(seat->overlay.active.edge != LAB_EDGE_NONE
		&& seat->overlay.active.output);
	struct wlr_box box = LABWC_src_overlay_c_get_edge_snap_box(seat->overlay.active.edge,
		seat->overlay.active.output);
	LABWC_src_overlay_c_show_overlay(seat, &rc.theme->snapping_overlay_edge, &box);
	return 0;
}
static bool
LABWC_src_overlay_c_edge_has_adjacent_output_from_cursor(struct seat *seat, struct output *output,
		enum lab_edge edge)
{
	if (!lab_edge_is_cardinal(edge)) {
		return false;
	}
	return wlr_output_layout_adjacent_output(
		server.output_layout, (enum wlr_direction)edge,
		output->wlr_output, seat->cursor->x, seat->cursor->y);
}
static void
LABWC_src_overlay_c_show_edge_overlay(struct seat *seat, enum lab_edge edge1, enum lab_edge edge2,
		struct output *output)
{
	if (!rc.snap_overlay_enabled) {
		return;
	}
	enum lab_edge edge = edge1 | edge2;
	if (seat->overlay.active.edge == edge
			&& seat->overlay.active.output == output) {
		return;
	}
	overlay_finish(seat);
	seat->overlay.active.edge = edge;
	seat->overlay.active.output = output;
	int delay;
	if (LABWC_src_overlay_c_edge_has_adjacent_output_from_cursor(seat, output, edge1)) {
		delay = rc.snap_overlay_delay_inner;
	} else {
		delay = rc.snap_overlay_delay_outer;
	}
	if (delay > 0) {
		if (!seat->overlay.timer) {
			seat->overlay.timer = wl_event_loop_add_timer(
				server.wl_event_loop,
				LABWC_src_overlay_c_handle_edge_overlay_timeout, seat);
		}
		wl_event_source_timer_update(seat->overlay.timer, delay);
	} else {
		LABWC_src_overlay_c_handle_edge_overlay_timeout(seat);
	}
}
void
overlay_update(struct seat *seat)
{
	if (regions_should_snap()) {
		struct region *region = regions_from_cursor();
		if (region) {
			LABWC_src_overlay_c_show_region_overlay(seat, region);
			return;
		}
	}
	struct output *output;
	enum lab_edge edge1, edge2;
	if (edge_from_cursor(seat, &output, &edge1, &edge2)) {
		LABWC_src_overlay_c_show_edge_overlay(seat, edge1, edge2, output);
		return;
	}
	overlay_finish(seat);
}
void
overlay_finish(struct seat *seat)
{
	if (seat->overlay.rect) {
		wlr_scene_node_destroy(&seat->overlay.rect->tree->node);
	}
	if (seat->overlay.timer) {
		wl_event_source_remove(seat->overlay.timer);
	}
	seat->overlay = (struct overlay){0};
}
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#define overlap_bitmap_index(bmp, i, j) \
	(bmp)->grid[(i) * ((bmp)->nr_cols - 1) + (j)]
struct LABWC_src_placement_c_overlap_bitmap {
	int nr_rows;
	int nr_cols;
	int *rows;
	int *cols;
	int *grid;
};
static int
LABWC_src_placement_c_compare_ints(const void *a, const void *b)
{
	return *(const int *)a - *(const int *)b;
}
static void
LABWC_src_placement_c_destroy_bitmap(struct LABWC_src_placement_c_overlap_bitmap *bmp)
{
	assert(bmp);
	zfree(bmp->rows);
	zfree(bmp->cols);
	zfree(bmp->grid);
	bmp->nr_rows = 0;
	bmp->nr_cols = 0;
}
static int
LABWC_src_placement_c_count_views(struct view *view)
{
	assert(view);
	struct output *output = view->output;
	if (!output_is_usable(output)) {
		return 0;
	}
	int nviews = 0;
	struct view *v;
	for_each_view(v, &server.views, LAB_VIEW_CRITERIA_CURRENT_WORKSPACE) {
		if (v == view || v->output != output) {
			continue;
		}
		nviews++;
	}
	return nviews;
}
static int
LABWC_src_placement_c_order_grid(int *edges, int nedges)
{
	qsort(edges, (size_t)nedges, sizeof(int), LABWC_src_placement_c_compare_ints);
	int i = 0;
	int j = 0;
	while (j < nedges) {
		int last = edges[j++];
		edges[i++] = last;
		while (j < nedges && edges[j] == last) {
			++j;
		}
	}
	return i;
}
static void
LABWC_src_placement_c_build_grid(struct LABWC_src_placement_c_overlap_bitmap *bmp, struct view *view)
{
	assert(bmp);
	assert(view);
	LABWC_src_placement_c_destroy_bitmap(bmp);
	struct output *output = view->output;
	if (!output_is_usable(output)) {
		return;
	}
	int nviews = LABWC_src_placement_c_count_views(view);
	if (nviews < 1) {
		return;
	}
	int max_rc = 2 * nviews + 2;
	bmp->rows = xzalloc((size_t)max_rc * sizeof(int));
	bmp->cols = xzalloc((size_t)max_rc * sizeof(int));
	if (!bmp->rows || !bmp->cols) {
		LABWC_src_placement_c_destroy_bitmap(bmp);
		return;
	}
	struct wlr_box usable = output_usable_area_in_layout_coords(output);
	int usable_right = usable.x + usable.width;
	int usable_bottom = usable.y + usable.height;
	bmp->cols[0] = usable.x;
	bmp->rows[0] = usable.y;
	bmp->cols[1] = usable_right;
	bmp->rows[1] = usable_bottom;
	int nr_rows = 2;
	int nr_cols = 2;
	struct view *v;
	for_each_view(v, &server.views, LAB_VIEW_CRITERIA_CURRENT_WORKSPACE) {
		if (v == view || v->output != output) {
			continue;
		}
		struct border margin = ssd_get_margin(v->ssd);
		int x = v->pending.x - margin.left;
		int y = v->pending.y - margin.top;
		if (x > usable.x && x < usable_right) {
			assert(nr_cols < max_rc);
			bmp->cols[nr_cols++] = x;
		}
		if (y > usable.y && y < usable_bottom) {
			assert(nr_rows < max_rc);
			bmp->rows[nr_rows++] = y;
		}
		x = v->pending.x + margin.right + v->pending.width;
		y = v->pending.y + margin.bottom
			+ view_effective_height(v,   true);
		if (x > usable.x && x < usable_right) {
			assert(nr_cols < max_rc);
			bmp->cols[nr_cols++] = x;
		}
		if (y > usable.y && y < usable_bottom) {
			assert(nr_rows < max_rc);
			bmp->rows[nr_rows++] = y;
		}
	}
	bmp->nr_rows = LABWC_src_placement_c_order_grid(bmp->rows, nr_rows);
	bmp->nr_cols = LABWC_src_placement_c_order_grid(bmp->cols, nr_cols);
	int grid_size = (bmp->nr_rows - 1) * (bmp->nr_cols - 1);
	bmp->grid = xzalloc((size_t)grid_size * sizeof(int));
	if (!bmp->grid) {
		LABWC_src_placement_c_destroy_bitmap(bmp);
		return;
	}
}
static int
LABWC_src_placement_c_find_interval(int *edges, int nedges, double val)
{
	int l = 0;
	int r = nedges;
	while (l < r) {
		int m = (l + r) / 2;
		if (edges[m] > val) {
			r = m;
		} else {
			l = m + 1;
		}
	}
	return r - 1;
}
static void
LABWC_src_placement_c_build_overlap(struct LABWC_src_placement_c_overlap_bitmap *bmp, struct view *view)
{
	assert(bmp);
	assert(view);
	if (bmp->nr_rows < 1 || bmp->nr_cols < 1) {
		return;
	}
	struct output *output = view->output;
	if (!output_is_usable(output)) {
		return;
	}
	struct view *v;
	for_each_view(v, &server.views, LAB_VIEW_CRITERIA_CURRENT_WORKSPACE) {
		if (v == view || v->output != output) {
			continue;
		}
		struct border margin = ssd_get_margin(v->ssd);
		int lx = v->pending.x - margin.left;
		int ly = v->pending.y - margin.top;
		int hx = v->pending.x + margin.right + v->pending.width;
		int hy = v->pending.y + margin.bottom
			+ view_effective_height(v,   true);
		int fc = LABWC_src_placement_c_find_interval(bmp->cols, bmp->nr_cols, lx + 0.5);
		int fr = LABWC_src_placement_c_find_interval(bmp->rows, bmp->nr_rows, ly + 0.5);
		fc = MAX(fc, 0);
		fr = MAX(fr, 0);
		int lc = LABWC_src_placement_c_find_interval(bmp->cols, bmp->nr_cols, hx - 0.5);
		int lr = LABWC_src_placement_c_find_interval(bmp->rows, bmp->nr_rows, hy - 0.5);
		lc = MIN(bmp->nr_cols - 1, lc + 1);
		lr = MIN(bmp->nr_rows - 1, lr + 1);
		for (int i = fr; i < lr; ++i) {
			for (int j = fc; j < lc; ++j) {
				overlap_bitmap_index(bmp, i, j) += 1;
			}
		}
	}
}
static int
LABWC_src_placement_c_compute_overlap(struct LABWC_src_placement_c_overlap_bitmap *bmp, int i, int j,
		int width, int height, bool right, bool down, bool *single)
{
	int nri = bmp->nr_rows - 1;
	int nci = bmp->nr_cols - 1;
	int i_incr = down ? 1 : -1;
	int j_incr = right ? 1 : -1;
	int overlap = 0;
	int count = 0;
	for (int ii = i; ii >= 0 && ii < nri && height > 0; ii += i_incr) {
		int rh = bmp->rows[ii + 1] - bmp->rows[ii];
		int mh = MAX(0, MIN(height, rh));
		height -= rh;
		int ww = width;
		for (int jj = j; jj >= 0 && jj < nci && ww > 0; jj += j_incr) {
			int cw = bmp->cols[jj + 1] - bmp->cols[jj];
			int mw = MAX(0, MIN(ww, cw));
			overlap += overlap_bitmap_index(bmp, ii, jj) * mh * mw;
			count++;
			ww -= cw;
		}
		if (ww > 0) {
			overlap = INT_MAX;
			break;
		}
	}
	if (height > 0) {
		overlap = INT_MAX;
	}
	if (single) {
		*single = (count == 1);
	}
	return overlap;
}
bool
placement_find_best(struct view *view, struct wlr_box *geometry)
{
	assert(view);
	struct border margin = ssd_get_margin(view->ssd);
	struct output *output = view->output;
	if (!output_is_usable(output)) {
		return false;
	}
	struct wlr_box usable = output_usable_area_in_layout_coords(output);
	geometry->x = usable.x + margin.left + rc.gap;
	geometry->y = usable.y + margin.top + rc.gap;
	struct LABWC_src_placement_c_overlap_bitmap bmp = { 0 };
	LABWC_src_placement_c_build_grid(&bmp, view);
	LABWC_src_placement_c_build_overlap(&bmp, view);
	int height = geometry->height + margin.top + margin.bottom + 2 * rc.gap;
	int width = geometry->width + margin.left + margin.right + 2 * rc.gap;
	int offset_x = margin.left + rc.gap;
	int offset_y = margin.top + rc.gap;
	int min_overlap = INT_MAX;
	int nri = bmp.nr_rows - 1;
	int nci = bmp.nr_cols - 1;
	for (int i = 0; i < nri; ++i) {
		for (int j = 0; j < nci; ++j) {
			for (int ii = 0; ii < 4; ++ii) {
				bool rt = (ii & 0x1) == 0;
				bool dn = (ii & 0x2) == 0;
				bool single = false;
				int overlap = LABWC_src_placement_c_compute_overlap(&bmp, i, j,
					width, height, rt, dn, &single);
				if (overlap >= min_overlap) {
					continue;
				}
				min_overlap = overlap;
				if (rt) {
					geometry->x = bmp.cols[j] + offset_x;
				} else {
					geometry->x =
						bmp.cols[j + 1] - width + offset_x;
				}
				if (dn) {
					geometry->y = bmp.rows[i] + offset_y;
				} else {
					geometry->y =
						bmp.rows[i + 1] - height + offset_y;
				}
				if (min_overlap <= 0) {
					goto final_placement;
				}
				if (single) {
					break;
				}
			}
		}
	}
final_placement:
	LABWC_src_placement_c_destroy_bitmap(&bmp);
	return true;
}
#undef overlap_bitmap_index
#include <assert.h>
#include <float.h>
#include <math.h>
#include <string.h>
bool
regions_should_snap(void)
{
	if (server.input_mode != LAB_INPUT_STATE_MOVE
			|| wl_list_empty(&rc.regions)
			|| server.seat.region_prevent_snap
			|| !view_is_floating(server.grabbed_view)) {
		return false;
	}
	return keyboard_get_all_modifiers(&server.seat);
}
struct region *
regions_from_name(const char *region_name, struct output *output)
{
	assert(region_name);
	assert(output);
	struct region *region;
	wl_list_for_each(region, &output->regions, link) {
		if (!strcmp(region->name, region_name)) {
			return region;
		}
	}
	return NULL;
}
struct region *
regions_from_cursor(void)
{
	double lx = server.seat.cursor->x;
	double ly = server.seat.cursor->y;
	struct wlr_output *wlr_output = wlr_output_layout_output_at(
		server.output_layout, lx, ly);
	struct output *output = output_from_wlr_output(wlr_output);
	if (!output_is_usable(output)) {
		return NULL;
	}
	double dist;
	double dist_min = DBL_MAX;
	struct region *closest_region = NULL;
	struct region *region;
	wl_list_for_each(region, &output->regions, link) {
		if (wlr_box_contains_point(&region->geo, lx, ly)) {
			dist = pow(region->center.x - lx, 2) + pow(region->center.y - ly, 2);
			if (dist < dist_min) {
				closest_region = region;
				dist_min = dist;
			}
		}
	}
	return closest_region;
}
void
regions_reconfigure_output(struct output *output)
{
	assert(output);
	if (!wl_list_empty(&output->regions)) {
		regions_evacuate_output(output);
		regions_destroy(&server.seat, &output->regions);
	}
	struct region *region;
	wl_list_for_each(region, &rc.regions, link) {
		struct region *region_new = znew(*region_new);
		region_new->output = output;
		region_new->name = xstrdup(region->name);
		region_new->percentage = region->percentage;
		wl_list_append(&output->regions, &region_new->link);
	}
	regions_update_geometry(output);
}
void
regions_reconfigure(void)
{
	struct output *output;
	wl_list_for_each(output, &server.outputs, link) {
		regions_reconfigure_output(output);
	}
	desktop_arrange_all_views();
}
void
regions_update_geometry(struct output *output)
{
	assert(output);
	struct region *region;
	struct wlr_box usable = output_usable_area_in_layout_coords(output);
	struct wlr_box *perc, *geo;
	wl_list_for_each(region, &output->regions, link) {
		geo = &region->geo;
		perc = &region->percentage;
		int left = usable.width * perc->x / 100;
		int right = usable.width * (perc->x + perc->width) / 100;
		int top = usable.height * perc->y / 100;
		int bottom = usable.height * (perc->y + perc->height) / 100;
		geo->x = usable.x + left;
		geo->y = usable.y + top;
		geo->width = right - left;
		geo->height = bottom - top;
		region->center.x = geo->x + geo->width / 2;
		region->center.y = geo->y + geo->height / 2;
	}
}
void
regions_evacuate_output(struct output *output)
{
	assert(output);
	struct view *view;
	wl_list_for_each(view, &server.views, link) {
		if (view->tiled_region && view->tiled_region->output == output) {
			view_evacuate_region(view);
		}
	}
}
void
regions_destroy(struct seat *seat, struct wl_list *regions)
{
	assert(regions);
	struct region *region, *region_tmp;
	wl_list_for_each_safe(region, region_tmp, regions, link) {
		wl_list_remove(&region->link);
		if (seat && seat->overlay.active.region == region) {
			overlay_finish(seat);
		}
		zfree(region->name);
		zfree(region);
	}
}
bool
resize_outlines_enabled(struct view *view)
{
	return view->resize_outlines.rect
		&& view->resize_outlines.rect->tree->node.enabled;
}
void
resize_outlines_update(struct view *view, struct wlr_box new_geo)
{
	struct resize_outlines *outlines = &view->resize_outlines;
	if (!outlines->rect) {
		struct lab_scene_rect_options opts = {
			.border_colors = (float *[3]) {
				rc.theme->osd_bg_color,
				rc.theme->osd_label_text_color,
				rc.theme->osd_bg_color,
			},
			.nr_borders = 3,
			.border_width = 1,
		};
		outlines->rect = lab_scene_rect_create(view->scene_tree, &opts);
	}
	struct border margin = ssd_get_margin(view->ssd);
	struct wlr_box box = {
		.x = new_geo.x - margin.left,
		.y = new_geo.y - margin.top,
		.width = new_geo.width + margin.left + margin.right,
		.height = new_geo.height + margin.top + margin.bottom,
	};
	lab_scene_rect_set_size(outlines->rect, box.width, box.height);
	wlr_scene_node_set_position(&outlines->rect->tree->node,
		box.x - view->current.x, box.y - view->current.y);
	wlr_scene_node_set_enabled(
		&view->resize_outlines.rect->tree->node, true);
	outlines->view_geo = new_geo;
	resize_indicator_update(view);
}
void
resize_outlines_finish(struct view *view)
{
	view_move_resize(view, view->resize_outlines.view_geo);
	wlr_scene_node_set_enabled(
		&view->resize_outlines.rect->tree->node, false);
}
#include <assert.h>
static void
LABWC_src_resistance_c_check_edge(int *next, struct edge current, struct edge target,
		struct edge oppose, struct edge align, int tolerance, bool lesser)
{
	int cur = current.offset;
	int tgt = target.offset;
	int opp = oppose.offset;
	if (cur == tgt) {
		return;
	}
	const bool decreasing = tgt < cur;
	if (tolerance >= 0 && lesser != decreasing) {
		return;
	}
	bool valid = false;
	if (decreasing) {
		const int lo = clipped_sub(opp, abs(tolerance));
		const int hi = clipped_sub(opp, MIN(tolerance, 0));
		valid = tgt >= lo && tgt < hi && cur >= opp;
	} else {
		const int lo = clipped_add(opp, MIN(tolerance, 0));
		const int hi = clipped_add(opp, abs(tolerance));
		valid = tgt > lo && tgt <= hi && cur <= opp;
	}
	if (valid && edges_traverse_edge(current, target, oppose)) {
		*next = edge_get_best(*next, opp, decreasing);
	}
}
static void
LABWC_src_resistance_c_check_edge_output(int *next, struct edge current, struct edge target,
		struct edge oppose, struct edge align, bool lesser)
{
	LABWC_src_resistance_c_check_edge(next, current, target,
		oppose, align, rc.screen_edge_strength, lesser);
}
static void
LABWC_src_resistance_c_check_edge_window(int *next, struct edge current, struct edge target,
		struct edge oppose, struct edge align, bool lesser)
{
	LABWC_src_resistance_c_check_edge(next, current, target,
		oppose, align, rc.window_edge_strength, lesser);
}
bool
resistance_unsnap_apply(struct view *view, int *x, int *y)
{
	if (view_is_floating(view)) {
		return false;
	}
	int dx = *x - view->current.x;
	int dy = *y - view->current.y;
	if (view->maximized == VIEW_AXIS_HORIZONTAL) {
		if (abs(dx) < rc.unmaximize_threshold) {
			*x = view->current.x;
			return false;
		}
	} else if (view->maximized == VIEW_AXIS_VERTICAL) {
		if (abs(dy) < rc.unmaximize_threshold) {
			*y = view->current.y;
			return false;
		}
	} else {
		if (dx * dx + dy * dy < rc.unsnap_threshold * rc.unsnap_threshold) {
			*x = view->current.x;
			*y = view->current.y;
			return false;
		}
	}
	return true;
}
void
resistance_move_apply(struct view *view, int *x, int *y)
{
	assert(view);
	struct border next_edges;
	edges_initialize(&next_edges);
	struct wlr_box target = {
		.x = *x,
		.y = *y,
		.width = view->current.width,
		.height = view->current.height,
	};
	if (rc.screen_edge_strength != 0) {
		edges_find_outputs(&next_edges, view,
			view->current, target, NULL, LABWC_src_resistance_c_check_edge_output);
	}
	if (rc.window_edge_strength != 0) {
		edges_find_neighbors(&next_edges, view, view->current, target,
			NULL, LABWC_src_resistance_c_check_edge_window,   true);
	}
	edges_adjust_move_coords(view, next_edges,
		&target.x, &target.y,   false);
	*x = target.x;
	*y = target.y;
}
void
resistance_resize_apply(struct view *view, struct wlr_box *new_geom)
{
	assert(view);
	assert(!view->shaded);
	struct border next_edges;
	edges_initialize(&next_edges);
	enum lab_edge resize_edges = server.resize_edges;
	struct wlr_box origin =
		snap_constraints_effective(view, resize_edges,   false);
	if (rc.screen_edge_strength != 0) {
		edges_find_outputs(&next_edges, view,
			origin, *new_geom, NULL, LABWC_src_resistance_c_check_edge_output);
	}
	if (rc.window_edge_strength != 0) {
		edges_find_neighbors(&next_edges, view, origin, *new_geom,
			NULL, LABWC_src_resistance_c_check_edge_window,   true);
	}
	edges_adjust_resize_geom(view, next_edges,
		resize_edges, new_geom,   false);
	snap_constraints_set(view, resize_edges, *new_geom);
}
#define handle_focus_change LABWC_src_seat_c_handle_focus_change
#define handle_new_input LABWC_src_seat_c_handle_new_input
#define handle_new_virtual_keyboard LABWC_src_seat_c_handle_new_virtual_keyboard
#define handle_new_virtual_pointer LABWC_src_seat_c_handle_new_virtual_pointer
#include <assert.h>
#include <stdbool.h>
#include <strings.h>
#if WLR_HAS_LIBINPUT_BACKEND
#else
	#define wlr_input_device_is_libinput(device) (false)
#endif
static void
LABWC_src_seat_c_input_device_destroy(struct wl_listener *listener, void *data)
{
	struct input *input = wl_container_of(listener, input, destroy);
	wl_list_remove(&input->link);
	wl_list_remove(&input->destroy.link);
	if (input->wlr_input_device->type == WLR_INPUT_DEVICE_KEYBOARD) {
		struct keyboard *keyboard = (struct keyboard *)input;
		wl_list_remove(&keyboard->key.link);
		wl_list_remove(&keyboard->modifiers.link);
		keyboard_cancel_keybind_repeat(keyboard);
	}
	free(input);
}
#if WLR_HAS_LIBINPUT_BACKEND
static enum lab_libinput_device_type
LABWC_src_seat_c_device_type_from_wlr_device(struct wlr_input_device *wlr_input_device)
{
	if (wlr_input_device->type == WLR_INPUT_DEVICE_POINTER &&
			wlr_input_device_is_libinput(wlr_input_device)) {
		struct libinput_device *libinput_device =
			wlr_libinput_get_device_handle(wlr_input_device);
		if (libinput_device_config_tap_get_finger_count(libinput_device) > 0) {
			return LAB_LIBINPUT_DEVICE_TOUCHPAD;
		}
	}
	return LAB_LIBINPUT_DEVICE_NON_TOUCH;
}
static struct libinput_category *
LABWC_src_seat_c_get_category(struct wlr_input_device *device)
{
	struct libinput_category *category;
	wl_list_for_each_reverse(category, &rc.libinput_categories, link) {
		if (category->name) {
			if (!strcasecmp(device->name, category->name)) {
				return category;
			}
		}
	}
	enum lab_libinput_device_type type = LABWC_src_seat_c_device_type_from_wlr_device(device);
	wl_list_for_each_reverse(category, &rc.libinput_categories, link) {
		if (category->type == type) {
			return category;
		}
	}
	return libinput_category_get_default();
}
#endif
static void
LABWC_src_seat_c_configure_libinput(struct wlr_input_device *wlr_input_device)
{
	if (!wlr_input_device) {
		return;
	}
	struct input *input = wlr_input_device->data;
	assert(input);
	if (!wlr_input_device_is_libinput(wlr_input_device)) {
		input->scroll_factor = 1.0;
		return;
	}
#if WLR_HAS_LIBINPUT_BACKEND
	struct libinput_device *libinput_dev =
		wlr_libinput_get_device_handle(wlr_input_device);
	if (!libinput_dev) {
		return;
	}
	struct libinput_category *dc = LABWC_src_seat_c_get_category(wlr_input_device);
	assert(dc);
	libinput_device_config_tap_set_enabled(libinput_dev,
		libinput_device_config_tap_get_default_enabled(libinput_dev));
	libinput_device_config_tap_set_button_map(libinput_dev,
		libinput_device_config_tap_get_default_button_map(libinput_dev));
	if (libinput_device_config_tap_get_finger_count(libinput_dev) <= 0) {
	} else {
		libinput_device_config_tap_set_enabled(libinput_dev, dc->tap);
		libinput_device_config_tap_set_button_map(libinput_dev,
			dc->tap_button_map);
	}
	libinput_device_config_tap_set_drag_enabled(libinput_dev,
		libinput_device_config_tap_get_default_drag_enabled(libinput_dev));
	if (libinput_device_config_tap_get_finger_count(libinput_dev) <= 0
			|| dc->tap_and_drag < 0) {
	} else {
		libinput_device_config_tap_set_drag_enabled(
			libinput_dev, dc->tap_and_drag);
	}
	libinput_device_config_tap_set_drag_lock_enabled(libinput_dev,
		libinput_device_config_tap_get_default_drag_lock_enabled(libinput_dev));
	if (libinput_device_config_tap_get_finger_count(libinput_dev) <= 0
			|| dc->drag_lock < 0) {
	} else {
		libinput_device_config_tap_set_drag_lock_enabled(
			libinput_dev, dc->drag_lock);
	}
#if HAVE_LIBINPUT_CONFIG_3FG_DRAG_ENABLED_3FG
	libinput_device_config_3fg_drag_set_enabled(libinput_dev,
		libinput_device_config_3fg_drag_get_default_enabled(libinput_dev));
	if (libinput_device_config_tap_get_finger_count(libinput_dev) <= 0
			|| dc->three_finger_drag < 0) {
	} else {
		libinput_device_config_3fg_drag_set_enabled(
			libinput_dev, dc->three_finger_drag);
	}
#endif
	libinput_device_config_scroll_set_natural_scroll_enabled(libinput_dev,
		libinput_device_config_scroll_get_default_natural_scroll_enabled(libinput_dev));
	if (libinput_device_config_scroll_has_natural_scroll(libinput_dev) <= 0
			|| dc->natural_scroll < 0) {
	} else {
		libinput_device_config_scroll_set_natural_scroll_enabled(
			libinput_dev, dc->natural_scroll);
	}
	libinput_device_config_left_handed_set(libinput_dev,
		libinput_device_config_left_handed_get_default(libinput_dev));
	if (libinput_device_config_left_handed_is_available(libinput_dev) <= 0
			|| dc->left_handed < 0) {
	} else {
		libinput_device_config_left_handed_set(libinput_dev,
			dc->left_handed);
	}
	libinput_device_config_accel_set_speed(libinput_dev,
		libinput_device_config_accel_get_default_speed(libinput_dev));
	libinput_device_config_accel_set_profile(libinput_dev,
		libinput_device_config_accel_get_default_profile(libinput_dev));
	if (libinput_device_config_accel_is_available(libinput_dev) == 0) {
	} else {
		if (dc->pointer_speed >= -1) {
			libinput_device_config_accel_set_speed(libinput_dev,
				dc->pointer_speed);
		} else {
		}
		if (dc->accel_profile > 0) {
			libinput_device_config_accel_set_profile(libinput_dev,
				dc->accel_profile);
		} else {
		}
	}
	libinput_device_config_middle_emulation_set_enabled(libinput_dev,
		libinput_device_config_middle_emulation_get_default_enabled(libinput_dev));
	if (libinput_device_config_middle_emulation_is_available(libinput_dev)
			== 0 || dc->middle_emu < 0)  {
	} else {
		libinput_device_config_middle_emulation_set_enabled(
			libinput_dev, dc->middle_emu);
	}
	libinput_device_config_dwt_set_enabled(libinput_dev,
		libinput_device_config_dwt_get_default_enabled(libinput_dev));
	if (libinput_device_config_dwt_is_available(libinput_dev) == 0
			|| dc->dwt < 0) {
	} else {
		libinput_device_config_dwt_set_enabled(libinput_dev, dc->dwt);
	}
	libinput_device_config_click_set_method(libinput_dev,
		libinput_device_config_click_get_default_method(libinput_dev));
	if ((dc->click_method != LIBINPUT_CONFIG_CLICK_METHOD_NONE
			&& (libinput_device_config_click_get_methods(libinput_dev)
				& (uint32_t)dc->click_method) == 0)
			|| dc->click_method < 0) {
	} else {
		libinput_device_config_click_set_method(libinput_dev, dc->click_method);
	}
	libinput_device_config_scroll_set_method(libinput_dev,
		libinput_device_config_scroll_get_default_method(libinput_dev));
	if (dc->scroll_method < 0) {
	} else if (dc->scroll_method != LIBINPUT_CONFIG_SCROLL_NO_SCROLL
			&& (libinput_device_config_scroll_get_methods(libinput_dev)
				& (uint32_t)dc->scroll_method) == 0) {
	} else {
		libinput_device_config_scroll_set_method(libinput_dev, dc->scroll_method);
	}
	libinput_device_config_scroll_set_button(libinput_dev,
		libinput_device_config_scroll_get_default_button(libinput_dev));
	if (dc->scroll_button < 0) {
	} else {
		libinput_device_config_scroll_set_button(libinput_dev,
			(uint32_t)dc->scroll_button);
	}
	libinput_device_config_send_events_set_mode(libinput_dev,
		libinput_device_config_send_events_get_default_mode(libinput_dev));
	if ((dc->send_events_mode != LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
			&& (libinput_device_config_send_events_get_modes(libinput_dev)
				& (uint32_t)dc->send_events_mode) == 0)
			|| dc->send_events_mode < 0) {
	} else {
		libinput_device_config_send_events_set_mode(libinput_dev,
			(uint32_t)dc->send_events_mode);
	}
	float default_matrix[6];
	libinput_device_config_calibration_get_default_matrix(libinput_dev, default_matrix);
	libinput_device_config_calibration_set_matrix(libinput_dev, default_matrix);
	if (libinput_device_config_calibration_has_matrix(libinput_dev) == 0
			|| !dc->have_calibration_matrix) {
	} else {
		libinput_device_config_calibration_set_matrix(libinput_dev, dc->calibration_matrix);
	}
	input->scroll_factor = dc->scroll_factor;
#endif
}
static struct wlr_output *
LABWC_src_seat_c_output_by_name(const char *name)
{
	assert(name);
	struct output *output;
	wl_list_for_each(output, &server.outputs, link) {
		if (!strcasecmp(output->wlr_output->name, name)) {
			return output->wlr_output;
		}
	}
	return NULL;
}
static void
LABWC_src_seat_c_map_input_to_output(struct seat *seat, struct wlr_input_device *dev, char *output_name)
{
	struct wlr_output *output = NULL;
	if (output_name) {
		output = LABWC_src_seat_c_output_by_name(output_name);
	}
	wlr_cursor_map_input_to_output(seat->cursor, dev, output);
	wlr_cursor_map_input_to_region(seat->cursor, dev, NULL);
}
static void
LABWC_src_seat_c_map_pointer_to_output(struct seat *seat, struct wlr_input_device *dev)
{
	struct wlr_pointer *pointer = wlr_pointer_from_input_device(dev);
	LABWC_src_seat_c_map_input_to_output(seat, dev, pointer->output_name);
}
static struct input *
LABWC_src_seat_c_new_pointer(struct seat *seat, struct wlr_input_device *dev)
{
	struct input *input = znew(*input);
	input->wlr_input_device = dev;
	dev->data = input;
	LABWC_src_seat_c_configure_libinput(dev);
	wlr_cursor_attach_input_device(seat->cursor, dev);
	if (dev->type == WLR_INPUT_DEVICE_POINTER) {
		LABWC_src_seat_c_map_pointer_to_output(seat, dev);
	}
	return input;
}
static struct input *
LABWC_src_seat_c_new_keyboard(struct seat *seat, struct wlr_input_device *device, bool is_virtual)
{
	struct wlr_keyboard *kb = wlr_keyboard_from_input_device(device);
	struct keyboard *keyboard = znew(*keyboard);
	keyboard->base.wlr_input_device = device;
	keyboard->wlr_keyboard = kb;
	keyboard->is_virtual = is_virtual;
	device->data = keyboard;
	LABWC_src_seat_c_configure_libinput(device);
	if (!seat->keyboard_group->keyboard.keymap) {
		exit(EXIT_FAILURE);
	}
	wlr_keyboard_set_keymap(kb, seat->keyboard_group->keyboard.keymap);
	keyboard_set_numlock(kb);
	if (is_virtual) {
		wlr_keyboard_set_repeat_info(kb, rc.repeat_rate, rc.repeat_delay);
	} else {
		wlr_keyboard_group_add_keyboard(seat->keyboard_group, kb);
	}
	keyboard_setup_handlers(keyboard);
	wlr_seat_set_keyboard(seat->wlr_seat, kb);
	return (struct input *)keyboard;
}
static void
LABWC_src_seat_c_seat_update_capabilities(struct seat *seat)
{
	struct input *input = NULL;
	uint32_t caps = 0;
	wl_list_for_each(input, &seat->inputs, link) {
		switch (input->wlr_input_device->type) {
		case WLR_INPUT_DEVICE_KEYBOARD:
			caps |= WL_SEAT_CAPABILITY_KEYBOARD;
			break;
		case WLR_INPUT_DEVICE_POINTER:
			caps |= WL_SEAT_CAPABILITY_POINTER;
			break;
		default:
			break;
		}
	}
	wlr_seat_set_capabilities(seat->wlr_seat, caps);
}
static void
LABWC_src_seat_c_seat_add_device(struct seat *seat, struct input *input)
{
	input->seat = seat;
	input->destroy.notify = LABWC_src_seat_c_input_device_destroy;
	wl_signal_add(&input->wlr_input_device->events.destroy, &input->destroy);
	wl_list_insert(&seat->inputs, &input->link);
	LABWC_src_seat_c_seat_update_capabilities(seat);
}
static void
LABWC_src_seat_c_handle_new_input(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, new_input);
	struct wlr_input_device *device = data;
	struct input *input = NULL;
	switch (device->type) {
	case WLR_INPUT_DEVICE_KEYBOARD:
		input = LABWC_src_seat_c_new_keyboard(seat, device, false);
		break;
	case WLR_INPUT_DEVICE_POINTER:
		input = LABWC_src_seat_c_new_pointer(seat, device);
		break;
	default:
		return;
	}
	LABWC_src_seat_c_seat_add_device(seat, input);
}
static void
LABWC_src_seat_c_handle_new_virtual_pointer(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, new_virtual_pointer);
	struct wlr_virtual_pointer_v1_new_pointer_event *event = data;
	struct wlr_virtual_pointer_v1 *pointer = event->new_pointer;
	struct wlr_input_device *device = &pointer->pointer.base;
	struct input *input = LABWC_src_seat_c_new_pointer(seat, device);
	device->data = input;
	LABWC_src_seat_c_seat_add_device(seat, input);
	if (event->suggested_output) {
		wlr_cursor_map_input_to_output(seat->cursor, device,
			event->suggested_output);
	}
}
static void
LABWC_src_seat_c_handle_new_virtual_keyboard(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat, new_virtual_keyboard);
	struct wlr_virtual_keyboard_v1 *virtual_keyboard = data;
	struct wlr_input_device *device = &virtual_keyboard->keyboard.base;
	struct input *input = LABWC_src_seat_c_new_keyboard(seat, device, true);
	device->data = input;
	LABWC_src_seat_c_seat_add_device(seat, input);
}
static void
LABWC_src_seat_c_handle_focus_change(struct wl_listener *listener, void *data)
{
	struct wlr_seat_keyboard_focus_change_event *event = data;
	struct wlr_surface *surface = event->new_surface;
	struct view *view = surface ? view_from_wlr_surface(surface) : NULL;
	if (surface && !view) {
		return;
	}
	if (server.input_mode != LAB_INPUT_STATE_PASSTHROUGH) {
		return;
	}
	if (view != server.active_view) {
		if (server.active_view) {
			view_set_activated(server.active_view, false);
		}
		if (view) {
			view_set_activated(view, true);
		}
		server.active_view = view;
	}
}
void
seat_init(void)
{
	struct seat *seat = &server.seat;
	seat->wlr_seat = wlr_seat_create(server.wl_display, "seat0");
	if (!seat->wlr_seat) {
		exit(EXIT_FAILURE);
	}
	wl_list_init(&seat->constraint_commit.link);
	wl_list_init(&seat->inputs);
	CONNECT_SIGNAL(server.backend, seat, new_input);
	CONNECT_SIGNAL(&seat->wlr_seat->keyboard_state, seat, focus_change);
	seat->virtual_pointer = wlr_virtual_pointer_manager_v1_create(
		server.wl_display);
	CONNECT_SIGNAL(seat->virtual_pointer, seat, new_virtual_pointer);
	seat->virtual_keyboard = wlr_virtual_keyboard_manager_v1_create(
		server.wl_display);
	CONNECT_SIGNAL(seat->virtual_keyboard, seat, new_virtual_keyboard);
	seat->input_method_relay = input_method_relay_create(seat);
	seat->xcursor_manager = NULL;
	seat->cursor_visible = true;
	seat->cursor = wlr_cursor_create();
	if (!seat->cursor) {
		exit(EXIT_FAILURE);
	}
	wlr_cursor_attach_output_layout(seat->cursor, server.output_layout);
	input_handlers_init(seat);
}
void
seat_finish(void)
{
	struct seat *seat = &server.seat;
	wl_list_remove(&seat->new_input.link);
	wl_list_remove(&seat->focus_change.link);
	wl_list_remove(&seat->new_virtual_pointer.link);
	wl_list_remove(&seat->new_virtual_keyboard.link);
	struct input *input, *next;
	wl_list_for_each_safe(input, next, &seat->inputs, link) {
		LABWC_src_seat_c_input_device_destroy(&input->destroy, NULL);
	}
	if (seat->workspace_osd_timer) {
		wl_event_source_remove(seat->workspace_osd_timer);
		seat->workspace_osd_timer = NULL;
	}
	overlay_finish(seat);
	input_handlers_finish(seat);
	input_method_relay_finish(seat->input_method_relay);
}
static void
LABWC_src_seat_c_configure_keyboard(struct seat *seat, struct input *input)
{
	struct wlr_input_device *device = input->wlr_input_device;
	assert(device->type == WLR_INPUT_DEVICE_KEYBOARD);
	struct keyboard *keyboard = (struct keyboard *)input;
	struct wlr_keyboard *kb = wlr_keyboard_from_input_device(device);
	keyboard_configure(seat, kb, keyboard->is_virtual);
}
void
seat_pointer_end_grab(struct seat *seat, struct wlr_surface *surface)
{
	if (!surface || !wlr_seat_pointer_has_grab(seat->wlr_seat)) {
		return;
	}
	struct wlr_xdg_surface *xdg_surface =
		wlr_xdg_surface_try_from_wlr_surface(surface);
	if (!xdg_surface || xdg_surface->role != WLR_XDG_SURFACE_ROLE_POPUP) {
		wlr_seat_pointer_end_grab(seat->wlr_seat);
	}
}
void
seat_reconfigure(void)
{
	struct seat *seat = &server.seat;
	struct input *input;
	cursor_reload(seat);
	overlay_finish(seat);
	keyboard_reset_current_keybind();
	wl_list_for_each(input, &seat->inputs, link) {
		switch (input->wlr_input_device->type) {
		case WLR_INPUT_DEVICE_KEYBOARD:
			LABWC_src_seat_c_configure_libinput(input->wlr_input_device);
			LABWC_src_seat_c_configure_keyboard(seat, input);
			break;
		case WLR_INPUT_DEVICE_POINTER:
			LABWC_src_seat_c_configure_libinput(input->wlr_input_device);
			LABWC_src_seat_c_map_pointer_to_output(seat, input->wlr_input_device);
			break;
		default:
			break;
		}
	}
}
void
seat_force_focus_surface(struct seat *seat, struct wlr_surface *surface)
{
	if (server.session_lock_manager->locked) {
		return;
	}
	uint32_t *pressed_sent_keycodes = key_state_pressed_sent_keycodes();
	int nr_pressed_sent_keycodes = key_state_nr_pressed_sent_keycodes();
	struct wlr_keyboard *kb = &seat->keyboard_group->keyboard;
	wlr_seat_keyboard_enter(seat->wlr_seat, surface,
		pressed_sent_keycodes, (size_t)nr_pressed_sent_keycodes,
		&kb->modifiers);
}
static void
LABWC_src_seat_c_seat_focus(struct seat *seat, struct wlr_surface *surface,
		bool replace_exclusive_layer, bool is_lock_surface)
{
	if (seat->focused_layer && seat->focused_layer->current.keyboard_interactive
			== ZWLR_LAYER_SURFACE_V1_KEYBOARD_INTERACTIVITY_EXCLUSIVE
				&& !replace_exclusive_layer) {
		return;
	}
	if (server.session_lock_manager->locked && !is_lock_surface) {
		return;
	}
	if (!surface) {
		wlr_seat_keyboard_notify_clear_focus(seat->wlr_seat);
		input_method_relay_set_focus(seat->input_method_relay, NULL);
		return;
	}
	if (!wlr_seat_get_keyboard(seat->wlr_seat)) {
		wlr_seat_set_keyboard(seat->wlr_seat, &seat->keyboard_group->keyboard);
	}
	uint32_t *pressed_sent_keycodes = key_state_pressed_sent_keycodes();
	int nr_pressed_sent_keycodes = key_state_nr_pressed_sent_keycodes();
	struct wlr_keyboard *kb = &seat->keyboard_group->keyboard;
	wlr_seat_keyboard_notify_enter(seat->wlr_seat, surface,
		pressed_sent_keycodes, (size_t)nr_pressed_sent_keycodes,
		&kb->modifiers);
	input_method_relay_set_focus(seat->input_method_relay, surface);
	struct wlr_pointer_constraint_v1 *constraint =
		wlr_pointer_constraints_v1_constraint_for_surface(server.constraints,
			surface, seat->wlr_seat);
	constrain_cursor(constraint);
}
void
seat_focus_surface(struct seat *seat, struct wlr_surface *surface)
{
	if (server.input_mode != LAB_INPUT_STATE_PASSTHROUGH) {
		return;
	}
	LABWC_src_seat_c_seat_focus(seat, surface,   false,
		  false);
}
void
seat_focus_lock_surface(struct seat *seat, struct wlr_surface *surface)
{
	LABWC_src_seat_c_seat_focus(seat, surface,   true,
		  true);
}
void
seat_set_focus_layer(struct seat *seat, struct wlr_layer_surface_v1 *layer)
{
	if (!layer) {
		seat->focused_layer = NULL;
		desktop_focus_topmost_view();
		return;
	}
	LABWC_src_seat_c_seat_focus(seat, layer->surface,   true,
		  false);
	seat->focused_layer = layer;
}
void
seat_output_layout_changed(struct seat *seat)
{
	struct input *input = NULL;
	wl_list_for_each(input, &seat->inputs, link) {
		switch (input->wlr_input_device->type) {
		case WLR_INPUT_DEVICE_POINTER:
			LABWC_src_seat_c_map_pointer_to_output(seat, input->wlr_input_device);
			break;
		default:
			break;
		}
	}
}
static void
LABWC_src_seat_c_handle_focus_override_surface_destroy(struct wl_listener *listener, void *data)
{
	struct seat *seat = wl_container_of(listener, seat,
		focus_override.surface_destroy);
	wl_list_remove(&seat->focus_override.surface_destroy.link);
	seat->focus_override.surface = NULL;
}
void
seat_focus_override_begin(struct seat *seat, enum input_mode input_mode,
	enum lab_cursors cursor_shape)
{
	assert(!seat->focus_override.surface);
	assert(server.input_mode == LAB_INPUT_STATE_PASSTHROUGH);
	server.input_mode = input_mode;
	seat->focus_override.surface = seat->wlr_seat->keyboard_state.focused_surface;
	if (seat->focus_override.surface) {
		seat->focus_override.surface_destroy.notify =
			LABWC_src_seat_c_handle_focus_override_surface_destroy;
		wl_signal_add(&seat->focus_override.surface->events.destroy,
			&seat->focus_override.surface_destroy);
	}
	LABWC_src_seat_c_seat_focus(seat, NULL,   false,
		  false);
	wlr_seat_pointer_clear_focus(seat->wlr_seat);
	cursor_set(seat, cursor_shape);
}
void
seat_focus_override_end(struct seat *seat, bool restore_focus)
{
	server.input_mode = LAB_INPUT_STATE_PASSTHROUGH;
	if (seat->focus_override.surface) {
		if (restore_focus) {
			LABWC_src_seat_c_seat_focus(seat, seat->focus_override.surface,
				  false,
				  false);
		}
		wl_list_remove(&seat->focus_override.surface_destroy.link);
		seat->focus_override.surface = NULL;
	}
	if (restore_focus) {
		cursor_update_focus();
	}
}
#undef handle_focus_change
#undef handle_new_input
#undef handle_new_virtual_keyboard
#undef handle_new_virtual_pointer
#undef wlr_input_device_is_libinput
#include <assert.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#if HAVE_XWAYLAND
#endif
#if WLR_HAS_DRM_BACKEND
#endif
#define LAB_EXT_DATA_CONTROL_VERSION 1
#define LAB_EXT_FOREIGN_TOPLEVEL_LIST_VERSION 1
#define LAB_WLR_COMPOSITOR_VERSION 6
#define LAB_WLR_LINUX_DMABUF_VERSION 4
#define LAB_WLR_PRESENTATION_TIME_VERSION 2
static void
LABWC_src_server_c_reload_config_and_theme(void)
{
	desktop_cancel_pending_auto_raise();
	scaled_buffer_invalidate_sharing();
	theme_finish(rc.theme);
	theme_init(rc.theme);
	struct view *view;
	wl_list_for_each(view, &server.views, link) {
		view_reload_ssd(view);
	}
	menu_reconfigure();
	seat_reconfigure();
	regions_reconfigure();
	resize_indicator_reconfigure();
	kde_server_decoration_update_default();
}
static int
LABWC_src_server_c_handle_sigterm(int signal, void *data)
{
	struct wl_display *display = data;
	wl_display_terminate(display);
	return 0;
}
static int
LABWC_src_server_c_handle_sigchld(int signal, void *data)
{
	siginfo_t info;
	info.si_pid = 0;
	while (waitid(P_ALL,   0, &info, WEXITED | WNOHANG | WNOWAIT) != -1) {
		if (info.si_pid == 0) {
			return 0;
		}
	#if HAVE_XWAYLAND
		if (server.xwayland && server.xwayland->server
				&& info.si_pid == server.xwayland->server->pid) {
			return 0;
		}
	#endif
		int ret = waitid(P_PID, (id_t)info.si_pid, &info, WEXITED);
		if (ret == -1) {
			goto check_next;
		}
		if (info.si_pid == server.primary_client_pid) {
			wl_display_terminate(server.wl_display);
		}
check_next:
		info.si_pid = 0;
	}
	return 0;
}
#if WLR_HAS_DRM_BACKEND
static void
LABWC_src_server_c_handle_drm_lease_request(struct wl_listener *listener, void *data)
{
	struct wlr_drm_lease_request_v1 *req = data;
	struct wlr_drm_lease_v1 *lease = wlr_drm_lease_request_v1_grant(req);
	if (!lease) {
		wlr_drm_lease_request_v1_reject(req);
		return;
	}
}
#endif
static bool
LABWC_src_server_c_allow_for_sandbox(const struct wlr_security_context_v1_state *security_state,
		const struct wl_interface *iface)
{
	if (!strcmp(iface->name, "security_context_manager_v1")) {
		return false;
	}
	static const char * const allowed_protocols[] = {
		"wl_shm",
		"wl_compositor",
		"wl_subcompositor",
		"wl_data_device_manager",
		"wl_seat",
		"xdg_wm_base",
		"wl_fixes",
		"wl_output",
		"wl_drm",
		"zwp_linux_dmabuf_v1",
		"zwp_primary_selection_device_manager_v1",
		"zwp_text_input_manager_v3",
		"zwp_pointer_gestures_v1",
		"wp_cursor_shape_manager_v1",
		"zwp_relative_pointer_manager_v1",
		"xdg_activation_v1",
		"org_kde_kwin_server_decoration_manager",
		"zxdg_decoration_manager_v1",
		"wp_presentation",
		"wp_viewporter",
		"wp_single_pixel_buffer_manager_v1",
#ifdef FULLSCREEN_TEARING
		"wp_tearing_control_manager_v1",
#endif
		"zxdg_importer_v1",
		"zxdg_importer_v2",
		"xdg_wm_dialog_v1",
		"wp_alpha_modifier_v1",
		"wp_color_representation_manager_v1",
		"wp_linux_drm_syncobj_manager_v1",
		"zxdg_exporter_v1",
		"zxdg_exporter_v2",
		"zwp_idle_inhibit_manager_v1",
		"zwp_pointer_constraints_v1",
		"zxdg_output_manager_v1",
	};
	for (size_t i = 0; i < ARRAY_SIZE(allowed_protocols); i++) {
		if (!strcmp(iface->name, allowed_protocols[i])) {
			return true;
		}
	}
	return false;
}
static bool
LABWC_src_server_c_server_global_filter(const struct wl_client *client, const struct wl_global *global, void *data)
{
	const struct wl_interface *iface = wl_global_get_interface(global);
	(void)iface;
#if HAVE_XWAYLAND
	struct wl_client *xwayland_client = (server.xwayland && server.xwayland->server)
		? server.xwayland->server->client
		: NULL;
	if (client != xwayland_client && !strcmp(iface->name, "xwayland_shell_v1")) {
		return false;
	}
#endif
	uint32_t iface_id = parse_privileged_interface(iface->name);
	if (iface_id && !(iface_id & rc.allowed_interfaces)) {
		return false;
	}
	const struct wlr_security_context_v1_state *security_context =
		wlr_security_context_manager_v1_lookup_client(
			server.security_context_manager_v1, (struct wl_client *)client);
	if (security_context && global == server.security_context_manager_v1->global) {
		return false;
	} else if (security_context) {
		bool allow = LABWC_src_server_c_allow_for_sandbox(security_context, iface);
		return allow;
	}
	return true;
}
static void
LABWC_src_server_c_get_headless_backend(struct wlr_backend *backend, void *data)
{
	if (wlr_backend_is_headless(backend)) {
		struct wlr_backend **headless = data;
		*headless = backend;
	}
}
static void
LABWC_src_server_c_handle_renderer_lost(struct wl_listener *listener, void *data)
{
	struct wlr_renderer *renderer = wlr_renderer_autocreate(server.backend);
	if (!renderer) {
		return;
	}
	struct wlr_allocator *allocator =
		wlr_allocator_autocreate(server.backend, renderer);
	if (!allocator) {
		wlr_renderer_destroy(renderer);
		return;
	}
	struct wlr_renderer *old_renderer = server.renderer;
	struct wlr_allocator *old_allocator = server.allocator;
	server.renderer = renderer;
	server.allocator = allocator;
	wl_list_remove(&server.renderer_lost.link);
	wl_signal_add(&server.renderer->events.lost, &server.renderer_lost);
	wlr_compositor_set_renderer(server.compositor, renderer);
	struct output *output;
	wl_list_for_each(output, &server.outputs, link) {
		wlr_output_init_render(output->wlr_output,
			server.allocator, server.renderer);
	}
	LABWC_src_server_c_reload_config_and_theme();
	wlr_allocator_destroy(old_allocator);
	wlr_renderer_destroy(old_renderer);
}
static void
LABWC_src_server_c_handle_toplevel_capture_source_destroy(struct wl_listener *listener, void *data)
{
	struct view *view = wl_container_of(listener, view, capture.on_capture_source_destroy);
	assert(view->capture.source);
	view->capture.source = NULL;
	wl_list_remove(&listener->link);
	wl_list_init(&listener->link);
}
static void
LABWC_src_server_c_handle_toplevel_capture_request(struct wl_listener *listener, void *data)
{
	struct wlr_ext_foreign_toplevel_image_capture_source_manager_v1_request_event *request = data;
	struct view *view = request->toplevel_handle->data;
	assert(view);
	if (!view->capture.source) {
		view->capture.source = wlr_ext_image_capture_source_v1_create_with_scene_node(
			&view->capture.scene->tree.node, server.wl_event_loop,
			server.allocator, server.renderer);
		assert(view->capture.source);
		view->capture.on_capture_source_destroy.notify =
			LABWC_src_server_c_handle_toplevel_capture_source_destroy;
		wl_signal_add(&view->capture.source->events.destroy,
			&view->capture.on_capture_source_destroy);
	}
	wlr_ext_foreign_toplevel_image_capture_source_manager_v1_request_accept(
		request, view->capture.source);
}
void
server_init(void)
{
	server.primary_client_pid = -1;
	server.wl_display = wl_display_create();
	if (!server.wl_display) {
		exit(EXIT_FAILURE);
	}
	wl_display_set_default_max_buffer_size(server.wl_display, 1024 * 1024);
	wl_display_set_global_filter(server.wl_display, LABWC_src_server_c_server_global_filter, NULL);
	server.wl_event_loop = wl_display_get_event_loop(server.wl_display);
	wlr_fixes_create(server.wl_display, 1);
	signal(SIGHUP, SIG_IGN);
	server.sigint_source = wl_event_loop_add_signal(
		server.wl_event_loop, SIGINT, LABWC_src_server_c_handle_sigterm, server.wl_display);
	server.sigterm_source = wl_event_loop_add_signal(
		server.wl_event_loop, SIGTERM, LABWC_src_server_c_handle_sigterm, server.wl_display);
	server.sigchld_source = wl_event_loop_add_signal(
		server.wl_event_loop, SIGCHLD, LABWC_src_server_c_handle_sigchld, NULL);
	signal(SIGPIPE, SIG_IGN);
	server.backend = wlr_backend_autocreate(
		server.wl_event_loop, &server.session);
	if (!server.backend) {
		exit(EXIT_FAILURE);
	}
	wlr_multi_for_each_backend(server.backend,
		LABWC_src_server_c_get_headless_backend, &server.headless.backend);
	if (!server.headless.backend) {
		server.headless.backend = wlr_headless_backend_create(
			server.wl_event_loop);
	} else {
	}
	if (!server.headless.backend) {
		exit(EXIT_FAILURE);
	}
	wlr_multi_backend_add(server.backend, server.headless.backend);
	wlr_output_destroy(wlr_headless_add_output(server.headless.backend, 0, 0));
	server.renderer = wlr_renderer_autocreate(server.backend);
	if (!server.renderer) {
		exit(EXIT_FAILURE);
	}
	server.renderer_lost.notify = LABWC_src_server_c_handle_renderer_lost;
	wl_signal_add(&server.renderer->events.lost, &server.renderer_lost);
	if (!wlr_renderer_init_wl_shm(server.renderer, server.wl_display)) {
		exit(EXIT_FAILURE);
	}
	if (wlr_renderer_get_texture_formats(
			server.renderer, WLR_BUFFER_CAP_DMABUF)) {
		if (wlr_renderer_get_drm_fd(server.renderer) >= 0) {
			wlr_drm_create(server.wl_display, server.renderer);
		}
		server.linux_dmabuf = wlr_linux_dmabuf_v1_create_with_renderer(
			server.wl_display,
			LAB_WLR_LINUX_DMABUF_VERSION,
			server.renderer);
	} else {
	}
	if (wlr_renderer_get_drm_fd(server.renderer) >= 0 &&
			server.renderer->features.timeline &&
			server.backend->features.timeline) {
		wlr_linux_drm_syncobj_manager_v1_create(server.wl_display, 1,
			wlr_renderer_get_drm_fd(server.renderer));
	}
	server.allocator = wlr_allocator_autocreate(
		server.backend, server.renderer);
	if (!server.allocator) {
		exit(EXIT_FAILURE);
	}
	wl_list_init(&server.views);
	wl_list_init(&server.unmanaged_surfaces);
	server.scene = wlr_scene_create();
	die_if_null(server.scene);
	server.direct_scanout_enabled = server.scene->direct_scanout;
	wlr_color_representation_manager_v1_create_with_renderer(
		server.wl_display, 1, server.renderer);
	server.workspace_tree = lab_wlr_scene_tree_create(&server.scene->tree);
	server.xdg_popup_tree = lab_wlr_scene_tree_create(&server.scene->tree);
#if HAVE_XWAYLAND
	server.unmanaged_tree = lab_wlr_scene_tree_create(&server.scene->tree);
#endif
	server.menu_tree = lab_wlr_scene_tree_create(&server.scene->tree);
	workspaces_init();
	output_init();
	server.compositor = wlr_compositor_create(server.wl_display,
		LAB_WLR_COMPOSITOR_VERSION, server.renderer);
	if (!server.compositor) {
		exit(EXIT_FAILURE);
	}
	wlr_subcompositor_create(server.wl_display);
	struct wlr_data_device_manager *device_manager = NULL;
	device_manager = wlr_data_device_manager_create(server.wl_display);
	if (!device_manager) {
		exit(EXIT_FAILURE);
	}
	if (rc.primary_selection) {
		wlr_primary_selection_v1_device_manager_create(server.wl_display);
	}
	server.input_method_manager = wlr_input_method_manager_v2_create(
		server.wl_display);
	server.text_input_manager = wlr_text_input_manager_v3_create(
		server.wl_display);
	seat_init();
	xdg_shell_init();
	kde_server_decoration_init();
	xdg_server_decoration_init();
	struct wlr_presentation *presentation = wlr_presentation_create(
		server.wl_display, server.backend,
		LAB_WLR_PRESENTATION_TIME_VERSION);
	if (!presentation) {
		exit(EXIT_FAILURE);
	}
	if (server.linux_dmabuf) {
		wlr_scene_set_linux_dmabuf_v1(server.scene, server.linux_dmabuf);
	}
	wlr_export_dmabuf_manager_v1_create(server.wl_display);
	wlr_screencopy_manager_v1_create(server.wl_display);
	wlr_ext_image_copy_capture_manager_v1_create(server.wl_display, 1);
	wlr_ext_output_image_capture_source_manager_v1_create(server.wl_display, 1);
	server.toplevel_capture.manager =
		wlr_ext_foreign_toplevel_image_capture_source_manager_v1_create(
			server.wl_display, 1);
	if (server.toplevel_capture.manager) {
		server.toplevel_capture.on.new_request.notify = LABWC_src_server_c_handle_toplevel_capture_request;
		wl_signal_add(&server.toplevel_capture.manager->events.capture_request,
			&server.toplevel_capture.on.new_request);
	} else {
		wl_list_init(&server.toplevel_capture.on.new_request.link);
	}
	wlr_data_control_manager_v1_create(server.wl_display);
	wlr_ext_data_control_manager_v1_create(server.wl_display,
		LAB_EXT_DATA_CONTROL_VERSION);
	server.security_context_manager_v1 =
		wlr_security_context_manager_v1_create(server.wl_display);
	wlr_viewporter_create(server.wl_display);
	wlr_single_pixel_buffer_manager_v1_create(server.wl_display);
	idle_manager_create(server.wl_display);
	server.relative_pointer_manager = wlr_relative_pointer_manager_v1_create(
		server.wl_display);
	server.constraints = wlr_pointer_constraints_v1_create(
		server.wl_display);
	server.new_constraint.notify = create_constraint;
	wl_signal_add(&server.constraints->events.new_constraint,
		&server.new_constraint);
	server.foreign_toplevel_manager =
		wlr_foreign_toplevel_manager_v1_create(server.wl_display);
	server.foreign_toplevel_list =
		wlr_ext_foreign_toplevel_list_v1_create(
			server.wl_display, LAB_EXT_FOREIGN_TOPLEVEL_LIST_VERSION);
	wlr_alpha_modifier_v1_create(server.wl_display);
	session_lock_init();
#if WLR_HAS_DRM_BACKEND
	server.drm_lease_manager = wlr_drm_lease_v1_manager_create(
		server.wl_display, server.backend);
	if (server.drm_lease_manager) {
		server.drm_lease_request.notify = LABWC_src_server_c_handle_drm_lease_request;
		wl_signal_add(&server.drm_lease_manager->events.request,
				&server.drm_lease_request);
	} else {
	}
#endif
	server.output_power_manager_v1 =
		wlr_output_power_manager_v1_create(server.wl_display);
	server.output_power_manager_set_mode.notify =
		handle_output_power_manager_set_mode;
	wl_signal_add(&server.output_power_manager_v1->events.set_mode,
		&server.output_power_manager_set_mode);
#ifdef FULLSCREEN_TEARING
	server.tearing_control = wlr_tearing_control_manager_v1_create(server.wl_display, 1);
	server.tearing_new_object.notify = handle_tearing_new_object;
	wl_signal_add(&server.tearing_control->events.new_object, &server.tearing_new_object);
#endif
	layers_init();
	struct wlr_xdg_foreign_registry *registry =
		wlr_xdg_foreign_registry_create(server.wl_display);
	wlr_xdg_foreign_v1_create(server.wl_display, registry);
	wlr_xdg_foreign_v2_create(server.wl_display, registry);
#if HAVE_XWAYLAND
	xwayland_server_init(server.compositor);
#endif
}
void
server_start(void)
{
	const char *socket = wl_display_add_socket_auto(server.wl_display);
	if (!socket) {
		exit(EXIT_FAILURE);
	}
	if (!wlr_backend_start(server.backend)) {
		exit(EXIT_FAILURE);
	}
	output_virtual_update_fallback();
	if (setenv("WAYLAND_DISPLAY", socket, true) < 0) {
	} else {
	}
}
void
server_finish(void)
{
#if HAVE_XWAYLAND
	xwayland_server_finish();
#endif
	wl_event_source_remove(server.sigint_source);
	wl_event_source_remove(server.sigterm_source);
	wl_event_source_remove(server.sigchld_source);
	wl_display_destroy_clients(server.wl_display);
	seat_finish();
	output_finish();
	xdg_shell_finish();
	layers_finish();
	kde_server_decoration_finish();
	xdg_server_decoration_finish();
	wl_list_remove(&server.new_constraint.link);
	wl_list_remove(&server.output_power_manager_set_mode.link);
#ifdef FULLSCREEN_TEARING
	wl_list_remove(&server.tearing_new_object.link);
#endif
	if (server.drm_lease_request.notify) {
		wl_list_remove(&server.drm_lease_request.link);
		server.drm_lease_request.notify = NULL;
	}
	wl_list_remove(&server.toplevel_capture.on.new_request.link);
	wlr_backend_destroy(server.backend);
	wlr_allocator_destroy(server.allocator);
	wl_list_remove(&server.renderer_lost.link);
	wlr_renderer_destroy(server.renderer);
	workspaces_destroy();
	wlr_scene_node_destroy(&server.scene->tree.node);
	wl_display_destroy(server.wl_display);
}
#undef LAB_EXT_DATA_CONTROL_VERSION
#undef LAB_EXT_FOREIGN_TOPLEVEL_LIST_VERSION
#undef LAB_WLR_COMPOSITOR_VERSION
#undef LAB_WLR_LINUX_DMABUF_VERSION
#undef LAB_WLR_PRESENTATION_TIME_VERSION
#include <assert.h>
struct LABWC_src_session_lock_c_session_lock_output {
	struct wlr_scene_tree *tree;
	struct wlr_scene_rect *background;
	struct session_lock_manager *manager;
	struct output *output;
	struct wlr_session_lock_surface_v1 *surface;
	struct wl_event_source *blank_timer;
	struct wl_list link;
	struct wl_listener destroy;
	struct wl_listener commit;
	struct wl_listener surface_destroy;
	struct wl_listener surface_map;
};
static void
LABWC_src_session_lock_c_focus_surface(struct session_lock_manager *manager, struct wlr_surface *focused)
{
	manager->focused = focused;
	seat_focus_lock_surface(&server.seat, focused);
}
static void
LABWC_src_session_lock_c_refocus_output(struct LABWC_src_session_lock_c_session_lock_output *output)
{
	if (output->manager->focused != output->surface->surface) {
		return;
	}
	struct LABWC_src_session_lock_c_session_lock_output *iter;
	wl_list_for_each(iter, &output->manager->lock_outputs, link) {
		if (iter == output || !iter->surface || !iter->surface->surface) {
			continue;
		}
		if (iter->surface->surface->mapped) {
			LABWC_src_session_lock_c_focus_surface(output->manager, iter->surface->surface);
			return;
		}
	}
	LABWC_src_session_lock_c_focus_surface(output->manager, NULL);
}
static void
LABWC_src_session_lock_c_update_focus(void *data)
{
	struct LABWC_src_session_lock_c_session_lock_output *output = data;
	cursor_update_focus();
	if (!output->manager->focused) {
		LABWC_src_session_lock_c_focus_surface(output->manager, output->surface->surface);
	}
}
static void
LABWC_src_session_lock_c_handle_surface_map(struct wl_listener *listener, void *data)
{
	struct LABWC_src_session_lock_c_session_lock_output *output =
		wl_container_of(listener, output, surface_map);
	wl_event_loop_add_idle(
		server.wl_event_loop, LABWC_src_session_lock_c_update_focus, output);
}
static void
LABWC_src_session_lock_c_handle_surface_destroy(struct wl_listener *listener, void *data)
{
	struct LABWC_src_session_lock_c_session_lock_output *output =
		wl_container_of(listener, output, surface_destroy);
	LABWC_src_session_lock_c_refocus_output(output);
	assert(output->surface);
	output->surface = NULL;
	wl_list_remove(&output->surface_destroy.link);
	wl_list_remove(&output->surface_map.link);
}
static void
LABWC_src_session_lock_c_lock_output_reconfigure(struct LABWC_src_session_lock_c_session_lock_output *output)
{
	struct wlr_box box;
	wlr_output_layout_get_box(server.output_layout,
		output->output->wlr_output, &box);
	wlr_scene_rect_set_size(output->background, box.width, box.height);
	if (output->surface) {
		wlr_session_lock_surface_v1_configure(
			output->surface, (uint32_t)box.width, (uint32_t)box.height);
	}
}
static struct LABWC_src_session_lock_c_session_lock_output *
LABWC_src_session_lock_c_lock_output_for_output(struct session_lock_manager *manager,
		struct output *output)
{
	struct LABWC_src_session_lock_c_session_lock_output *lock_output;
	wl_list_for_each(lock_output, &manager->lock_outputs, link) {
		if (lock_output->output == output) {
			return lock_output;
		}
	}
	return NULL;
}
static void
LABWC_src_session_lock_c_handle_new_surface(struct wl_listener *listener, void *data)
{
	struct session_lock_manager *manager =
		wl_container_of(listener, manager, lock_new_surface);
	struct wlr_session_lock_surface_v1 *lock_surface = data;
	struct output *output = lock_surface->output->data;
	struct LABWC_src_session_lock_c_session_lock_output *lock_output =
		LABWC_src_session_lock_c_lock_output_for_output(manager, output);
	if (!lock_output) {
		return;
	}
	lock_output->surface = lock_surface;
	struct wlr_scene_tree *surface_tree =
		wlr_scene_subsurface_tree_create(lock_output->tree, lock_surface->surface);
	die_if_null(surface_tree);
	node_descriptor_create(&surface_tree->node,
		LAB_NODE_SESSION_LOCK_SURFACE,   NULL,   NULL);
	lock_output->surface_destroy.notify = LABWC_src_session_lock_c_handle_surface_destroy;
	wl_signal_add(&lock_surface->events.destroy, &lock_output->surface_destroy);
	lock_output->surface_map.notify = LABWC_src_session_lock_c_handle_surface_map;
	wl_signal_add(&lock_surface->surface->events.map, &lock_output->surface_map);
	LABWC_src_session_lock_c_lock_output_reconfigure(lock_output);
}
static void
LABWC_src_session_lock_c_session_lock_output_destroy(struct LABWC_src_session_lock_c_session_lock_output *output)
{
	if (output->surface) {
		LABWC_src_session_lock_c_refocus_output(output);
		wl_list_remove(&output->surface_destroy.link);
		wl_list_remove(&output->surface_map.link);
	}
	wl_list_remove(&output->commit.link);
	wl_list_remove(&output->destroy.link);
	wl_list_remove(&output->link);
	wl_event_source_remove(output->blank_timer);
	free(output);
}
static void
LABWC_src_session_lock_c_handle_output_destroy(struct wl_listener *listener, void *data)
{
	struct LABWC_src_session_lock_c_session_lock_output *output = wl_container_of(listener, output, destroy);
	LABWC_src_session_lock_c_session_lock_output_destroy(output);
}
static void
LABWC_src_session_lock_c_handle_output_commit(struct wl_listener *listener, void *data)
{
	struct wlr_output_event_commit *event = data;
	struct LABWC_src_session_lock_c_session_lock_output *output = wl_container_of(listener, output, commit);
	uint32_t require_reconfigure = WLR_OUTPUT_STATE_MODE
		| WLR_OUTPUT_STATE_SCALE | WLR_OUTPUT_STATE_TRANSFORM;
	if (event->state->committed & require_reconfigure) {
		LABWC_src_session_lock_c_lock_output_reconfigure(output);
	}
}
static void
LABWC_src_session_lock_c_align_session_lock_tree(struct output *output)
{
	struct wlr_box box;
	wlr_output_layout_get_box(server.output_layout,
		output->wlr_output, &box);
	wlr_scene_node_set_position(&output->session_lock_tree->node, box.x, box.y);
}
static int
LABWC_src_session_lock_c_handle_output_blank_timeout(void *data)
{
	struct LABWC_src_session_lock_c_session_lock_output *lock_output = data;
	wlr_scene_node_set_enabled(&lock_output->background->node, true);
	return 0;
}
void
session_lock_output_create(struct session_lock_manager *manager, struct output *output)
{
	if (LABWC_src_session_lock_c_lock_output_for_output(manager, output)) {
		return;
	}
	struct LABWC_src_session_lock_c_session_lock_output *lock_output = znew(*lock_output);
	struct wlr_scene_tree *tree = lab_wlr_scene_tree_create(output->session_lock_tree);
	float black[4] = { 0.f, 0.f, 0.f, 1.f };
	struct wlr_scene_rect *background = lab_wlr_scene_rect_create(tree, 0, 0, black);
	lock_output->blank_timer =
		wl_event_loop_add_timer(server.wl_event_loop,
			LABWC_src_session_lock_c_handle_output_blank_timeout, lock_output);
	if (!manager->locked) {
		wlr_scene_node_set_enabled(&background->node, false);
		wl_event_source_timer_update(lock_output->blank_timer, 100);
	}
	LABWC_src_session_lock_c_align_session_lock_tree(output);
	lock_output->output = output;
	lock_output->tree = tree;
	lock_output->background = background;
	lock_output->manager = manager;
	lock_output->destroy.notify = LABWC_src_session_lock_c_handle_output_destroy;
	wl_signal_add(&tree->node.events.destroy, &lock_output->destroy);
	lock_output->commit.notify = LABWC_src_session_lock_c_handle_output_commit;
	wl_signal_add(&output->wlr_output->events.commit, &lock_output->commit);
	LABWC_src_session_lock_c_lock_output_reconfigure(lock_output);
	wl_list_insert(&manager->lock_outputs, &lock_output->link);
}
static void
LABWC_src_session_lock_c_session_lock_destroy(struct session_lock_manager *manager)
{
	struct LABWC_src_session_lock_c_session_lock_output *lock_output, *next;
	wl_list_for_each_safe(lock_output, next, &manager->lock_outputs, link) {
		wlr_scene_node_destroy(&lock_output->tree->node);
	}
	if (manager->lock) {
		wl_list_remove(&manager->lock_destroy.link);
		wl_list_remove(&manager->lock_unlock.link);
		wl_list_remove(&manager->lock_new_surface.link);
	}
	manager->lock = NULL;
}
static void
LABWC_src_session_lock_c_handle_lock_unlock(struct wl_listener *listener, void *data)
{
	struct session_lock_manager *manager =
		wl_container_of(listener, manager, lock_unlock);
	LABWC_src_session_lock_c_session_lock_destroy(manager);
	manager->locked = false;
	if (manager->last_active_view) {
		desktop_focus_view(manager->last_active_view,   false);
	} else {
		desktop_focus_topmost_view();
	}
	manager->last_active_view = NULL;
	cursor_update_focus();
}
static void
LABWC_src_session_lock_c_handle_lock_destroy(struct wl_listener *listener, void *data)
{
	struct session_lock_manager *manager =
		wl_container_of(listener, manager, lock_destroy);
	wl_list_remove(&manager->lock_destroy.link);
	wl_list_remove(&manager->lock_unlock.link);
	wl_list_remove(&manager->lock_new_surface.link);
	manager->lock = NULL;
}
static void
LABWC_src_session_lock_c_handle_new_session_lock(struct wl_listener *listener, void *data)
{
	struct session_lock_manager *manager =
		wl_container_of(listener, manager, new_lock);
	struct wlr_session_lock_v1 *lock = data;
	if (manager->lock) {
		wlr_session_lock_v1_destroy(lock);
		return;
	}
	if (manager->locked) {
		LABWC_src_session_lock_c_session_lock_destroy(manager);
	}
	assert(wl_list_empty(&manager->lock_outputs));
	manager->last_active_view = server.active_view;
	seat_focus_surface(&server.seat, NULL);
	struct output *output;
	wl_list_for_each(output, &server.outputs, link) {
		session_lock_output_create(manager, output);
	}
	manager->lock_new_surface.notify = LABWC_src_session_lock_c_handle_new_surface;
	wl_signal_add(&lock->events.new_surface, &manager->lock_new_surface);
	manager->lock_unlock.notify = LABWC_src_session_lock_c_handle_lock_unlock;
	wl_signal_add(&lock->events.unlock, &manager->lock_unlock);
	manager->lock_destroy.notify = LABWC_src_session_lock_c_handle_lock_destroy;
	wl_signal_add(&lock->events.destroy, &manager->lock_destroy);
	manager->locked = true;
	manager->lock = lock;
	wlr_session_lock_v1_send_locked(lock);
}
static void
LABWC_src_session_lock_c_handle_manager_destroy(struct wl_listener *listener, void *data)
{
	struct session_lock_manager *manager =
		wl_container_of(listener, manager, destroy);
	LABWC_src_session_lock_c_session_lock_destroy(manager);
	wl_list_remove(&manager->new_lock.link);
	wl_list_remove(&manager->destroy.link);
	server.session_lock_manager = NULL;
	free(manager);
}
void
session_lock_init(void)
{
	struct session_lock_manager *manager = znew(*manager);
	server.session_lock_manager = manager;
	manager->wlr_manager = wlr_session_lock_manager_v1_create(server.wl_display);
	wl_list_init(&manager->lock_outputs);
	manager->new_lock.notify = LABWC_src_session_lock_c_handle_new_session_lock;
	wl_signal_add(&manager->wlr_manager->events.new_lock, &manager->new_lock);
	manager->destroy.notify = LABWC_src_session_lock_c_handle_manager_destroy;
	wl_signal_add(&manager->wlr_manager->events.destroy, &manager->destroy);
}
void
session_lock_update_for_layout_change(void)
{
	if (!server.session_lock_manager->locked) {
		return;
	}
	struct output *output;
	wl_list_for_each(output, &server.outputs, link) {
		LABWC_src_session_lock_c_align_session_lock_tree(output);
	}
	struct session_lock_manager *manager = server.session_lock_manager;
	struct LABWC_src_session_lock_c_session_lock_output *lock_output;
	wl_list_for_each(lock_output, &manager->lock_outputs, link) {
		LABWC_src_session_lock_c_lock_output_reconfigure(lock_output);
	}
}
static bool LABWC_src_show_desktop_c_is_showing_desktop;
static void
LABWC_src_show_desktop_c_minimize_views(struct wl_array *views, bool minimize)
{
	struct view **view;
	wl_array_for_each_reverse(view, views) {
		view_minimize(*view, minimize);
	}
}
static void
LABWC_src_show_desktop_c_show(void)
{
	static struct wl_array views;
	wl_array_init(&views);
	struct view *view;
	for_each_view(view, &server.views, LAB_VIEW_CRITERIA_CURRENT_WORKSPACE) {
		if (view->minimized) {
			continue;
		}
		view->was_minimized_by_show_desktop_action = true;
		array_add(&views, view);
	}
	LABWC_src_show_desktop_c_minimize_views(&views, true);
	LABWC_src_show_desktop_c_is_showing_desktop = true;
	wl_array_release(&views);
}
static void
LABWC_src_show_desktop_c_restore(void)
{
	static struct wl_array views;
	wl_array_init(&views);
	struct view *view;
	for_each_view(view, &server.views, LAB_VIEW_CRITERIA_CURRENT_WORKSPACE) {
		if (view->was_minimized_by_show_desktop_action) {
			array_add(&views, view);
		}
	}
	LABWC_src_show_desktop_c_minimize_views(&views, false);
	show_desktop_reset();
	wl_array_release(&views);
}
void
show_desktop_toggle(void)
{
	if (LABWC_src_show_desktop_c_is_showing_desktop) {
		LABWC_src_show_desktop_c_restore();
	} else {
		LABWC_src_show_desktop_c_show();
	}
}
void
show_desktop_reset(void)
{
	LABWC_src_show_desktop_c_is_showing_desktop = false;
	struct view *view;
	for_each_view(view, &server.views, LAB_VIEW_CRITERIA_NONE) {
		view->was_minimized_by_show_desktop_action = false;
	}
}
#include <assert.h>
#include <string.h>
static struct {
	struct view *view;
	bool pending;
	int vertical_offset;
	int horizontal_offset;
	enum lab_edge resize_edges;
	struct wlr_box geom;
} last_snap_hit;
static void
LABWC_src_snap_constraints_c_snap_constraints_reset(void)
{
	last_snap_hit.view = NULL;
	last_snap_hit.pending = false;
	last_snap_hit.horizontal_offset = INT_MIN;
	last_snap_hit.vertical_offset = INT_MIN;
	last_snap_hit.resize_edges = LAB_EDGE_NONE;
	memset(&last_snap_hit.geom, 0, sizeof(last_snap_hit.geom));
}
static bool
LABWC_src_snap_constraints_c_snap_constraints_are_valid(struct view *view, enum lab_edge resize_edges)
{
	assert(view);
	if (view != last_snap_hit.view) {
		return false;
	}
	if (resize_edges != last_snap_hit.resize_edges) {
		return false;
	}
	if (resize_edges & LAB_EDGES_LEFT_RIGHT) {
		if (!BOUNDED_INT(last_snap_hit.horizontal_offset)) {
			return false;
		}
		if ((resize_edges & LAB_EDGE_LEFT) && (resize_edges & LAB_EDGE_RIGHT)) {
			return false;
		}
	} else if (resize_edges & LAB_EDGES_TOP_BOTTOM) {
		if (!BOUNDED_INT(last_snap_hit.vertical_offset)) {
			return false;
		}
		if ((resize_edges & LAB_EDGE_TOP) && (resize_edges & LAB_EDGE_BOTTOM)) {
			return false;
		}
	} else {
		return false;
	}
	return wlr_box_equal(&view->pending, &last_snap_hit.geom);
}
void
snap_constraints_set(struct view *view, enum lab_edge resize_edges,
		struct wlr_box geom)
{
	assert(view);
	last_snap_hit.horizontal_offset = INT_MIN;
	if (resize_edges & LAB_EDGE_LEFT) {
		last_snap_hit.horizontal_offset = geom.x;
	} else if (resize_edges & LAB_EDGE_RIGHT) {
		last_snap_hit.horizontal_offset = geom.x + geom.width;
	}
	last_snap_hit.vertical_offset = INT_MIN;
	if (resize_edges & LAB_EDGE_TOP) {
		last_snap_hit.vertical_offset = geom.y;
	} else if (resize_edges & LAB_EDGE_BOTTOM) {
		last_snap_hit.vertical_offset = geom.y + geom.height;
	}
	last_snap_hit.view = view;
	last_snap_hit.resize_edges = resize_edges;
	last_snap_hit.geom = geom;
	last_snap_hit.pending = true;
}
void
snap_constraints_invalidate(struct view *view)
{
	if (view == last_snap_hit.view) {
		LABWC_src_snap_constraints_c_snap_constraints_reset();
	}
}
void
snap_constraints_update(struct view *view)
{
	assert(view);
	if (view != last_snap_hit.view) {
		return;
	}
	if (!last_snap_hit.pending) {
		return;
	}
	if (view->pending.width != last_snap_hit.geom.width) {
		return;
	}
	if (view->pending.height != last_snap_hit.geom.height) {
		return;
	}
	last_snap_hit.geom = view->current;
	last_snap_hit.pending = false;
}
struct wlr_box
snap_constraints_effective(struct view *view, enum lab_edge resize_edges,
		bool use_pending)
{
	assert(view);
	struct wlr_box real_geom = use_pending ? view->pending : view->current;
	if (!LABWC_src_snap_constraints_c_snap_constraints_are_valid(view, resize_edges)) {
		return real_geom;
	}
	struct wlr_box geom = real_geom;
	if (resize_edges & LAB_EDGE_LEFT) {
		geom.x = last_snap_hit.horizontal_offset;
	} else if (resize_edges & LAB_EDGE_RIGHT) {
		geom.width = last_snap_hit.horizontal_offset - geom.x;
	}
	if (resize_edges & LAB_EDGE_TOP) {
		geom.y = last_snap_hit.vertical_offset;
	} else if (resize_edges & LAB_EDGE_BOTTOM) {
		geom.height = last_snap_hit.vertical_offset - geom.y;
	}
	if (!BOUNDED_INT(geom.x) || !BOUNDED_INT(geom.y)) {
		return real_geom;
	}
	if (geom.width <= 0 || geom.height <= 0) {
		return real_geom;
	}
	return geom;
}
#include <assert.h>
static void
LABWC_src_snap_c_check_edge(int *next, struct edge current, struct edge target,
		struct edge oppose, struct edge align, bool lesser)
{
	int cur = current.offset;
	int tgt = target.offset;
	int opp = oppose.offset;
	int aln = align.offset;
	if (cur == tgt) {
		return;
	}
	const bool decreasing = tgt < cur;
	if ((tgt <= opp && opp < cur) || (cur < opp && opp <= tgt)) {
		*next = edge_get_best(*next, opp, decreasing);
	}
	if ((tgt <= aln && aln < cur) || (cur < aln && aln <= tgt)) {
		*next = edge_get_best(*next, aln, decreasing);
	}
}
void
snap_move_to_edge(struct view *view, enum lab_edge direction,
		bool snap_to_windows, int *dx, int *dy)
{
	assert(view);
	*dx = 0;
	*dy = 0;
	struct output *output = view->output;
	if (!output_is_usable(output)) {
		return;
	}
	struct wlr_box target = view->pending;
	struct border ssd = ssd_thickness(view);
	struct wlr_box usable = output_usable_area_in_layout_coords(output);
	switch (direction) {
	case LAB_EDGE_LEFT:
		target.x = usable.x + ssd.left + rc.gap;
		if (target.x >= view->pending.x) {
			return;
		}
		break;
	case LAB_EDGE_RIGHT:
		target.x = usable.x + usable.width
			- rc.gap - target.width - ssd.right;
		if (target.x <= view->pending.x) {
			return;
		}
		break;
	case LAB_EDGE_TOP:
		target.y = usable.y + ssd.top + rc.gap;
		if (target.y >= view->pending.y) {
			return;
		}
		break;
	case LAB_EDGE_BOTTOM:
		target.y = usable.y + usable.height - rc.gap - ssd.bottom
			- view_effective_height(view,   true);
		if (target.y <= view->pending.y) {
			return;
		}
		break;
	default:
		return;
	}
	if (snap_to_windows) {
		struct border next_edges;
		edges_initialize(&next_edges);
		edges_find_neighbors(&next_edges, view, view->pending, target,
			output, LABWC_src_snap_c_check_edge,   false);
		edges_adjust_move_coords(view, next_edges,
			&target.x, &target.y,   true);
	}
	*dx = target.x - view->pending.x;
	*dy = target.y - view->pending.y;
}
void
snap_grow_to_next_edge(struct view *view,
		enum lab_edge direction, struct wlr_box *geo)
{
	assert(view);
	assert(!view->shaded);
	*geo = view->pending;
	struct output *output = view->output;
	if (!output_is_usable(output)) {
		return;
	}
	struct border ssd = ssd_thickness(view);
	struct wlr_box usable = output_usable_area_in_layout_coords(output);
	enum lab_edge resize_edges;
	switch (direction) {
	case LAB_EDGE_LEFT:
		geo->x = usable.x + ssd.left + rc.gap;
		geo->width = view->pending.x + view->pending.width - geo->x;
		resize_edges = LAB_EDGE_LEFT;
		break;
	case LAB_EDGE_RIGHT:
		geo->width = usable.x + usable.width
			- rc.gap - ssd.right - view->pending.x;
		resize_edges = LAB_EDGE_RIGHT;
		break;
	case LAB_EDGE_TOP:
		geo->y = usable.y + ssd.top + rc.gap;
		geo->height = view->pending.y + view->pending.height - geo->y;
		resize_edges = LAB_EDGE_TOP;
		break;
	case LAB_EDGE_BOTTOM:
		geo->height = usable.y + usable.height
			- rc.gap - ssd.bottom - view->pending.y;
		resize_edges = LAB_EDGE_BOTTOM;
		break;
	default:
		return;
	}
	if (geo->width < view->pending.width ||
			geo->height < view->pending.height) {
		*geo = view->pending;
		return;
	}
	if (geo->width == view->pending.width &&
			geo->height == view->pending.height) {
		*geo = view->pending;
		return;
	}
	struct border next_edges;
	edges_initialize(&next_edges);
	struct wlr_box origin =
		snap_constraints_effective(view, resize_edges,   true);
	edges_find_neighbors(&next_edges, view, origin, *geo,
		output, LABWC_src_snap_c_check_edge,   false);
	edges_adjust_resize_geom(view, next_edges,
		resize_edges, geo,   true);
	snap_constraints_set(view, resize_edges, *geo);
}
void
snap_shrink_to_next_edge(struct view *view,
		enum lab_edge direction, struct wlr_box *geo)
{
	assert(view);
	assert(!view->shaded);
	*geo = view->pending;
	enum lab_edge resize_edges;
	switch (direction) {
	case LAB_EDGE_RIGHT:
		geo->width = MAX(geo->width / 2, LAB_MIN_VIEW_WIDTH);
		geo->x = view->pending.x + view->pending.width - geo->width;
		resize_edges = LAB_EDGE_LEFT;
		break;
	case LAB_EDGE_LEFT:
		geo->width = MAX(geo->width / 2, LAB_MIN_VIEW_WIDTH);
		resize_edges = LAB_EDGE_RIGHT;
		break;
	case LAB_EDGE_BOTTOM:
		geo->height = MAX(geo->height / 2, LAB_MIN_VIEW_HEIGHT);
		geo->y = view->pending.y + view->pending.height - geo->height;
		resize_edges = LAB_EDGE_TOP;
		break;
	case LAB_EDGE_TOP:
		geo->height = MAX(geo->height / 2, LAB_MIN_VIEW_HEIGHT);
		resize_edges = LAB_EDGE_BOTTOM;
		break;
	default:
		return;
	}
	if (geo->width == view->pending.width &&
			geo->height == view->pending.height) {
		*geo = view->pending;
		return;
	}
	struct border next_edges;
	edges_initialize(&next_edges);
	struct wlr_box origin =
		snap_constraints_effective(view, resize_edges,   true);
	edges_find_outputs(&next_edges, view,
		origin, *geo, view->output, LABWC_src_snap_c_check_edge);
	edges_find_neighbors(&next_edges, view, origin, *geo,
		view->output, LABWC_src_snap_c_check_edge,   false);
	edges_adjust_resize_geom(view, next_edges,
		resize_edges, geo,   true);
	snap_constraints_set(view, resize_edges, *geo);
}
#include <assert.h>
#define PADDING 4
static void
LABWC_src_ssd_resize_indicator_c_resize_indicator_reconfigure_view(struct resize_indicator *indicator)
{
	assert(indicator->tree);
	struct theme *theme = rc.theme;
	indicator->height = font_height(&rc.font_osd)
		+ 2 * PADDING
		+ 2 * theme->osd_border_width;
	wlr_scene_node_set_position(&indicator->background->node,
		theme->osd_border_width, theme->osd_border_width);
	wlr_scene_node_set_position(&indicator->text->scene_buffer->node,
		theme->osd_border_width + PADDING,
		theme->osd_border_width + PADDING);
	wlr_scene_rect_set_color(indicator->border, theme->osd_border_color);
	wlr_scene_rect_set_color(indicator->background, theme->osd_bg_color);
}
static void
LABWC_src_ssd_resize_indicator_c_resize_indicator_init(struct view *view)
{
	assert(view);
	struct resize_indicator *indicator = &view->resize_indicator;
	assert(!indicator->tree);
	indicator->tree = lab_wlr_scene_tree_create(view->scene_tree);
	indicator->border = lab_wlr_scene_rect_create(
		indicator->tree, 0, 0, rc.theme->osd_border_color);
	indicator->background = lab_wlr_scene_rect_create(
		indicator->tree, 0, 0, rc.theme->osd_bg_color);
	indicator->text = scaled_font_buffer_create(indicator->tree);
	wlr_scene_node_set_enabled(&indicator->tree->node, false);
	LABWC_src_ssd_resize_indicator_c_resize_indicator_reconfigure_view(indicator);
}
static bool
LABWC_src_ssd_resize_indicator_c_wants_indicator(struct view *view)
{
	assert(view);
	if (rc.resize_indicator == LAB_RESIZE_INDICATOR_NON_PIXEL) {
		if (server.input_mode != LAB_INPUT_STATE_RESIZE) {
			return false;
		}
		struct view_size_hints hints = view_get_size_hints(view);
		if (hints.width_inc && hints.height_inc) {
			return true;
		}
	}
	return rc.resize_indicator == LAB_RESIZE_INDICATOR_ALWAYS;
}
void
resize_indicator_reconfigure(void)
{
	struct view *view;
	wl_list_for_each(view, &server.views, link) {
		struct resize_indicator *indicator = &view->resize_indicator;
		if (indicator->tree) {
			LABWC_src_ssd_resize_indicator_c_resize_indicator_reconfigure_view(indicator);
		}
		if (view != server.grabbed_view) {
			continue;
		}
		if (indicator->tree && indicator->tree->node.enabled) {
			if (LABWC_src_ssd_resize_indicator_c_wants_indicator(view)) {
				resize_indicator_update(view);
			} else {
				resize_indicator_hide(view);
			}
		} else if (LABWC_src_ssd_resize_indicator_c_wants_indicator(view)) {
			resize_indicator_show(view);
		}
	}
}
static void
LABWC_src_ssd_resize_indicator_c_resize_indicator_set_size(struct resize_indicator *indicator, int width)
{
	assert(indicator->tree);
	indicator->width = width
		+ 2 * PADDING
		+ 2 * rc.theme->osd_border_width;
	wlr_scene_rect_set_size(indicator->border, indicator->width, indicator->height);
	wlr_scene_rect_set_size(indicator->background,
		indicator->width - 2 * rc.theme->osd_border_width,
		indicator->height - 2 * rc.theme->osd_border_width);
}
void
resize_indicator_show(struct view *view)
{
	assert(view);
	if (!LABWC_src_ssd_resize_indicator_c_wants_indicator(view)) {
		return;
	}
	struct resize_indicator *indicator = &view->resize_indicator;
	if (!indicator->tree) {
		LABWC_src_ssd_resize_indicator_c_resize_indicator_init(view);
	}
	wlr_scene_node_raise_to_top(&indicator->tree->node);
	wlr_scene_node_set_enabled(&indicator->tree->node, true);
	resize_indicator_update(view);
}
void
resize_indicator_update(struct view *view)
{
	assert(view);
	assert(view == server.grabbed_view);
	if (!LABWC_src_ssd_resize_indicator_c_wants_indicator(view)) {
		return;
	}
	struct resize_indicator *indicator = &view->resize_indicator;
	if (!indicator->tree) {
		resize_indicator_show(view);
	}
	char text[32];
	struct wlr_box view_box;
	if (resize_outlines_enabled(view)) {
		view_box = view->resize_outlines.view_geo;
	} else {
		view_box = view->current;
		view_box.height = view_effective_height(view,   false);
	}
	if (server.input_mode == LAB_INPUT_STATE_RESIZE) {
		struct view_size_hints hints = view_get_size_hints(view);
		snprintf(text, sizeof(text), "%d x %d",
			MAX(0, view_box.width - hints.base_width)
				/ MAX(1, hints.width_inc),
			MAX(0, view_box.height - hints.base_height)
				/ MAX(1, hints.height_inc));
	} else if (server.input_mode == LAB_INPUT_STATE_MOVE) {
		struct border margin = ssd_get_margin(view->ssd);
		snprintf(text, sizeof(text), "%d , %d",
			view_box.x - margin.left,
			view_box.y - margin.top);
	} else {
		return;
	}
	int width = font_width(&rc.font_osd, text);
	LABWC_src_ssd_resize_indicator_c_resize_indicator_set_size(indicator, width);
	int x = view_box.x - view->current.x + (view_box.width - indicator->width) / 2;
	int y = view_box.y - view->current.y + (view_box.height - indicator->height) / 2;
	wlr_scene_node_set_position(&indicator->tree->node, x, y);
	scaled_font_buffer_update(indicator->text, text, width, &rc.font_osd,
		rc.theme->osd_label_text_color, rc.theme->osd_bg_color);
}
void
resize_indicator_hide(struct view *view)
{
	assert(view);
	struct resize_indicator *indicator = &view->resize_indicator;
	if (!indicator->tree) {
		return;
	}
	wlr_scene_node_set_enabled(&indicator->tree->node, false);
}
#undef PADDING
#include <assert.h>
void
ssd_border_create(struct ssd *ssd)
{
	assert(ssd);
	assert(!ssd->border.tree);
	struct view *view = ssd->view;
	struct theme *theme = rc.theme;
	int width = view->current.width;
	int height = view_effective_height(view,   false);
	int full_width = width + 2 * theme->border_width;
	int corner_width = ssd_get_corner_width();
	ssd->border.tree = lab_wlr_scene_tree_create(ssd->tree);
	wlr_scene_node_set_position(&ssd->border.tree->node, -theme->border_width, 0);
	enum ssd_active_state active;
	FOR_EACH_ACTIVE_STATE(active) {
		struct ssd_border_subtree *subtree = &ssd->border.subtrees[active];
		subtree->tree = lab_wlr_scene_tree_create(ssd->border.tree);
		struct wlr_scene_tree *parent = subtree->tree;
		wlr_scene_node_set_enabled(&parent->node, active);
		float *color = theme->window[active].border_color;
		subtree->left = lab_wlr_scene_rect_create(parent,
			theme->border_width, height, color);
		wlr_scene_node_set_position(&subtree->left->node, 0, 0);
		subtree->right = lab_wlr_scene_rect_create(parent,
			theme->border_width, height, color);
		wlr_scene_node_set_position(&subtree->right->node,
			theme->border_width + width, 0);
		subtree->bottom = lab_wlr_scene_rect_create(parent,
			full_width, theme->border_width, color);
		wlr_scene_node_set_position(&subtree->bottom->node,
			0, height);
		subtree->top = lab_wlr_scene_rect_create(parent,
			MAX(width - 2 * corner_width, 0), theme->border_width, color);
		wlr_scene_node_set_position(&subtree->top->node,
			theme->border_width + corner_width,
			-(ssd->titlebar.height + theme->border_width));
	}
	if (view->maximized == VIEW_AXIS_BOTH) {
		wlr_scene_node_set_enabled(&ssd->border.tree->node, false);
	}
	if (view->current.width > 0 && view->current.height > 0) {
		ssd_border_update(ssd);
	}
}
void
ssd_border_update(struct ssd *ssd)
{
	assert(ssd);
	assert(ssd->border.tree);
	struct view *view = ssd->view;
	if (view->maximized == VIEW_AXIS_BOTH
			&& ssd->border.tree->node.enabled) {
		wlr_scene_node_set_enabled(&ssd->border.tree->node, false);
		ssd->margin = ssd_thickness(ssd->view);
	}
	if (view->maximized == VIEW_AXIS_BOTH) {
		return;
	} else if (!ssd->border.tree->node.enabled) {
		wlr_scene_node_set_enabled(&ssd->border.tree->node, true);
		ssd->margin = ssd_thickness(ssd->view);
	}
	struct theme *theme = rc.theme;
	int width = view->current.width;
	int height = view_effective_height(view,   false);
	int full_width = width + 2 * theme->border_width;
	int corner_width = ssd_get_corner_width();
	int side_height = ssd->state.was_squared
		? height + ssd->titlebar.height
		: height;
	int side_y = ssd->state.was_squared
		? -ssd->titlebar.height
		: 0;
	int top_width = ssd->titlebar.height <= 0 || ssd->state.was_squared
		? full_width
		: MAX(width - 2 * corner_width, 0);
	int top_x = ssd->titlebar.height <= 0 || ssd->state.was_squared
		? 0
		: theme->border_width + corner_width;
	enum ssd_active_state active;
	FOR_EACH_ACTIVE_STATE(active) {
		struct ssd_border_subtree *subtree = &ssd->border.subtrees[active];
		wlr_scene_rect_set_size(subtree->left,
			theme->border_width, side_height);
		wlr_scene_node_set_position(&subtree->left->node,
			0, side_y);
		wlr_scene_rect_set_size(subtree->right,
			theme->border_width, side_height);
		wlr_scene_node_set_position(&subtree->right->node,
			theme->border_width + width, side_y);
		wlr_scene_rect_set_size(subtree->bottom,
			full_width, theme->border_width);
		wlr_scene_node_set_position(&subtree->bottom->node,
			0, height);
		wlr_scene_rect_set_size(subtree->top,
			top_width, theme->border_width);
		wlr_scene_node_set_position(&subtree->top->node,
			top_x, -(ssd->titlebar.height + theme->border_width));
	}
}
void
ssd_border_destroy(struct ssd *ssd)
{
	assert(ssd);
	assert(ssd->border.tree);
	wlr_scene_node_destroy(&ssd->border.tree->node);
	ssd->border = (struct ssd_border_scene){0};
}
#include <assert.h>
struct ssd_button *
attach_ssd_button(struct wl_list *button_parts, enum lab_node_type type,
		struct wlr_scene_tree *parent,
		struct lab_img *imgs[LAB_BS_ALL + 1],
		int x, int y, struct view *view)
{
	struct wlr_scene_tree *root = lab_wlr_scene_tree_create(parent);
	wlr_scene_node_set_position(&root->node, x, y);
	assert(node_type_contains(LAB_NODE_BUTTON, type));
	struct ssd_button *button = znew(*button);
	button->node = &root->node;
	button->type = type;
	node_descriptor_create(&root->node, type, view, button);
	wl_list_append(button_parts, &button->link);
	float invisible[4] = { 0, 0, 0, 0 };
	lab_wlr_scene_rect_create(root, rc.theme->window_button_width,
		rc.theme->window_button_height, invisible);
	for (uint8_t state_set = LAB_BS_DEFAULT;
			state_set <= LAB_BS_ALL; state_set++) {
		if (!imgs[state_set]) {
			continue;
		}
		struct scaled_img_buffer *img_buffer = scaled_img_buffer_create(
			root, imgs[state_set], rc.theme->window_button_width,
			rc.theme->window_button_height);
		assert(img_buffer);
		struct wlr_scene_node *icon_node = &img_buffer->scene_buffer->node;
		wlr_scene_node_set_enabled(icon_node, false);
		button->img_buffers[state_set] = img_buffer;
	}
	wlr_scene_node_set_enabled(
		&button->img_buffers[LAB_BS_DEFAULT]->scene_buffer->node, true);
	return button;
}
void ssd_button_free(struct ssd_button *button)
{
	wl_list_remove(&button->link);
	free(button);
}
#include <pixman.h>
void
ssd_extents_create(struct ssd *ssd)
{
	struct view *view = ssd->view;
	struct theme *theme = rc.theme;
	int border_width = MAX(0, MAX(rc.resize_minimum_area, theme->border_width));
	ssd->extents.tree = lab_wlr_scene_tree_create(ssd->tree);
	struct wlr_scene_tree *parent = ssd->extents.tree;
	if (view->fullscreen || view->maximized == VIEW_AXIS_BOTH) {
		wlr_scene_node_set_enabled(&parent->node, false);
	}
	wlr_scene_node_set_position(&parent->node,
		-border_width, -(ssd->titlebar.height + border_width));
	float invisible[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ssd->extents.top = lab_wlr_scene_rect_create(parent, 0, 0, invisible);
	ssd->extents.left = lab_wlr_scene_rect_create(parent, 0, 0, invisible);
	ssd->extents.right = lab_wlr_scene_rect_create(parent, 0, 0, invisible);
	ssd->extents.bottom = lab_wlr_scene_rect_create(parent, 0, 0, invisible);
	ssd_extents_update(ssd);
}
static void
LABWC_src_ssd_ssd_extents_c_resize_extent_within_usable(struct wlr_scene_rect *rect,
		pixman_region32_t *usable, int x, int y, int w, int h)
{
	pixman_region32_t intersection;
	pixman_region32_init(&intersection);
	pixman_region32_intersect_rect(&intersection, usable, x, y,
		(unsigned int)w, (unsigned int)h);
	int nrects;
	const pixman_box32_t *inter_rects =
		pixman_region32_rectangles(&intersection, &nrects);
	if (nrects == 0) {
		wlr_scene_node_set_enabled(&rect->node, false);
		goto out;
	}
	struct wlr_box result_box = (struct wlr_box) {
		.x = inter_rects[0].x1,
		.y = inter_rects[0].y1,
		.width = inter_rects[0].x2 - inter_rects[0].x1,
		.height = inter_rects[0].y2 - inter_rects[0].y1
	};
	wlr_scene_node_set_enabled(&rect->node, true);
	wlr_scene_node_set_position(&rect->node, result_box.x, result_box.y);
	wlr_scene_rect_set_size(rect, result_box.width, result_box.height);
out:
	pixman_region32_fini(&intersection);
}
void
ssd_extents_update(struct ssd *ssd)
{
	struct view *view = ssd->view;
	if (view->fullscreen || view->maximized == VIEW_AXIS_BOTH) {
		wlr_scene_node_set_enabled(&ssd->extents.tree->node, false);
		return;
	}
	if (!ssd->extents.tree->node.enabled) {
		wlr_scene_node_set_enabled(&ssd->extents.tree->node, true);
	}
	if (!output_is_usable(view->output)) {
		return;
	}
	struct theme *theme = rc.theme;
	int width = view->current.width;
	int height = view_effective_height(view,   false);
	int full_height = height + theme->border_width * 2 + ssd->titlebar.height;
	int full_width = width + 2 * theme->border_width;
	int border_width = MAX(rc.resize_minimum_area, theme->border_width);
	int extended_area = MAX(0, rc.resize_minimum_area - theme->border_width);
	wlr_scene_node_set_position(&ssd->extents.tree->node,
		-border_width, -(ssd->titlebar.height + border_width));
	pixman_region32_t usable;
	pixman_region32_init(&usable);
	struct output *output;
	wl_list_for_each(output, &server.outputs, link) {
		if (!view_on_output(view, output)) {
			continue;
		}
		struct wlr_box usable_area =
			output_usable_area_in_layout_coords(output);
		pixman_region32_union_rect(&usable, &usable, usable_area.x,
			usable_area.y, (unsigned int)usable_area.width,
			(unsigned int)usable_area.height);
	}
	int base_x, base_y;
	wlr_scene_node_coords(&ssd->extents.tree->node, &base_x, &base_y);
	pixman_region32_translate(&usable, -base_x, -base_y);
	LABWC_src_ssd_ssd_extents_c_resize_extent_within_usable(ssd->extents.top, &usable,
		0, 0,
		full_width + extended_area * 2, extended_area);
	LABWC_src_ssd_ssd_extents_c_resize_extent_within_usable(ssd->extents.left, &usable,
		0, extended_area,
		extended_area, full_height);
	LABWC_src_ssd_ssd_extents_c_resize_extent_within_usable(ssd->extents.right, &usable,
		extended_area + full_width, extended_area,
		extended_area, full_height);
	LABWC_src_ssd_ssd_extents_c_resize_extent_within_usable(ssd->extents.bottom, &usable,
		0, extended_area + full_height,
		full_width + extended_area * 2, extended_area);
	pixman_region32_fini(&usable);
}
void
ssd_extents_destroy(struct ssd *ssd)
{
	if (!ssd->extents.tree) {
		return;
	}
	wlr_scene_node_destroy(&ssd->extents.tree->node);
	ssd->extents = (struct ssd_extents_scene){0};
}
#include <assert.h>
static bool
LABWC_src_ssd_ssd_shadow_c_never_accepts_input(struct wlr_scene_buffer *buffer, double *sx, double *sy)
{
	return false;
}
static void
LABWC_src_ssd_ssd_shadow_c_corner_scale_crop(struct wlr_scene_buffer *buffer, int horizontal_overlap,
		int vertical_overlap, int corner_size)
{
	int width = MAX(corner_size - horizontal_overlap, 0);
	int height = MAX(corner_size - vertical_overlap, 0);
	struct wlr_fbox src_box = {
		.x = horizontal_overlap,
		.y = vertical_overlap,
		.width = width,
		.height = height,
	};
	wlr_scene_buffer_set_source_box(buffer, &src_box);
	wlr_scene_buffer_set_dest_size(buffer, width, height);
}
static void
LABWC_src_ssd_ssd_shadow_c_set_shadow_parts_geometry(struct ssd_shadow_subtree *subtree,
		int width, int height, int titlebar_height, int corner_size,
		int inset, int visible_shadow_width)
{
	bool show_topbottom = width > inset * 2;
	bool show_sides = height > inset * 2;
	int horizontal_overlap = MAX(0, inset - width / 2);
	int vertical_overlap = MAX(0, inset - height / 2);
	int horizontal_overlap_downsized = horizontal_overlap;
	if (horizontal_overlap > 0) {
		horizontal_overlap_downsized -= width % 2;
	}
	int vertical_overlap_downsized = vertical_overlap;
	if (vertical_overlap > 0) {
		vertical_overlap_downsized -= height % 2;
	}
	int x;
	int y;
	x = width - inset + horizontal_overlap_downsized;
	y = -titlebar_height + height - inset + vertical_overlap_downsized;
	wlr_scene_node_set_position(&subtree->bottom_right->node, x, y);
	LABWC_src_ssd_ssd_shadow_c_corner_scale_crop(subtree->bottom_right, horizontal_overlap_downsized,
		vertical_overlap_downsized, corner_size);
	x = -visible_shadow_width;
	y = -titlebar_height + height - inset + vertical_overlap;
	wlr_scene_node_set_position(&subtree->bottom_left->node, x, y);
	LABWC_src_ssd_ssd_shadow_c_corner_scale_crop(subtree->bottom_left, horizontal_overlap,
		vertical_overlap, corner_size);
	x = -visible_shadow_width;
	y = -titlebar_height - visible_shadow_width;
	wlr_scene_node_set_position(&subtree->top_left->node, x, y);
	LABWC_src_ssd_ssd_shadow_c_corner_scale_crop(subtree->top_left, horizontal_overlap_downsized,
		vertical_overlap_downsized, corner_size);
	x = width - inset + horizontal_overlap;
	y = -titlebar_height - visible_shadow_width;
	wlr_scene_node_set_position(&subtree->top_right->node, x, y);
	LABWC_src_ssd_ssd_shadow_c_corner_scale_crop(subtree->top_right, horizontal_overlap,
		vertical_overlap, corner_size);
	x = width;
	y = -titlebar_height + inset;
	wlr_scene_node_set_position(&subtree->right->node, x, y);
	wlr_scene_buffer_set_dest_size(subtree->right,
		visible_shadow_width, MAX(height - 2 * inset, 0));
	wlr_scene_node_set_enabled(&subtree->right->node, show_sides);
	x = inset;
	y = -titlebar_height + height;
	wlr_scene_node_set_position(&subtree->bottom->node, x, y);
	wlr_scene_buffer_set_dest_size(subtree->bottom,
		MAX(width - 2 * inset, 0), visible_shadow_width);
	wlr_scene_node_set_enabled(&subtree->bottom->node, show_topbottom);
	x = -visible_shadow_width;
	y = -titlebar_height + inset;
	wlr_scene_node_set_position(&subtree->left->node, x, y);
	wlr_scene_buffer_set_dest_size(subtree->left,
		visible_shadow_width, MAX(height - 2 * inset, 0));
	wlr_scene_node_set_enabled(&subtree->left->node, show_sides);
	x = inset;
	y = -titlebar_height - visible_shadow_width;
	wlr_scene_node_set_position(&subtree->top->node, x, y);
	wlr_scene_buffer_set_dest_size(subtree->top,
		MAX(width - 2 * inset, 0), visible_shadow_width);
	wlr_scene_node_set_enabled(&subtree->top->node, show_topbottom);
}
static void
LABWC_src_ssd_ssd_shadow_c_set_shadow_geometry(struct ssd *ssd)
{
	struct view *view = ssd->view;
	struct theme *theme = rc.theme;
	int titlebar_height = ssd->titlebar.height;
	int width = view->current.width;
	int height = view_effective_height(view, false) + titlebar_height;
	enum ssd_active_state active;
	FOR_EACH_ACTIVE_STATE(active) {
		struct ssd_shadow_subtree *subtree = &ssd->shadow.subtrees[active];
		if (!subtree->tree) {
			continue;
		}
		int visible_shadow_width = theme->window[active].shadow_size;
		double inset_proportion = SSD_SHADOW_INSET;
		int inset = (int)(inset_proportion * (double)visible_shadow_width);
		int corner_size = (int)theme->window[active]
			.shadow_corner_top->logical_height;
		LABWC_src_ssd_ssd_shadow_c_set_shadow_parts_geometry(subtree, width, height,
			titlebar_height, corner_size, inset,
			visible_shadow_width);
	}
}
static struct wlr_scene_buffer *
LABWC_src_ssd_ssd_shadow_c_make_shadow(struct view *view,
	struct wlr_scene_tree *parent, struct wlr_buffer *buf,
	enum wl_output_transform tx)
{
	struct wlr_scene_buffer *scene_buf =
		lab_wlr_scene_buffer_create(parent, buf);
	wlr_scene_buffer_set_transform(scene_buf, tx);
	scene_buf->point_accepts_input = LABWC_src_ssd_ssd_shadow_c_never_accepts_input;
	scene_buf->filter_mode = WLR_SCALE_FILTER_NEAREST;
	return scene_buf;
}
void
ssd_shadow_create(struct ssd *ssd)
{
	assert(ssd);
	assert(!ssd->shadow.tree);
	ssd->shadow.tree = lab_wlr_scene_tree_create(ssd->tree);
	struct theme *theme = rc.theme;
	struct view *view = ssd->view;
	enum ssd_active_state active;
	FOR_EACH_ACTIVE_STATE(active) {
		struct ssd_shadow_subtree *subtree = &ssd->shadow.subtrees[active];
		if (!rc.shadows_enabled) {
			continue;
		}
		if (theme->window[active].shadow_size == 0) {
			continue;
		}
		subtree->tree = lab_wlr_scene_tree_create(ssd->shadow.tree);
		struct wlr_scene_tree *parent = subtree->tree;
		struct wlr_buffer *corner_top_buffer =
			&theme->window[active].shadow_corner_top->base;
		struct wlr_buffer *corner_bottom_buffer =
			&theme->window[active].shadow_corner_bottom->base;
		struct wlr_buffer *edge_buffer =
			&theme->window[active].shadow_edge->base;
		subtree->bottom_right = LABWC_src_ssd_ssd_shadow_c_make_shadow(view, parent,
			corner_bottom_buffer, WL_OUTPUT_TRANSFORM_NORMAL);
		subtree->bottom_left = LABWC_src_ssd_ssd_shadow_c_make_shadow(view, parent,
			corner_bottom_buffer, WL_OUTPUT_TRANSFORM_FLIPPED);
		subtree->top_left = LABWC_src_ssd_ssd_shadow_c_make_shadow(view, parent,
			corner_top_buffer, WL_OUTPUT_TRANSFORM_180);
		subtree->top_right = LABWC_src_ssd_ssd_shadow_c_make_shadow(view, parent,
			corner_top_buffer, WL_OUTPUT_TRANSFORM_FLIPPED_180);
		subtree->right = LABWC_src_ssd_ssd_shadow_c_make_shadow(view, parent,
			edge_buffer, WL_OUTPUT_TRANSFORM_NORMAL);
		subtree->bottom = LABWC_src_ssd_ssd_shadow_c_make_shadow(view, parent,
			edge_buffer, WL_OUTPUT_TRANSFORM_90);
		subtree->left = LABWC_src_ssd_ssd_shadow_c_make_shadow(view, parent,
			edge_buffer, WL_OUTPUT_TRANSFORM_180);
		subtree->top = LABWC_src_ssd_ssd_shadow_c_make_shadow(view, parent,
			edge_buffer, WL_OUTPUT_TRANSFORM_270);
	}
	ssd_shadow_update(ssd);
}
void
ssd_shadow_update(struct ssd *ssd)
{
	assert(ssd);
	assert(ssd->shadow.tree);
	struct view *view = ssd->view;
	struct theme *theme = rc.theme;
	bool maximized = view->maximized == VIEW_AXIS_BOTH;
	bool tiled_shadows = false;
	if (rc.shadows_on_tiled) {
		if (rc.gap >= theme->window[SSD_ACTIVE].shadow_size
				&& rc.gap >= theme->window[SSD_INACTIVE].shadow_size) {
			tiled_shadows = true;
		} else {
		}
	};
	bool show_shadows = rc.shadows_enabled && !maximized
		&& (!view_is_tiled(ssd->view) || tiled_shadows);
	wlr_scene_node_set_enabled(&ssd->shadow.tree->node, show_shadows);
	if (show_shadows) {
		LABWC_src_ssd_ssd_shadow_c_set_shadow_geometry(ssd);
	}
}
void
ssd_shadow_destroy(struct ssd *ssd)
{
	assert(ssd);
	assert(ssd->shadow.tree);
	wlr_scene_node_destroy(&ssd->shadow.tree->node);
	ssd->shadow = (struct ssd_shadow_scene){0};
}
#include <assert.h>
#include <string.h>
static void LABWC_src_ssd_ssd_titlebar_c_set_squared_corners(struct ssd *ssd, bool enable);
static void LABWC_src_ssd_ssd_titlebar_c_set_alt_button_icon(struct ssd *ssd, enum lab_node_type type, bool enable);
static void LABWC_src_ssd_ssd_titlebar_c_update_visible_buttons(struct ssd *ssd);
void
ssd_titlebar_create(struct ssd *ssd)
{
	struct view *view = ssd->view;
	struct theme *theme = rc.theme;
	int width = view->current.width;
	int corner_width = ssd_get_corner_width();
	ssd->titlebar.tree = lab_wlr_scene_tree_create(ssd->tree);
	node_descriptor_create(&ssd->titlebar.tree->node,
		LAB_NODE_TITLEBAR, view,   NULL);
	enum ssd_active_state active;
	FOR_EACH_ACTIVE_STATE(active) {
		struct ssd_titlebar_subtree *subtree = &ssd->titlebar.subtrees[active];
		subtree->tree = lab_wlr_scene_tree_create(ssd->titlebar.tree);
		struct wlr_scene_tree *parent = subtree->tree;
		wlr_scene_node_set_enabled(&parent->node, active);
		wlr_scene_node_set_position(&parent->node, 0, -theme->titlebar_height);
		struct wlr_buffer *titlebar_fill =
			&theme->window[active].titlebar_fill->base;
		struct wlr_buffer *corner_top_left =
			&theme->window[active].corner_top_left_normal->base;
		struct wlr_buffer *corner_top_right =
			&theme->window[active].corner_top_right_normal->base;
		subtree->bar = lab_wlr_scene_buffer_create(parent, titlebar_fill);
		if (wlr_renderer_is_pixman(server.renderer)) {
			wlr_scene_buffer_set_filter_mode(
				subtree->bar, WLR_SCALE_FILTER_NEAREST);
		}
		wlr_scene_node_set_position(&subtree->bar->node, corner_width, 0);
		subtree->corner_left = lab_wlr_scene_buffer_create(parent, corner_top_left);
		wlr_scene_node_set_position(&subtree->corner_left->node,
			-rc.theme->border_width, -rc.theme->border_width);
		subtree->corner_right = lab_wlr_scene_buffer_create(parent, corner_top_right);
		wlr_scene_node_set_position(&subtree->corner_right->node,
			width - corner_width, -rc.theme->border_width);
		int x = theme->window_titlebar_padding_width;
		int y = (theme->titlebar_height - theme->window_button_height) / 2;
		wl_list_init(&subtree->buttons_left);
		wl_list_init(&subtree->buttons_right);
		for (int b = 0; b < rc.nr_title_buttons_left; b++) {
			enum lab_node_type type = rc.title_buttons_left[b];
			struct lab_img **imgs =
				theme->window[active].button_imgs[type];
			attach_ssd_button(&subtree->buttons_left, type, parent,
				imgs, x, y, view);
			x += theme->window_button_width + theme->window_button_spacing;
		}
		x = width - theme->window_titlebar_padding_width + theme->window_button_spacing;
		for (int b = rc.nr_title_buttons_right - 1; b >= 0; b--) {
			x -= theme->window_button_width + theme->window_button_spacing;
			enum lab_node_type type = rc.title_buttons_right[b];
			struct lab_img **imgs =
				theme->window[active].button_imgs[type];
			attach_ssd_button(&subtree->buttons_right, type, parent,
				imgs, x, y, view);
		}
	}
	LABWC_src_ssd_ssd_titlebar_c_update_visible_buttons(ssd);
	bool maximized = view->maximized == VIEW_AXIS_BOTH;
	bool squared = ssd_should_be_squared(ssd);
	if (maximized) {
		LABWC_src_ssd_ssd_titlebar_c_set_alt_button_icon(ssd, LAB_NODE_BUTTON_MAXIMIZE, true);
		ssd->state.was_maximized = true;
	}
	if (squared) {
		ssd->state.was_squared = true;
	}
	LABWC_src_ssd_ssd_titlebar_c_set_squared_corners(ssd, maximized || squared);
	if (view->shaded) {
		LABWC_src_ssd_ssd_titlebar_c_set_alt_button_icon(ssd, LAB_NODE_BUTTON_SHADE, true);
	}
	if (view->visible_on_all_workspaces) {
		LABWC_src_ssd_ssd_titlebar_c_set_alt_button_icon(ssd, LAB_NODE_BUTTON_OMNIPRESENT, true);
	}
}
static void
LABWC_src_ssd_ssd_titlebar_c_update_button_state(struct ssd_button *button, enum lab_button_state state,
		bool enable)
{
	if (enable) {
		button->state_set |= (uint8_t)state;
	} else {
		button->state_set &= (uint8_t)~state;
	}
	for (uint8_t state_set = LAB_BS_DEFAULT;
			state_set <= LAB_BS_ALL; state_set++) {
		struct scaled_img_buffer *buffer = button->img_buffers[state_set];
		if (!buffer) {
			continue;
		}
		wlr_scene_node_set_enabled(&buffer->scene_buffer->node,
			state_set == button->state_set);
	}
}
static void
LABWC_src_ssd_ssd_titlebar_c_set_squared_corners(struct ssd *ssd, bool enable)
{
	struct view *view = ssd->view;
	int width = view->current.width;
	int corner_width = ssd_get_corner_width();
	struct theme *theme = rc.theme;
	int x = enable ? 0 : corner_width;
	enum ssd_active_state active;
	FOR_EACH_ACTIVE_STATE(active) {
		struct ssd_titlebar_subtree *subtree = &ssd->titlebar.subtrees[active];
		wlr_scene_node_set_position(&subtree->bar->node, x, 0);
		wlr_scene_buffer_set_dest_size(subtree->bar,
			MAX(width - 2 * x, 0), theme->titlebar_height);
		wlr_scene_node_set_enabled(&subtree->corner_left->node, !enable);
		wlr_scene_node_set_enabled(&subtree->corner_right->node, !enable);
		struct ssd_button *button;
		wl_list_for_each(button, &subtree->buttons_left, link) {
			LABWC_src_ssd_ssd_titlebar_c_update_button_state(button, LAB_BS_ROUNDED, !enable);
			break;
		}
		wl_list_for_each(button, &subtree->buttons_right, link) {
			LABWC_src_ssd_ssd_titlebar_c_update_button_state(button, LAB_BS_ROUNDED, !enable);
			break;
		}
	}
}
static void
LABWC_src_ssd_ssd_titlebar_c_set_alt_button_icon(struct ssd *ssd, enum lab_node_type type, bool enable)
{
	enum ssd_active_state active;
	FOR_EACH_ACTIVE_STATE(active) {
		struct ssd_titlebar_subtree *subtree = &ssd->titlebar.subtrees[active];
		struct ssd_button *button;
		wl_list_for_each(button, &subtree->buttons_left, link) {
			if (button->type == type) {
				LABWC_src_ssd_ssd_titlebar_c_update_button_state(button,
					LAB_BS_TOGGLED, enable);
			}
		}
		wl_list_for_each(button, &subtree->buttons_right, link) {
			if (button->type == type) {
				LABWC_src_ssd_ssd_titlebar_c_update_button_state(button,
					LAB_BS_TOGGLED, enable);
			}
		}
	}
}
static void
LABWC_src_ssd_ssd_titlebar_c_update_visible_buttons(struct ssd *ssd)
{
	struct view *view = ssd->view;
	struct theme *theme = rc.theme;
	int width = MAX(view->current.width - 2 * theme->window_titlebar_padding_width, 0);
	int button_width = theme->window_button_width;
	int button_spacing = theme->window_button_spacing;
	int button_count_left = rc.nr_title_buttons_left;
	int button_count_right = rc.nr_title_buttons_right;
	assert(button_width > 0);
	while (width < ((button_width + button_spacing)
			* (button_count_left + button_count_right))) {
		if (button_count_left > button_count_right) {
			button_count_left--;
		} else {
			button_count_right--;
		}
	}
	enum ssd_active_state active;
	FOR_EACH_ACTIVE_STATE(active) {
		struct ssd_titlebar_subtree *subtree = &ssd->titlebar.subtrees[active];
		int button_count = 0;
		struct ssd_button *button;
		wl_list_for_each(button, &subtree->buttons_left, link) {
			wlr_scene_node_set_enabled(button->node,
				button_count < button_count_left);
			button_count++;
		}
		button_count = 0;
		wl_list_for_each(button, &subtree->buttons_right, link) {
			wlr_scene_node_set_enabled(button->node,
				button_count < button_count_right);
			button_count++;
		}
	}
}
void
ssd_titlebar_update(struct ssd *ssd)
{
	struct view *view = ssd->view;
	int width = view->current.width;
	int corner_width = ssd_get_corner_width();
	struct theme *theme = rc.theme;
	bool maximized = view->maximized == VIEW_AXIS_BOTH;
	bool squared = ssd_should_be_squared(ssd);
	if (ssd->state.was_maximized != maximized
			|| ssd->state.was_squared != squared) {
		LABWC_src_ssd_ssd_titlebar_c_set_squared_corners(ssd, maximized || squared);
		if (ssd->state.was_maximized != maximized) {
			LABWC_src_ssd_ssd_titlebar_c_set_alt_button_icon(ssd, LAB_NODE_BUTTON_MAXIMIZE, maximized);
		}
		ssd->state.was_maximized = maximized;
		ssd->state.was_squared = squared;
	}
	if (ssd->state.was_shaded != view->shaded) {
		LABWC_src_ssd_ssd_titlebar_c_set_alt_button_icon(ssd, LAB_NODE_BUTTON_SHADE, view->shaded);
		ssd->state.was_shaded = view->shaded;
	}
	if (ssd->state.was_omnipresent != view->visible_on_all_workspaces) {
		LABWC_src_ssd_ssd_titlebar_c_set_alt_button_icon(ssd, LAB_NODE_BUTTON_OMNIPRESENT,
			view->visible_on_all_workspaces);
		ssd->state.was_omnipresent = view->visible_on_all_workspaces;
	}
	if (width == ssd->state.geometry.width) {
		return;
	}
	LABWC_src_ssd_ssd_titlebar_c_update_visible_buttons(ssd);
	int y = (theme->titlebar_height - theme->window_button_height) / 2;
	int x;
	int bg_offset = maximized || squared ? 0 : corner_width;
	enum ssd_active_state active;
	FOR_EACH_ACTIVE_STATE(active) {
		struct ssd_titlebar_subtree *subtree = &ssd->titlebar.subtrees[active];
		wlr_scene_buffer_set_dest_size(subtree->bar,
			MAX(width - bg_offset * 2, 0), theme->titlebar_height);
		x = theme->window_titlebar_padding_width;
		struct ssd_button *button;
		wl_list_for_each(button, &subtree->buttons_left, link) {
			wlr_scene_node_set_position(button->node, x, y);
			x += theme->window_button_width + theme->window_button_spacing;
		}
		x = width - corner_width;
		wlr_scene_node_set_position(&subtree->corner_right->node,
			x, -rc.theme->border_width);
		x = width - theme->window_titlebar_padding_width + theme->window_button_spacing;
		wl_list_for_each(button, &subtree->buttons_right, link) {
			x -= theme->window_button_width + theme->window_button_spacing;
			wlr_scene_node_set_position(button->node, x, y);
		}
	}
}
void
ssd_titlebar_destroy(struct ssd *ssd)
{
	if (!ssd->titlebar.tree) {
		return;
	}
	wlr_scene_node_destroy(&ssd->titlebar.tree->node);
	ssd->titlebar = (struct ssd_titlebar_scene){0};
}
void
ssd_update_hovered_button(struct wlr_scene_node *node)
{
	struct ssd_button *button = NULL;
	if (node && node->data) {
		button = node_try_ssd_button_from_node(node);
		if (button == server.hovered_button) {
			return;
		}
	}
	if (server.hovered_button) {
		LABWC_src_ssd_ssd_titlebar_c_update_button_state(server.hovered_button, LAB_BS_HOVERED, false);
	}
	server.hovered_button = button;
	if (button) {
		LABWC_src_ssd_ssd_titlebar_c_update_button_state(button, LAB_BS_HOVERED, true);
	}
}
bool
ssd_should_be_squared(struct ssd *ssd)
{
	struct view *view = ssd->view;
	int corner_width = ssd_get_corner_width();
	return (view_is_tiled_and_notify_tiled(view)
			|| view->current.width < corner_width * 2)
		&& view->maximized != VIEW_AXIS_BOTH;
}
#include <assert.h>
#include <strings.h>
struct border
ssd_thickness(struct view *view)
{
	assert(view);
	if (!view->ssd_mode || view->fullscreen) {
		return (struct border){ 0 };
	}
	struct theme *theme = rc.theme;
	if (view->maximized == VIEW_AXIS_BOTH) {
		struct border thickness = { 0 };
		if (view_titlebar_visible(view)) {
			thickness.top += theme->titlebar_height;
		}
		return thickness;
	}
	struct border thickness = {
		.top = theme->titlebar_height + theme->border_width,
		.right = theme->border_width,
		.bottom = theme->border_width,
		.left = theme->border_width,
	};
	if (!view_titlebar_visible(view)) {
		thickness.top -= theme->titlebar_height;
	}
	return thickness;
}
struct wlr_box
ssd_max_extents(struct view *view)
{
	assert(view);
	struct border border = ssd_thickness(view);
	int eff_width = view->current.width;
	int eff_height = view_effective_height(view,   false);
	return (struct wlr_box){
		.x = view->current.x - border.left,
		.y = view->current.y - border.top,
		.width = eff_width + border.left + border.right,
		.height = eff_height + border.top + border.bottom,
	};
}
enum lab_node_type
ssd_get_resizing_type(const struct ssd *ssd, struct wlr_cursor *cursor)
{
	struct view *view = ssd ? ssd->view : NULL;
	if (!view || !cursor || !view->ssd_mode || view->fullscreen) {
		return LAB_NODE_NONE;
	}
	struct wlr_box view_box = view->current;
	view_box.height = view_effective_height(view,   false);
	if (view_titlebar_visible(view)) {
		int titlebar_height = rc.theme->titlebar_height;
		view_box.y -= titlebar_height;
		view_box.height += titlebar_height;
	}
	if (wlr_box_contains_point(&view_box, cursor->x, cursor->y)) {
		return LAB_NODE_NONE;
	}
	int corner_height = MAX(0, MIN(rc.resize_corner_range, view_box.height / 2));
	int corner_width = MAX(0, MIN(rc.resize_corner_range, view_box.width / 2));
	bool left = cursor->x < view_box.x + corner_width;
	bool right = cursor->x > view_box.x + view_box.width - corner_width;
	bool top = cursor->y < view_box.y + corner_height;
	bool bottom = cursor->y > view_box.y + view_box.height - corner_height;
	if (top && left) {
		return LAB_NODE_CORNER_TOP_LEFT;
	} else if (top && right) {
		return LAB_NODE_CORNER_TOP_RIGHT;
	} else if (bottom && left) {
		return LAB_NODE_CORNER_BOTTOM_LEFT;
	} else if (bottom && right) {
		return LAB_NODE_CORNER_BOTTOM_RIGHT;
	} else if (top) {
		return LAB_NODE_BORDER_TOP;
	} else if (bottom) {
		return LAB_NODE_BORDER_BOTTOM;
	} else if (left) {
		return LAB_NODE_BORDER_LEFT;
	} else if (right) {
		return LAB_NODE_BORDER_RIGHT;
	}
	return LAB_NODE_NONE;
}
struct ssd *
ssd_create(struct view *view, bool active)
{
	assert(view);
	struct ssd *ssd = znew(*ssd);
	ssd->view = view;
	ssd->tree = lab_wlr_scene_tree_create(view->scene_tree);
	node_descriptor_create(&ssd->tree->node,
		LAB_NODE_SSD_ROOT, view,   NULL);
	wlr_scene_node_lower_to_bottom(&ssd->tree->node);
	ssd->titlebar.height = rc.theme->titlebar_height;
	ssd_shadow_create(ssd);
	ssd_extents_create(ssd);
	ssd_titlebar_create(ssd);
	ssd_border_create(ssd);
	if (!view_titlebar_visible(view)) {
		ssd_set_titlebar(ssd, false);
	}
	ssd->margin = ssd_thickness(view);
	ssd_set_active(ssd, active);
	ssd_enable_keybind_inhibit_indicator(ssd, view->inhibits_keybinds);
	ssd->state.geometry = view->current;
	return ssd;
}
struct border
ssd_get_margin(const struct ssd *ssd)
{
	return ssd ? ssd->margin : (struct border){ 0 };
}
int
ssd_get_corner_width(void)
{
	return MAX(rc.corner_radius, 5);
}
void
ssd_update_margin(struct ssd *ssd)
{
	if (!ssd) {
		return;
	}
	ssd->margin = ssd_thickness(ssd->view);
}
void
ssd_update_geometry(struct ssd *ssd)
{
	if (!ssd) {
		return;
	}
	struct view *view = ssd->view;
	assert(view);
	struct wlr_box cached = ssd->state.geometry;
	struct wlr_box current = view->current;
	int eff_width = current.width;
	int eff_height = view_effective_height(view,   false);
	bool update_area = eff_width != cached.width || eff_height != cached.height;
	bool update_extents = update_area
		|| current.x != cached.x || current.y != cached.y;
	bool maximized = view->maximized == VIEW_AXIS_BOTH;
	bool squared = ssd_should_be_squared(ssd);
	bool state_changed = ssd->state.was_maximized != maximized
		|| ssd->state.was_shaded != view->shaded
		|| ssd->state.was_squared != squared
		|| ssd->state.was_omnipresent != view->visible_on_all_workspaces;
	ssd_set_titlebar(ssd, view_titlebar_visible(view));
	if (update_extents) {
		ssd_extents_update(ssd);
	}
	if (update_area || state_changed) {
		ssd_titlebar_update(ssd);
		ssd_border_update(ssd);
		ssd_shadow_update(ssd);
	}
	if (update_extents) {
		ssd->state.geometry = current;
	}
}
void
ssd_set_titlebar(struct ssd *ssd, bool enabled)
{
	if (!ssd || ssd->titlebar.tree->node.enabled == enabled) {
		return;
	}
	wlr_scene_node_set_enabled(&ssd->titlebar.tree->node, enabled);
	ssd->titlebar.height = enabled ? rc.theme->titlebar_height : 0;
	ssd_border_update(ssd);
	ssd_extents_update(ssd);
	ssd_shadow_update(ssd);
	ssd->margin = ssd_thickness(ssd->view);
}
void
ssd_destroy(struct ssd *ssd)
{
	if (!ssd) {
		return;
	}
	struct view *view = ssd->view;
	if (server.hovered_button && node_view_from_node(
			server.hovered_button->node) == view) {
		server.hovered_button = NULL;
	}
	ssd_titlebar_destroy(ssd);
	ssd_border_destroy(ssd);
	ssd_extents_destroy(ssd);
	ssd_shadow_destroy(ssd);
	wlr_scene_node_destroy(&ssd->tree->node);
	free(ssd);
}
enum lab_ssd_mode
ssd_mode_parse(const char *mode)
{
	if (!mode) {
		return LAB_SSD_MODE_INVALID;
	}
	if (!strcasecmp(mode, "none")) {
		return LAB_SSD_MODE_NONE;
	} else if (!strcasecmp(mode, "border")) {
		return LAB_SSD_MODE_BORDER;
	} else if (!strcasecmp(mode, "full")) {
		return LAB_SSD_MODE_FULL;
	} else {
		return LAB_SSD_MODE_INVALID;
	}
}
void
ssd_set_active(struct ssd *ssd, bool active)
{
	if (!ssd) {
		return;
	}
	enum ssd_active_state active_state;
	FOR_EACH_ACTIVE_STATE(active_state) {
		wlr_scene_node_set_enabled(
			&ssd->border.subtrees[active_state].tree->node,
			active == active_state);
		wlr_scene_node_set_enabled(
			&ssd->titlebar.subtrees[active_state].tree->node,
			active == active_state);
		if (ssd->shadow.subtrees[active_state].tree) {
			wlr_scene_node_set_enabled(
				&ssd->shadow.subtrees[active_state].tree->node,
				active == active_state);
		}
	}
}
void
ssd_enable_shade(struct ssd *ssd, bool enable)
{
	if (!ssd) {
		return;
	}
	ssd_titlebar_update(ssd);
	ssd_border_update(ssd);
	wlr_scene_node_set_enabled(&ssd->extents.tree->node, !enable);
	ssd_shadow_update(ssd);
}
void
ssd_enable_keybind_inhibit_indicator(struct ssd *ssd, bool enable)
{
	if (!ssd) {
		return;
	}
	float *color = enable
		? rc.theme->window_toggled_keybinds_color
		: rc.theme->window[SSD_ACTIVE].border_color;
	wlr_scene_rect_set_color(ssd->border.subtrees[SSD_ACTIVE].top, color);
}
#ifdef FULLSCREEN_TEARING
struct LABWC_src_tearing_c_tearing_controller {
	struct wlr_tearing_control_v1 *tearing_control;
	struct wl_listener set_hint;
	struct wl_listener destroy;
};
static void
LABWC_src_tearing_c_handle_controller_set_hint(struct wl_listener *listener, void *data)
{
	struct LABWC_src_tearing_c_tearing_controller *controller = wl_container_of(listener, controller, set_hint);
	struct view *view = view_from_wlr_surface(controller->tearing_control->surface);
	if (view) {
		view->tearing_hint = controller->tearing_control->current;
	}
}
static void
LABWC_src_tearing_c_handle_controller_destroy(struct wl_listener *listener, void *data)
{
	struct LABWC_src_tearing_c_tearing_controller *controller = wl_container_of(listener, controller, destroy);
	wl_list_remove(&controller->set_hint.link);
	wl_list_remove(&controller->destroy.link);
	free(controller);
}
void
handle_tearing_new_object(struct wl_listener *listener, void *data)
{
	struct wlr_tearing_control_v1 *tearing_control = data;
	struct LABWC_src_tearing_c_tearing_controller *controller = znew(*controller);
	controller->tearing_control = tearing_control;
	controller->set_hint.notify = LABWC_src_tearing_c_handle_controller_set_hint;
	wl_signal_add(&tearing_control->events.set_hint, &controller->set_hint);
	controller->destroy.notify = LABWC_src_tearing_c_handle_controller_destroy;
	wl_signal_add(&tearing_control->events.destroy, &controller->destroy);
}
#endif
#include <assert.h>
#include <cairo.h>
#include <drm_fourcc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
struct LABWC_src_theme_c_button {
	const char *name;
	const char *alt_name;
	const char *fallback_button;
	enum lab_node_type type;
	uint8_t state_set;
};
enum LABWC_src_theme_c_rounded_corner {
	ROUNDED_CORNER_TOP_LEFT,
	ROUNDED_CORNER_TOP_RIGHT
};
struct LABWC_src_theme_c_rounded_corner_ctx {
	struct wlr_box *box;
	double radius;
	double line_width;
	cairo_pattern_t *fill_pattern;
	float *border_color;
	enum LABWC_src_theme_c_rounded_corner corner;
};
#define zero_array(arr) memset(arr, 0, sizeof(arr))
static struct lab_data_buffer *LABWC_src_theme_c_rounded_rect(struct LABWC_src_theme_c_rounded_corner_ctx *ctx);
static const double LABWC_src_theme_c_deg = 0.017453292519943295;
static void
LABWC_src_theme_c_zdrop(struct lab_data_buffer **buffer)
{
	if (*buffer) {
		wlr_buffer_drop(&(*buffer)->base);
		*buffer = NULL;
	}
}
static void
LABWC_src_theme_c_whiten_button(cairo_t *cairo, int w, int h)
{
	cairo_set_operator(cairo, CAIRO_OPERATOR_IN);
	cairo_set_source_rgba(cairo, 1.0, 1.0, 1.0, 1.0);
	cairo_rectangle(cairo, 0, 0, w, h);
	cairo_fill(cairo);
	cairo_set_operator(cairo, CAIRO_OPERATOR_OVER);
}
static void
LABWC_src_theme_c_draw_hover_overlay_on_button(cairo_t *cairo, int w, int h)
{
	set_cairo_color(cairo, rc.theme->window_button_hover_bg_color);
	int r = rc.theme->window_button_hover_bg_corner_radius;
	cairo_new_sub_path(cairo);
	cairo_arc(cairo, r, r, r, 180 * LABWC_src_theme_c_deg, 270 * LABWC_src_theme_c_deg);
	cairo_line_to(cairo, w - r, 0);
	cairo_arc(cairo, w - r, r, r, -90 * LABWC_src_theme_c_deg, 0 * LABWC_src_theme_c_deg);
	cairo_line_to(cairo, w, h - r);
	cairo_arc(cairo, w - r, h - r, r, 0 * LABWC_src_theme_c_deg, 90 * LABWC_src_theme_c_deg);
	cairo_line_to(cairo, r, h);
	cairo_arc(cairo, r, h - r, r, 90 * LABWC_src_theme_c_deg, 180 * LABWC_src_theme_c_deg);
	cairo_close_path(cairo);
	cairo_fill(cairo);
}
static void
LABWC_src_theme_c_round_left_corner_button(cairo_t *cairo, int w, int h)
{
	double x = -rc.theme->window_titlebar_padding_width;
	double y = -(rc.theme->titlebar_height - rc.theme->window_button_height) / 2;
	double r = rc.corner_radius - (double)rc.theme->border_width / 2.0;
	cairo_new_sub_path(cairo);
	cairo_arc(cairo, x + r, y + r, r, LABWC_src_theme_c_deg * 180, LABWC_src_theme_c_deg * 270);
	cairo_line_to(cairo, w, y);
	cairo_line_to(cairo, w, h);
	cairo_line_to(cairo, x, h);
	cairo_close_path(cairo);
	cairo_set_source_rgba(cairo, 1, 1, 1, 1);
	cairo_set_operator(cairo, CAIRO_OPERATOR_DEST_IN);
	cairo_fill(cairo);
}
static void
LABWC_src_theme_c_round_right_corner_button(cairo_t *cairo, int w, int h)
{
	cairo_scale(cairo, -1, 1);
	cairo_translate(cairo, -w, 0);
	LABWC_src_theme_c_round_left_corner_button(cairo, w, h);
}
static void
LABWC_src_theme_c_load_button(struct theme *theme, struct LABWC_src_theme_c_button *b, enum ssd_active_state active)
{
	struct lab_img *(*button_imgs)[LAB_BS_ALL + 1] =
		theme->window[active].button_imgs;
	struct lab_img **img = &button_imgs[b->type][b->state_set];
	float *rgba = theme->window[active].button_colors[b->type];
	assert(!*img);
	if (!*img && b->fallback_button) {
		*img = lab_img_load_from_bitmap(b->fallback_button, rgba);
	}
	if (!*img && (b->state_set & LAB_BS_HOVERED)) {
		struct lab_img *non_hover_img =
			button_imgs[b->type][b->state_set & ~LAB_BS_HOVERED];
		*img = lab_img_copy(non_hover_img);
		lab_img_add_modifier(*img, LABWC_src_theme_c_whiten_button);
		lab_img_add_modifier(*img,
			LABWC_src_theme_c_draw_hover_overlay_on_button);
	}
	struct lab_img **rounded_img =
		&button_imgs[b->type][b->state_set | LAB_BS_ROUNDED];
	if (rc.nr_title_buttons_left > 0
			&& b->type == rc.title_buttons_left[0]) {
		*rounded_img = lab_img_copy(*img);
		lab_img_add_modifier(*rounded_img, LABWC_src_theme_c_round_left_corner_button);
	}
	if (rc.nr_title_buttons_right > 0
			&& b->type == rc.title_buttons_right
				[rc.nr_title_buttons_right - 1]) {
		*rounded_img = lab_img_copy(*img);
		lab_img_add_modifier(*rounded_img, LABWC_src_theme_c_round_right_corner_button);
	}
}
static void
LABWC_src_theme_c_load_buttons(struct theme *theme)
{
	struct LABWC_src_theme_c_button buttons[] = { {
		.name = "menu",
		.fallback_button = (const char[]){ 0x00, 0x21, 0x33, 0x1E, 0x0C, 0x00 },
		.type = LAB_NODE_BUTTON_WINDOW_MENU,
		.state_set = 0,
	}, {
		.name = "iconify",
		.fallback_button = (const char[]){ 0x00, 0x00, 0x00, 0x00, 0x3f, 0x3f },
		.type = LAB_NODE_BUTTON_ICONIFY,
		.state_set = 0,
	}, {
		.name = "max",
		.fallback_button = (const char[]){ 0x3f, 0x3f, 0x21, 0x21, 0x21, 0x3f },
		.type = LAB_NODE_BUTTON_MAXIMIZE,
		.state_set = 0,
	}, {
		.name = "max_toggled",
		.fallback_button = (const char[]){ 0x3e, 0x22, 0x2f, 0x29, 0x39, 0x0f },
		.type = LAB_NODE_BUTTON_MAXIMIZE,
		.state_set = LAB_BS_TOGGLED,
	}, {
		.name = "shade",
		.fallback_button = (const char[]){ 0x3f, 0x3f, 0x00, 0x0c, 0x1e, 0x3f },
		.type = LAB_NODE_BUTTON_SHADE,
		.state_set = 0,
	}, {
		.name = "shade_toggled",
		.fallback_button = (const char[]){ 0x3f, 0x3f, 0x00, 0x3f, 0x1e, 0x0c },
		.type = LAB_NODE_BUTTON_SHADE,
		.state_set = LAB_BS_TOGGLED,
	}, {
		.name = "desk",
		.fallback_button = (const char[]){ 0x33, 0x33, 0x00, 0x00, 0x33, 0x33 },
		.type = LAB_NODE_BUTTON_OMNIPRESENT,
		.state_set = 0,
	}, {
		.name = "desk_toggled",
		.fallback_button = (const char[]){ 0x00, 0x1e, 0x1a, 0x16, 0x1e, 0x00 },
		.type = LAB_NODE_BUTTON_OMNIPRESENT,
		.state_set = LAB_BS_TOGGLED,
	}, {
		.name = "close",
		.fallback_button = (const char[]){ 0x33, 0x3f, 0x1e, 0x1e, 0x3f, 0x33 },
		.type = LAB_NODE_BUTTON_CLOSE,
		.state_set = 0,
	}, {
		.name = "menu_hover",
		.type = LAB_NODE_BUTTON_WINDOW_MENU,
		.state_set = LAB_BS_HOVERED,
	}, {
		.name = "iconify_hover",
		.type = LAB_NODE_BUTTON_ICONIFY,
		.state_set = LAB_BS_HOVERED,
	}, {
		.name = "max_hover",
		.type = LAB_NODE_BUTTON_MAXIMIZE,
		.state_set = LAB_BS_HOVERED,
	}, {
		.name = "max_toggled_hover",
		.alt_name = "max_hover_toggled",
		.type = LAB_NODE_BUTTON_MAXIMIZE,
		.state_set = LAB_BS_TOGGLED | LAB_BS_HOVERED,
	}, {
		.name = "shade_hover",
		.type = LAB_NODE_BUTTON_SHADE,
		.state_set = LAB_BS_HOVERED,
	}, {
		.name = "shade_toggled_hover",
		.alt_name = "shade_hover_toggled",
		.type = LAB_NODE_BUTTON_SHADE,
		.state_set = LAB_BS_TOGGLED | LAB_BS_HOVERED,
	}, {
		.name = "desk_hover",
		.type = LAB_NODE_BUTTON_OMNIPRESENT,
		.state_set = LAB_BS_HOVERED,
	}, {
		.name = "desk_toggled_hover",
		.alt_name = "desk_hover_toggled",
		.type = LAB_NODE_BUTTON_OMNIPRESENT,
		.state_set = LAB_BS_TOGGLED | LAB_BS_HOVERED,
	}, {
		.name = "close_hover",
		.type = LAB_NODE_BUTTON_CLOSE,
		.state_set = LAB_BS_HOVERED,
	}, };
	for (size_t i = 0; i < ARRAY_SIZE(buttons); ++i) {
		struct LABWC_src_theme_c_button *b = &buttons[i];
		LABWC_src_theme_c_load_button(theme, b, SSD_INACTIVE);
		LABWC_src_theme_c_load_button(theme, b, SSD_ACTIVE);
	}
}
static int
LABWC_src_theme_c_hex_to_dec(char c)
{
	if (c >= '0' && c <= '9') {
		return c - '0';
	}
	if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	}
	if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	}
	return 0;
}
static void
LABWC_src_theme_c_parse_hexstr(const char *hex, float *rgba)
{
	if (hex[0] != '#') {
		return;
	}
	size_t len = strlen(hex);
	if (len == 4) {
		rgba[0] = (float)((LABWC_src_theme_c_hex_to_dec(hex[1]) * 16) / 255.0);
		rgba[1] = (float)((LABWC_src_theme_c_hex_to_dec(hex[2]) * 16) / 255.0);
		rgba[2] = (float)((LABWC_src_theme_c_hex_to_dec(hex[3]) * 16) / 255.0);
	} else if (len >= 7) {
		rgba[0] = (float)((LABWC_src_theme_c_hex_to_dec(hex[1]) * 16
			+ LABWC_src_theme_c_hex_to_dec(hex[2])) / 255.0);
		rgba[1] = (float)((LABWC_src_theme_c_hex_to_dec(hex[3]) * 16
			+ LABWC_src_theme_c_hex_to_dec(hex[4])) / 255.0);
		rgba[2] = (float)((LABWC_src_theme_c_hex_to_dec(hex[5]) * 16
			+ LABWC_src_theme_c_hex_to_dec(hex[6])) / 255.0);
	} else {
		return;
	}
	rgba[3] = 1.0;
	if (len >= 9 && hex[7] == ' ') {
		rgba[3] = (float)(atoi(hex + 8) / 100.0);
	} else if (len == 9) {
		rgba[3] = (float)((LABWC_src_theme_c_hex_to_dec(hex[7]) * 16
			+ LABWC_src_theme_c_hex_to_dec(hex[8])) / 255.0);
	} else if (len > 7) {
	}
	rgba[0] *= rgba[3];
	rgba[1] *= rgba[3];
	rgba[2] *= rgba[3];
}
static enum lab_justification
LABWC_src_theme_c_parse_justification(const char *str)
{
	if (!strcasecmp(str, "Center")) {
		return LAB_JUSTIFY_CENTER;
	} else if (!strcasecmp(str, "Right")) {
		return LAB_JUSTIFY_RIGHT;
	} else {
		return LAB_JUSTIFY_LEFT;
	}
}
static void
LABWC_src_theme_c_theme_builtin(struct theme *theme)
{
	theme->border_width = 1;
	theme->window_titlebar_padding_height = 0;
	theme->window_titlebar_padding_width = 0;
	LABWC_src_theme_c_parse_hexstr("#404040", theme->window[SSD_ACTIVE].border_color);
	LABWC_src_theme_c_parse_hexstr("#404040", theme->window[SSD_INACTIVE].border_color);
	LABWC_src_theme_c_parse_hexstr("#ff0000", theme->window_toggled_keybinds_color);
	theme->window[SSD_ACTIVE].title_bg.gradient = LAB_GRADIENT_NONE;
	theme->window[SSD_INACTIVE].title_bg.gradient = LAB_GRADIENT_NONE;
	LABWC_src_theme_c_parse_hexstr("#030303f5", theme->window[SSD_ACTIVE].title_bg.color);
	LABWC_src_theme_c_parse_hexstr("#030303f5", theme->window[SSD_INACTIVE].title_bg.color);
	theme->window[SSD_ACTIVE].title_bg.color_split_to[0] = FLT_MIN;
	theme->window[SSD_INACTIVE].title_bg.color_split_to[0] = FLT_MIN;
	theme->window[SSD_ACTIVE].title_bg.color_to[0] = FLT_MIN;
	theme->window[SSD_INACTIVE].title_bg.color_to[0] = FLT_MIN;
	theme->window[SSD_ACTIVE].title_bg.color_to_split_to[0] = FLT_MIN;
	theme->window[SSD_INACTIVE].title_bg.color_to_split_to[0] = FLT_MIN;
	LABWC_src_theme_c_parse_hexstr("#ffffff", theme->window[SSD_ACTIVE].label_text_color);
	LABWC_src_theme_c_parse_hexstr("#ffffff", theme->window[SSD_INACTIVE].label_text_color);
	theme->window_label_text_justify = LABWC_src_theme_c_parse_justification("Center");
	theme->window_button_width = 26;
	theme->window_button_height = 26;
	theme->window_button_spacing = 0;
	LABWC_src_theme_c_parse_hexstr("#9999991a", theme->window_button_hover_bg_color);
	theme->window_button_hover_bg_corner_radius = 4;
	for (enum lab_node_type type = LAB_NODE_BUTTON_FIRST;
			type <= LAB_NODE_BUTTON_LAST; type++) {
		LABWC_src_theme_c_parse_hexstr("#ffffff",
			theme->window[SSD_INACTIVE].button_colors[type]);
		LABWC_src_theme_c_parse_hexstr("#ffffff",
			theme->window[SSD_ACTIVE].button_colors[type]);
	}
	theme->window[SSD_ACTIVE].shadow_size = 60;
	theme->window[SSD_INACTIVE].shadow_size = 40;
	LABWC_src_theme_c_parse_hexstr("#000000", theme->window[SSD_ACTIVE].shadow_color);
	LABWC_src_theme_c_parse_hexstr("#000000", theme->window[SSD_INACTIVE].shadow_color);
	theme->menu_overlap_x = 0;
	theme->menu_overlap_y = 0;
	theme->menu_min_width = 20;
	theme->menu_max_width = 200;
	theme->menu_border_width = INT_MIN;
	theme->menu_border_color[0] = FLT_MIN;
	theme->menu_items_padding_x = 7;
	theme->menu_items_padding_y = 4;
	LABWC_src_theme_c_parse_hexstr("#030303f5", theme->menu_items_bg_color);
	LABWC_src_theme_c_parse_hexstr("#ffffff", theme->menu_items_text_color);
	LABWC_src_theme_c_parse_hexstr("#404040", theme->menu_items_active_bg_color);
	LABWC_src_theme_c_parse_hexstr("#ffffff", theme->menu_items_active_text_color);
	theme->menu_separator_line_thickness = 1;
	theme->menu_separator_padding_width = 6;
	theme->menu_separator_padding_height = 3;
	LABWC_src_theme_c_parse_hexstr("#40404066", theme->menu_separator_color);
	LABWC_src_theme_c_parse_hexstr("#0f1011", theme->menu_title_bg_color);
	theme->menu_title_text_justify = LABWC_src_theme_c_parse_justification("Center");
	LABWC_src_theme_c_parse_hexstr("#ffffff", theme->menu_title_text_color);
	theme->osd_workspace_switcher_boxes_width = 20;
	theme->osd_workspace_switcher_boxes_height = 20;
	theme->osd_workspace_switcher_boxes_border_width = 2;
	LABWC_src_theme_c_parse_hexstr("#030303", theme->osd_bg_color);
	theme->osd_border_width = INT_MIN;
	LABWC_src_theme_c_parse_hexstr("#404040", theme->osd_border_color);
	LABWC_src_theme_c_parse_hexstr("#ffffff", theme->osd_label_text_color);
	if (wlr_renderer_is_pixman(server.renderer)) {
		theme->snapping_overlay_region.bg_enabled = false;
		theme->snapping_overlay_edge.bg_enabled = false;
		theme->snapping_overlay_region.border_enabled = true;
		theme->snapping_overlay_edge.border_enabled = true;
	} else {
		theme->snapping_overlay_region.bg_enabled = true;
		theme->snapping_overlay_edge.bg_enabled = true;
		theme->snapping_overlay_region.border_enabled = false;
		theme->snapping_overlay_edge.border_enabled = false;
	}
	LABWC_src_theme_c_parse_hexstr("#8080b380", theme->snapping_overlay_region.bg_color);
	LABWC_src_theme_c_parse_hexstr("#8080b380", theme->snapping_overlay_edge.bg_color);
	theme->snapping_overlay_region.border_width = INT_MIN;
	theme->snapping_overlay_edge.border_width = INT_MIN;
	zero_array(theme->snapping_overlay_region.border_color);
	theme->snapping_overlay_region.border_color[0][0] = FLT_MIN;
	zero_array(theme->snapping_overlay_edge.border_color);
	theme->snapping_overlay_edge.border_color[0][0] = FLT_MIN;
}
static struct lab_data_buffer *
LABWC_src_theme_c_rounded_rect(struct LABWC_src_theme_c_rounded_corner_ctx *ctx)
{
	double w = ctx->box->width;
	double h = ctx->box->height;
	double r = ctx->radius;
	struct lab_data_buffer *buffer;
	buffer = buffer_create_cairo((uint32_t)w, (uint32_t)h, 1);
	cairo_surface_t *surf = buffer->surface;
	cairo_t *cairo = cairo_create(surf);
	cairo_set_operator(cairo, CAIRO_OPERATOR_CLEAR);
	cairo_paint(cairo);
	cairo_set_line_width(cairo, 0.0);
	cairo_new_sub_path(cairo);
	switch (ctx->corner) {
	case ROUNDED_CORNER_TOP_LEFT:
		cairo_arc(cairo, r, r, r, 180 * LABWC_src_theme_c_deg, 270 * LABWC_src_theme_c_deg);
		cairo_line_to(cairo, w, 0);
		cairo_line_to(cairo, w, h);
		cairo_line_to(cairo, 0, h);
		break;
	case ROUNDED_CORNER_TOP_RIGHT:
		cairo_arc(cairo, w - r, r, r, -90 * LABWC_src_theme_c_deg, 0 * LABWC_src_theme_c_deg);
		cairo_line_to(cairo, w, h);
		cairo_line_to(cairo, 0, h);
		cairo_line_to(cairo, 0, 0);
		break;
	}
	cairo_close_path(cairo);
	cairo_set_operator(cairo, CAIRO_OPERATOR_SOURCE);
	cairo_matrix_t matrix;
	cairo_matrix_init_translate(&matrix, 0, -ctx->line_width);
	cairo_pattern_set_matrix(ctx->fill_pattern, &matrix);
	cairo_set_source(cairo, ctx->fill_pattern);
	cairo_fill_preserve(cairo);
	cairo_stroke(cairo);
	cairo_matrix_init_identity(&matrix);
	cairo_pattern_set_matrix(ctx->fill_pattern, &matrix);
	cairo_set_line_cap(cairo, CAIRO_LINE_CAP_BUTT);
	set_cairo_color(cairo, ctx->border_color);
	cairo_set_line_width(cairo, ctx->line_width);
	double half_line_width = ctx->line_width / 2.0;
	switch (ctx->corner) {
	case ROUNDED_CORNER_TOP_LEFT:
		cairo_move_to(cairo, half_line_width, h);
		cairo_line_to(cairo, half_line_width, r);
		cairo_move_to(cairo, r, half_line_width);
		cairo_line_to(cairo, w, half_line_width);
		break;
	case ROUNDED_CORNER_TOP_RIGHT:
		cairo_move_to(cairo, 0, half_line_width);
		cairo_line_to(cairo, w - r, half_line_width);
		cairo_move_to(cairo, w - half_line_width, r);
		cairo_line_to(cairo, w - half_line_width, h);
		break;
	}
	cairo_stroke(cairo);
	if (!r) {
		goto out;
	}
	double line_width = MIN(ctx->line_width, r);
	cairo_set_line_width(cairo, line_width);
	half_line_width = line_width / 2.0;
	switch (ctx->corner) {
	case ROUNDED_CORNER_TOP_LEFT:
		cairo_move_to(cairo, half_line_width, r);
		cairo_arc(cairo, r, r, r - half_line_width, 180 * LABWC_src_theme_c_deg, 270 * LABWC_src_theme_c_deg);
		break;
	case ROUNDED_CORNER_TOP_RIGHT:
		cairo_move_to(cairo, w - r, half_line_width);
		cairo_arc(cairo, w - r, r, r - half_line_width, -90 * LABWC_src_theme_c_deg, 0 * LABWC_src_theme_c_deg);
		break;
	}
	cairo_stroke(cairo);
out:
	cairo_surface_flush(surf);
	cairo_destroy(cairo);
	return buffer;
}
static void
LABWC_src_theme_c_add_color_stop_rgba_premult(cairo_pattern_t *pattern, float offset,
		const float c[4])
{
	float alpha = c[3];
	if (alpha == 0.0f) {
		cairo_pattern_add_color_stop_rgba(pattern, offset, 0, 0, 0, 0);
	} else {
		cairo_pattern_add_color_stop_rgba(pattern, offset,
			c[0] / alpha, c[1] / alpha, c[2] / alpha, alpha);
	}
}
static cairo_pattern_t *
LABWC_src_theme_c_create_titlebar_pattern(const struct theme_background *bg, int height)
{
	cairo_pattern_t *pattern;
	switch (bg->gradient) {
	case LAB_GRADIENT_VERTICAL:
		pattern = cairo_pattern_create_linear(0, 0, 0, height);
		LABWC_src_theme_c_add_color_stop_rgba_premult(pattern, 0, bg->color);
		LABWC_src_theme_c_add_color_stop_rgba_premult(pattern, 1, bg->color_to);
		break;
	case LAB_GRADIENT_SPLITVERTICAL:
		pattern = cairo_pattern_create_linear(0, 0, 0, height);
		LABWC_src_theme_c_add_color_stop_rgba_premult(pattern, 0, bg->color_split_to);
		LABWC_src_theme_c_add_color_stop_rgba_premult(pattern, 0.5, bg->color);
		LABWC_src_theme_c_add_color_stop_rgba_premult(pattern, 0.5, bg->color_to);
		LABWC_src_theme_c_add_color_stop_rgba_premult(pattern, 1, bg->color_to_split_to);
		break;
	case LAB_GRADIENT_NONE:
	default:
		pattern = color_to_pattern(bg->color);
		break;
	}
	return pattern;
}
static struct lab_data_buffer *
LABWC_src_theme_c_create_titlebar_fill(cairo_pattern_t *pattern, int height)
{
	struct lab_data_buffer *fill = buffer_create_cairo(1,
		(uint32_t)height, 1);
	cairo_t *cairo = cairo_create(fill->surface);
	cairo_set_source(cairo, pattern);
	cairo_paint(cairo);
	cairo_surface_flush(fill->surface);
	cairo_destroy(cairo);
	return fill;
}
static void
LABWC_src_theme_c_create_backgrounds(struct theme *theme)
{
	enum ssd_active_state active;
	FOR_EACH_ACTIVE_STATE(active) {
		theme->window[active].titlebar_pattern = LABWC_src_theme_c_create_titlebar_pattern(
			&theme->window[active].title_bg,
			theme->titlebar_height);
		theme->window[active].titlebar_fill = LABWC_src_theme_c_create_titlebar_fill(
			theme->window[active].titlebar_pattern,
			theme->titlebar_height);
	}
}
static void
LABWC_src_theme_c_create_corners(struct theme *theme)
{
	int corner_width = ssd_get_corner_width();
	struct wlr_box box = {
		.x = 0,
		.y = 0,
		.width = corner_width + theme->border_width,
		.height = theme->titlebar_height + theme->border_width,
	};
	enum ssd_active_state active;
	FOR_EACH_ACTIVE_STATE(active) {
		struct LABWC_src_theme_c_rounded_corner_ctx ctx = {
			.box = &box,
			.radius = rc.corner_radius,
			.line_width = theme->border_width,
			.fill_pattern = theme->window[active].titlebar_pattern,
			.border_color = theme->window[active].border_color,
			.corner = ROUNDED_CORNER_TOP_LEFT,
		};
		theme->window[active].corner_top_left_normal = LABWC_src_theme_c_rounded_rect(&ctx);
		ctx.corner = ROUNDED_CORNER_TOP_RIGHT;
		theme->window[active].corner_top_right_normal = LABWC_src_theme_c_rounded_rect(&ctx);
	}
}
static void
LABWC_src_theme_c_shadow_edge_gradient(struct lab_data_buffer *buffer,
		int visible_size, int total_size, float start_color[4])
{
	if (!buffer) {
		return;
	}
	assert(buffer->format == DRM_FORMAT_ARGB8888);
	uint8_t *pixels = buffer->data;
	int inset = total_size - visible_size;
	double variance = 0.3 * 0.3;
	for (int x = 0; x < visible_size; x++) {
		double xn = (double)(x + inset) / (double)total_size;
		double alpha = exp(-(xn * xn) / variance);
		pixels[4 * x] = (uint8_t)(start_color[2] * alpha * 255);
		pixels[4 * x + 1] = (uint8_t)(start_color[1] * alpha * 255);
		pixels[4 * x + 2] = (uint8_t)(start_color[0] * alpha * 255);
		pixels[4 * x + 3] = (uint8_t)(start_color[3] * alpha * 255);
	}
}
static void
LABWC_src_theme_c_shadow_corner_gradient(struct lab_data_buffer *buffer, int visible_size,
	int total_size, float start_color[4])
{
	if (!buffer) {
		return;
	}
	assert(buffer->format == DRM_FORMAT_ARGB8888);
	uint8_t *pixels = buffer->data;
	double variance = 0.3 * 0.3;
	int inset = total_size - visible_size;
	for (int y = 0; y < total_size; y++) {
		uint8_t *pixel_row = &pixels[(size_t)y * buffer->stride];
		for (int x = 0; x < total_size; x++) {
			double x_norm = (double)(x) / (double)total_size;
			double y_norm = (double)(y) / (double)total_size;
			double gauss_x = exp(-(x_norm * x_norm) / variance);
			double gauss_y = exp(-(y_norm * y_norm) / variance);
			double alpha = gauss_x * gauss_y;
			if (x < inset && y < inset) {
				alpha = 0.0;
			}
			pixel_row[4 * x] = (uint8_t)(start_color[2] * alpha * 255);
			pixel_row[4 * x + 1] = (uint8_t)(start_color[1] * alpha * 255);
			pixel_row[4 * x + 2] = (uint8_t)(start_color[0] * alpha * 255);
			pixel_row[4 * x + 3] = (uint8_t)(start_color[3] * alpha * 255);
		}
	}
}
static void
LABWC_src_theme_c_create_shadow(struct theme *theme, enum ssd_active_state active)
{
	int visible_size = theme->window[active].shadow_size;
	int inset = (int)((double)visible_size * SSD_SHADOW_INSET);
	int total_size = visible_size + inset;
	if (visible_size > 0) {
		theme->window[active].shadow_edge = buffer_create_cairo(
			(uint32_t)visible_size, 1, 1.0);
		theme->window[active].shadow_corner_top = buffer_create_cairo(
			(uint32_t)total_size, (uint32_t)total_size, 1.0);
		theme->window[active].shadow_corner_bottom = buffer_create_cairo(
			(uint32_t)total_size, (uint32_t)total_size, 1.0);
		if (!theme->window[active].shadow_corner_top
				|| !theme->window[active].shadow_corner_bottom
				|| !theme->window[active].shadow_edge) {
			return;
		}
	}
	LABWC_src_theme_c_shadow_edge_gradient(theme->window[active].shadow_edge, visible_size,
		total_size, theme->window[active].shadow_color);
	LABWC_src_theme_c_shadow_corner_gradient(theme->window[active].shadow_corner_top,
		visible_size, total_size, theme->window[active].shadow_color);
	LABWC_src_theme_c_shadow_corner_gradient(theme->window[active].shadow_corner_bottom,
		visible_size, total_size, theme->window[active].shadow_color);
}
static void
LABWC_src_theme_c_create_shadows(struct theme *theme)
{
	LABWC_src_theme_c_create_shadow(theme, SSD_INACTIVE);
	LABWC_src_theme_c_create_shadow(theme, SSD_ACTIVE);
}
static void
LABWC_src_theme_c_copy_color_scaled(float dest[4], const float src[4], float scale)
{
	dest[0] = fminf(src[0] * scale, src[3]);
	dest[1] = fminf(src[1] * scale, src[3]);
	dest[2] = fminf(src[2] * scale, src[3]);
	dest[3] = src[3];
}
static void
LABWC_src_theme_c_fill_background_colors(struct theme_background *bg)
{
	if (bg->color_split_to[0] == FLT_MIN) {
		LABWC_src_theme_c_copy_color_scaled(bg->color_split_to, bg->color, 1.25f);
	}
	if (bg->color_to[0] == FLT_MIN) {
		memcpy(bg->color_to, bg->color, sizeof(bg->color_to));
	}
	if (bg->color_to_split_to[0] == FLT_MIN) {
		LABWC_src_theme_c_copy_color_scaled(bg->color_to_split_to, bg->color_to, 1.0625f);
	}
}
static void __attribute__((unused))
LABWC_src_theme_c_fill_colors_with_osd_theme(struct theme *theme, float colors[3][4])
{
	memcpy(colors[0], theme->osd_bg_color, sizeof(colors[0]));
	memcpy(colors[1], theme->osd_label_text_color, sizeof(colors[1]));
	memcpy(colors[2], theme->osd_bg_color, sizeof(colors[2]));
}
static int
LABWC_src_theme_c_get_titlebar_height(struct theme *theme)
{
	int h = MAX(font_height(&rc.font_activewindow),
		font_height(&rc.font_inactivewindow));
	if (h < theme->window_button_height) {
		h = theme->window_button_height;
	}
	h += 2 * theme->window_titlebar_padding_height;
	return h;
}
static void __attribute__((unused))
LABWC_src_theme_c_blend_color_with_bg(float *dst, float *fg, float fg_a, float *bg)
{
	if (fg[3] <= 0.0f) {
		memset(dst, 0, sizeof(float) * 4);
		return;
	}
	float new_fg[4] = {
		fg[0] / fg[3] * fg_a,
		fg[1] / fg[3] * fg_a,
		fg[2] / fg[3] * fg_a,
		fg_a,
	};
	dst[0] = new_fg[0] + bg[0] * (1.0f - new_fg[3]);
	dst[1] = new_fg[1] + bg[1] * (1.0f - new_fg[3]);
	dst[2] = new_fg[2] + bg[2] * (1.0f - new_fg[3]);
	dst[3] = new_fg[3] + bg[3] * (1.0f - new_fg[3]);
}
static void
LABWC_src_theme_c_post_processing(struct theme *theme)
{
	theme->titlebar_height = LABWC_src_theme_c_get_titlebar_height(theme);
	LABWC_src_theme_c_fill_background_colors(&theme->window[SSD_INACTIVE].title_bg);
	LABWC_src_theme_c_fill_background_colors(&theme->window[SSD_ACTIVE].title_bg);
	theme->menu_item_height = font_height(&rc.font_menuitem)
		+ 2 * theme->menu_items_padding_y;
	theme->menu_header_height = font_height(&rc.font_menuheader)
		+ 2 * theme->menu_items_padding_y;
	if (rc.corner_radius >= theme->titlebar_height) {
		rc.corner_radius = theme->titlebar_height - 1;
	}
	if (rc.resize_corner_range < 0) {
		rc.resize_corner_range = theme->titlebar_height / 2;
	}
	int min_button_hover_radius =
		MIN(theme->window_button_width, theme->window_button_height) / 2;
	if (theme->window_button_hover_bg_corner_radius > min_button_hover_radius) {
		theme->window_button_hover_bg_corner_radius = min_button_hover_radius;
	}
	if (theme->menu_max_width < theme->menu_min_width) {
		theme->menu_max_width = theme->menu_min_width;
	}
	if (theme->menu_border_width == INT_MIN) {
		theme->menu_border_width = theme->border_width;
	}
	if (theme->menu_border_color[0] == FLT_MIN) {
		memcpy(theme->menu_border_color,
			theme->window[SSD_ACTIVE].border_color,
			sizeof(theme->menu_border_color));
	}
	if (theme->osd_bg_color[0] == FLT_MIN) {
		memcpy(theme->osd_bg_color,
			theme->window[SSD_ACTIVE].title_bg.color,
			sizeof(theme->osd_bg_color));
	}
	if (theme->osd_border_width == INT_MIN) {
		theme->osd_border_width = theme->border_width;
	}
	if (theme->osd_label_text_color[0] == FLT_MIN) {
		memcpy(theme->osd_label_text_color,
			theme->window[SSD_ACTIVE].label_text_color,
			sizeof(theme->osd_label_text_color));
	}
	if (theme->osd_border_color[0] == FLT_MIN) {
		memcpy(theme->osd_border_color, theme->osd_label_text_color,
			sizeof(theme->osd_border_color));
	}
	if (theme->osd_workspace_switcher_boxes_width == 0) {
		theme->osd_workspace_switcher_boxes_height = 0;
	}
	if (theme->osd_workspace_switcher_boxes_height == 0) {
		theme->osd_workspace_switcher_boxes_width = 0;
	}
	(void)theme;
	if (theme->snapping_overlay_region.border_width == INT_MIN) {
		theme->snapping_overlay_region.border_width =
			theme->osd_border_width;
	}
	if (theme->snapping_overlay_edge.border_width == INT_MIN) {
		theme->snapping_overlay_edge.border_width =
			theme->osd_border_width;
	}
	if (theme->snapping_overlay_region.border_color[0][0] == FLT_MIN) {
		memcpy(theme->snapping_overlay_region.border_color,
			theme->osd_border_color,
			sizeof(theme->snapping_overlay_region.border_color));
	}
	if (theme->snapping_overlay_edge.border_color[0][0] == FLT_MIN) {
		memcpy(theme->snapping_overlay_edge.border_color,
			theme->osd_border_color,
			sizeof(theme->snapping_overlay_edge.border_color));
	}
}
static void jtlabwc_control_apply_theme_overrides(struct theme *theme);
void
theme_init(struct theme *theme)
{
	LABWC_src_theme_c_theme_builtin(theme);
	LABWC_src_theme_c_post_processing(theme);
	jtlabwc_control_apply_theme_overrides(theme);
	LABWC_src_theme_c_create_backgrounds(theme);
	LABWC_src_theme_c_create_corners(theme);
	LABWC_src_theme_c_load_buttons(theme);
	LABWC_src_theme_c_create_shadows(theme);
}
static void LABWC_src_theme_c_destroy_img(struct lab_img **img)
{
	lab_img_destroy(*img);
	*img = NULL;
}
void
theme_finish(struct theme *theme)
{
	for (enum lab_node_type type = LAB_NODE_BUTTON_FIRST;
			type <= LAB_NODE_BUTTON_LAST; type++) {
		for (uint8_t state_set = LAB_BS_DEFAULT;
				state_set <= LAB_BS_ALL; state_set++) {
			LABWC_src_theme_c_destroy_img(&theme->window[SSD_INACTIVE]
				.button_imgs[type][state_set]);
			LABWC_src_theme_c_destroy_img(&theme->window[SSD_ACTIVE]
				.button_imgs[type][state_set]);
		}
	}
	enum ssd_active_state active;
	FOR_EACH_ACTIVE_STATE(active) {
		zfree_pattern(theme->window[active].titlebar_pattern);
		LABWC_src_theme_c_zdrop(&theme->window[active].titlebar_fill);
		LABWC_src_theme_c_zdrop(&theme->window[active].corner_top_left_normal);
		LABWC_src_theme_c_zdrop(&theme->window[active].corner_top_right_normal);
		LABWC_src_theme_c_zdrop(&theme->window[active].shadow_corner_top);
		LABWC_src_theme_c_zdrop(&theme->window[active].shadow_corner_bottom);
		LABWC_src_theme_c_zdrop(&theme->window[active].shadow_edge);
	}
}
#undef zero_array
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>
#define JTLABWC_CONTROL_SOCKET "jtlabwc-ctl.sock"
#define JTLABWC_CONTROL_BACKLOG 8
#define JTLABWC_CONTROL_LINE_MAX 512
struct jtlabwc_control_color {
	bool set;
	bool has_alpha;
	float r, g, b, a;
};
struct jtlabwc_control_client {
	struct wl_list link;
	int fd;
	struct wl_event_source *source;
	size_t len;
	char buf[JTLABWC_CONTROL_LINE_MAX];
};
static struct jtlabwc_control_color jtlabwc_close_color;
static struct jtlabwc_control_color jtlabwc_snap_color;
static int jtlabwc_control_fd = -1;
static struct wl_event_source *jtlabwc_control_source;
static char jtlabwc_control_path[sizeof(((struct sockaddr_un *)0)->sun_path)];
static struct wl_list jtlabwc_control_clients;
static int
jtlabwc_control_hex_nibble(char c)
{
	if (c >= '0' && c <= '9') {
		return c - '0';
	}
	if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	}
	if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	}
	return -1;
}
static bool
jtlabwc_control_parse_color(const char *text, struct jtlabwc_control_color *color)
{
	if (!text) {
		return false;
	}
	if (*text == '#') {
		text++;
	} else if (text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) {
		text += 2;
	}
	size_t len = strlen(text);
	if (len != 6 && len != 8) {
		return false;
	}
	unsigned value = 0;
	for (size_t i = 0; i < len; i++) {
		int nibble = jtlabwc_control_hex_nibble(text[i]);
		if (nibble < 0) {
			return false;
		}
		value = (value << 4) | (unsigned)nibble;
	}
	color->set = true;
	if (len == 6) {
		color->r = (float)((value >> 16) & 0xffu) / 255.0f;
		color->g = (float)((value >> 8) & 0xffu) / 255.0f;
		color->b = (float)(value & 0xffu) / 255.0f;
		color->a = 1.0f;
		color->has_alpha = false;
	} else {
		color->r = (float)((value >> 24) & 0xffu) / 255.0f;
		color->g = (float)((value >> 16) & 0xffu) / 255.0f;
		color->b = (float)((value >> 8) & 0xffu) / 255.0f;
		color->a = (float)(value & 0xffu) / 255.0f;
		color->has_alpha = true;
	}
	return true;
}
static void
jtlabwc_control_apply_color(float out[4], const struct jtlabwc_control_color *color)
{
	if (!color->set) {
		return;
	}
	float alpha = color->has_alpha ? color->a : out[3];
	out[0] = color->r * alpha;
	out[1] = color->g * alpha;
	out[2] = color->b * alpha;
	out[3] = alpha;
}
static void
jtlabwc_control_apply_theme_overrides(struct theme *theme)
{
	if (jtlabwc_close_color.set) {
		enum ssd_active_state active;
		FOR_EACH_ACTIVE_STATE(active) {
			jtlabwc_control_apply_color(
				theme->window[active].button_colors[LAB_NODE_BUTTON_CLOSE],
				&jtlabwc_close_color);
		}
	}
	if (jtlabwc_snap_color.set) {
		jtlabwc_control_apply_color(
			theme->snapping_overlay_region.bg_color, &jtlabwc_snap_color);
		jtlabwc_control_apply_color(
			theme->snapping_overlay_edge.bg_color, &jtlabwc_snap_color);
		for (int i = 0; i < 3; i++) {
			jtlabwc_control_apply_color(
				theme->snapping_overlay_region.border_color[i],
				&jtlabwc_snap_color);
			jtlabwc_control_apply_color(
				theme->snapping_overlay_edge.border_color[i],
				&jtlabwc_snap_color);
		}
	}
}
static void
jtlabwc_control_refresh(void)
{
	if (!rc.theme) {
		return;
	}
	scaled_buffer_invalidate_sharing();
	theme_finish(rc.theme);
	theme_init(rc.theme);
	struct view *view;
	wl_list_for_each(view, &server.views, link) {
		view_reload_ssd(view);
	}
	if (server.seat.overlay.rect || server.seat.overlay.timer) {
		overlay_finish(&server.seat);
		overlay_update(&server.seat);
	}
}
static void
jtlabwc_control_process_line(char *line)
{
	while (*line == ' ' || *line == '\t' || *line == '\r') {
		line++;
	}
	size_t len = strlen(line);
	while (len > 0) {
		char c = line[len - 1];
		if (c != ' ' && c != '\t' && c != '\r') {
			break;
		}
		line[--len] = '\0';
	}
	if (!*line) {
		return;
	}
	char *saveptr = NULL;
	char *command = strtok_r(line, " \t\r", &saveptr);
	if (!command) {
		return;
	}
	char *argument = strtok_r(NULL, " \t\r", &saveptr);
	bool set_close = false;
	bool set_snap = false;
	if (!strcmp(command, "accent")) {
		set_close = true;
		set_snap = true;
	} else if (!strcmp(command, "close")) {
		set_close = true;
	} else if (!strcmp(command, "snap")) {
		set_snap = true;
	} else if (!strcmp(command, "reset")) {
		jtlabwc_close_color = (struct jtlabwc_control_color){0};
		jtlabwc_snap_color = (struct jtlabwc_control_color){0};
		jtlabwc_control_refresh();
		return;
	} else {
		argument = command;
		set_close = true;
		set_snap = true;
	}
	if (!argument) {
		return;
	}
	struct jtlabwc_control_color color = {0};
	if (!jtlabwc_control_parse_color(argument, &color)) {
		return;
	}
	if (set_close) {
		jtlabwc_close_color = color;
	}
	if (set_snap) {
		jtlabwc_snap_color = color;
	}
	jtlabwc_control_refresh();
}
static void
jtlabwc_control_process_buffer(struct jtlabwc_control_client *client)
{
	size_t start = 0;
	for (size_t i = 0; i < client->len; i++) {
		if (client->buf[i] != '\n') {
			continue;
		}
		client->buf[i] = '\0';
		jtlabwc_control_process_line(&client->buf[start]);
		start = i + 1;
	}
	if (start > 0) {
		client->len -= start;
		memmove(client->buf, client->buf + start, client->len);
		client->buf[client->len] = '\0';
	}
}
static void
jtlabwc_control_client_destroy(struct jtlabwc_control_client *client)
{
	wl_list_remove(&client->link);
	if (client->source) {
		wl_event_source_remove(client->source);
		client->source = NULL;
	}
	if (client->fd >= 0) {
		close(client->fd);
		client->fd = -1;
	}
	free(client);
}
static int
jtlabwc_control_handle_client(int fd, uint32_t mask, void *data)
{
	struct jtlabwc_control_client *client = data;
	if (mask & WL_EVENT_READABLE) {
		while (true) {
			if (client->len >= sizeof(client->buf) - 1) {
				client->len = 0;
				client->buf[0] = '\0';
			}
			ssize_t n = read(fd, client->buf + client->len,
				sizeof(client->buf) - client->len - 1);
			if (n < 0) {
				if (errno == EINTR) {
					continue;
				}
				if (errno == EAGAIN || errno == EWOULDBLOCK) {
					break;
				}
				jtlabwc_control_client_destroy(client);
				return 0;
			}
			if (n == 0) {
				jtlabwc_control_client_destroy(client);
				return 0;
			}
			client->len += (size_t)n;
			client->buf[client->len] = '\0';
			jtlabwc_control_process_buffer(client);
		}
	}
	if (mask & (WL_EVENT_HANGUP | WL_EVENT_ERROR)) {
		jtlabwc_control_client_destroy(client);
	}
	return 0;
}
static int
jtlabwc_control_handle_accept(int fd, uint32_t mask, void *data)
{
	(void)data;
	if (!(mask & WL_EVENT_READABLE)) {
		return 0;
	}
	while (true) {
		int client_fd = accept4(fd, NULL, NULL, SOCK_CLOEXEC | SOCK_NONBLOCK);
		if (client_fd < 0) {
			if (errno == EINTR) {
				continue;
			}
			break;
		}
		struct jtlabwc_control_client *client = znew(*client);
		client->fd = client_fd;
		client->source = wl_event_loop_add_fd(server.wl_event_loop,
			client_fd, WL_EVENT_READABLE,
			jtlabwc_control_handle_client, client);
		if (!client->source) {
			close(client_fd);
			free(client);
			continue;
		}
		wl_list_insert(&jtlabwc_control_clients, &client->link);
	}
	return 0;
}
static bool
jtlabwc_control_open_socket(void)
{
	const char *runtime = getenv("XDG_RUNTIME_DIR");
	if (!runtime || !*runtime) {
		return false;
	}
	int n = snprintf(jtlabwc_control_path, sizeof(jtlabwc_control_path),
		"%s/%s", runtime, JTLABWC_CONTROL_SOCKET);
	if (n < 0 || (size_t)n >= sizeof(jtlabwc_control_path)) {
		jtlabwc_control_path[0] = '\0';
		return false;
	}
	int fd = socket(AF_UNIX,
		SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);
	if (fd < 0) {
		jtlabwc_control_path[0] = '\0';
		return false;
	}
	unlink(jtlabwc_control_path);
	struct sockaddr_un addr = {0};
	addr.sun_family = AF_UNIX;
	memcpy(addr.sun_path, jtlabwc_control_path, (size_t)n + 1);
	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		close(fd);
		jtlabwc_control_path[0] = '\0';
		return false;
	}
	chmod(jtlabwc_control_path, S_IRUSR | S_IWUSR);
	if (listen(fd, JTLABWC_CONTROL_BACKLOG) < 0) {
		close(fd);
		unlink(jtlabwc_control_path);
		jtlabwc_control_path[0] = '\0';
		return false;
	}
	jtlabwc_control_fd = fd;
	return true;
}
static void
jtlabwc_control_init(void)
{
	wl_list_init(&jtlabwc_control_clients);
	if (!jtlabwc_control_open_socket()) {
		return;
	}
	jtlabwc_control_source = wl_event_loop_add_fd(server.wl_event_loop,
		jtlabwc_control_fd, WL_EVENT_READABLE,
		jtlabwc_control_handle_accept, NULL);
	if (!jtlabwc_control_source) {
		close(jtlabwc_control_fd);
		jtlabwc_control_fd = -1;
		unlink(jtlabwc_control_path);
		jtlabwc_control_path[0] = '\0';
	}
}
static void
jtlabwc_control_finish(void)
{
	struct jtlabwc_control_client *client, *tmp;
	wl_list_for_each_safe(client, tmp, &jtlabwc_control_clients, link) {
		jtlabwc_control_client_destroy(client);
	}
	if (jtlabwc_control_source) {
		wl_event_source_remove(jtlabwc_control_source);
		jtlabwc_control_source = NULL;
	}
	if (jtlabwc_control_fd >= 0) {
		close(jtlabwc_control_fd);
		jtlabwc_control_fd = -1;
	}
	if (jtlabwc_control_path[0]) {
		unlink(jtlabwc_control_path);
		jtlabwc_control_path[0] = '\0';
	}
}
void
view_impl_map(struct view *view)
{
	view_update_visibility(view);
	if (!view->minimized) {
		desktop_focus_view(view,   true);
	}
	if (!view->been_mapped) {
		window_rules_apply(view, LAB_WINDOW_RULE_EVENT_ON_FIRST_MAP);
		if (view->app_id && !strcasecmp(view->app_id, "Waydroid")) {
			view_set_fullscreen(view, true);
		}
	}
	if (!view->foreign_toplevel && view_is_focusable(view)
			&& window_rules_get_property(view, "skipTaskbar")
				!= LAB_PROP_TRUE) {
		view->foreign_toplevel = foreign_toplevel_create(view);
		struct view *parent = view->impl->get_parent(view);
		if (parent && parent->foreign_toplevel) {
			foreign_toplevel_set_parent(view->foreign_toplevel,
				parent->foreign_toplevel);
		}
	}
}
void
view_impl_unmap(struct view *view)
{
	view_update_visibility(view);
	if (view == server.active_view || !server.active_view) {
		desktop_focus_topmost_view();
	}
	if (view->foreign_toplevel) {
		foreign_toplevel_destroy(view->foreign_toplevel);
		view->foreign_toplevel = NULL;
	}
}
static bool
LABWC_src_view_impl_common_c_resizing_edge(struct view *view, enum lab_edge edge)
{
	return server.input_mode == LAB_INPUT_STATE_RESIZE
		&& server.grabbed_view == view
		&& (server.resize_edges & edge);
}
void
view_impl_apply_geometry(struct view *view, int w, int h)
{
	struct wlr_box *current = &view->current;
	struct wlr_box *pending = &view->pending;
	bool resizing_left_edge = LABWC_src_view_impl_common_c_resizing_edge(view, LAB_EDGE_LEFT);
	if (resizing_left_edge || (current->x != pending->x
			&& current->x + current->width ==
			pending->x + pending->width)) {
		current->x = pending->x + pending->width - w;
	} else {
		current->x = pending->x;
	}
	bool resizing_top_edge = LABWC_src_view_impl_common_c_resizing_edge(view, LAB_EDGE_TOP);
	if (resizing_top_edge || (current->y != pending->y
			&& current->y + current->height ==
			pending->y + pending->height)) {
		current->y = pending->y + pending->height - h;
	} else {
		current->y = pending->y;
	}
	current->width = w;
	current->height = h;
}
#include <assert.h>
#include <strings.h>
#include "wlr/util/log.h"
#if HAVE_XWAYLAND
#endif
struct view *
view_from_wlr_surface(struct wlr_surface *surface)
{
	assert(surface);
	struct wlr_xdg_surface *xdg_surface =
		wlr_xdg_surface_try_from_wlr_surface(surface);
	if (xdg_surface) {
		return xdg_surface->data;
	}
#if HAVE_XWAYLAND
	struct wlr_xwayland_surface *xsurface =
		wlr_xwayland_surface_try_from_wlr_surface(surface);
	if (xsurface) {
		return xsurface->data;
	}
#endif
	return NULL;
}
static const struct wlr_security_context_v1_state *
LABWC_src_view_c_security_context_from_view(struct view *view)
{
	if (view && view->surface && view->surface->resource) {
		struct wl_client *client = wl_resource_get_client(view->surface->resource);
		return wlr_security_context_manager_v1_lookup_client(
			server.security_context_manager_v1, client);
	}
	return NULL;
}
struct view_query *
view_query_create(void)
{
	struct view_query *query = znew(*query);
	query->window_type = LAB_WINDOW_TYPE_INVALID;
	query->maximized = VIEW_AXIS_INVALID;
	query->decoration = LAB_SSD_MODE_INVALID;
	return query;
}
void
view_query_free(struct view_query *query)
{
	wl_list_remove(&query->link);
	zfree(query->identifier);
	zfree(query->title);
	zfree(query->sandbox_engine);
	zfree(query->sandbox_app_id);
	zfree(query->tiled_region);
	zfree(query->desktop);
	zfree(query->monitor);
	zfree(query);
}
static bool
LABWC_src_view_c_query_tristate_match(enum lab_tristate desired, bool actual)
{
	switch (desired) {
	case LAB_STATE_ENABLED:
		return actual;
	case LAB_STATE_DISABLED:
		return !actual;
	default:
		return true;
	}
}
static bool
LABWC_src_view_c_query_str_match(const char *condition, const char *value)
{
	if (!condition) {
		return true;
	}
	return value && match_glob(condition, value);
}
static bool
LABWC_src_view_c_view_contains_window_type(struct view *view, enum lab_window_type window_type)
{
	assert(view);
	if (view->impl->contains_window_type) {
		return view->impl->contains_window_type(view, window_type);
	}
	return false;
}
bool
view_matches_query(struct view *view, struct view_query *query)
{
	if (!LABWC_src_view_c_query_str_match(query->identifier, view->app_id)) {
		return false;
	}
	if (!LABWC_src_view_c_query_str_match(query->title, view->title)) {
		return false;
	}
	if (query->window_type != LAB_WINDOW_TYPE_INVALID
			&& !LABWC_src_view_c_view_contains_window_type(view, query->window_type)) {
		return false;
	}
	if (query->sandbox_engine || query->sandbox_app_id) {
		const struct wlr_security_context_v1_state *ctx =
			LABWC_src_view_c_security_context_from_view(view);
		if (!ctx) {
			return false;
		}
		if (!LABWC_src_view_c_query_str_match(query->sandbox_engine, ctx->sandbox_engine)) {
			return false;
		}
		if (!LABWC_src_view_c_query_str_match(query->sandbox_app_id, ctx->app_id)) {
			return false;
		}
	}
	if (!LABWC_src_view_c_query_tristate_match(query->shaded, view->shaded)) {
		return false;
	}
	if (query->maximized != VIEW_AXIS_INVALID && view->maximized != query->maximized) {
		return false;
	}
	if (!LABWC_src_view_c_query_tristate_match(query->iconified, view->minimized)) {
		return false;
	}
	if (!LABWC_src_view_c_query_tristate_match(query->focused, server.active_view == view)) {
		return false;
	}
	if (!LABWC_src_view_c_query_tristate_match(query->omnipresent, view->visible_on_all_workspaces)) {
		return false;
	}
	if (query->tiled == LAB_EDGE_ANY) {
		if (!view->tiled) {
			return false;
		}
	} else if (query->tiled != LAB_EDGE_NONE) {
		if (query->tiled != view->tiled) {
			return false;
		}
	}
	const char *tiled_region =
		view->tiled_region ? view->tiled_region->name : NULL;
	if (!LABWC_src_view_c_query_str_match(query->tiled_region, tiled_region)) {
		return false;
	}
	if (query->desktop) {
		const char *view_workspace = view->workspace->name;
		struct workspace *current = server.workspaces.current;
		if (!strcasecmp(query->desktop, "other")) {
			if (!strcasecmp(view_workspace, current->name)) {
				return false;
			}
		} else {
			struct workspace *target =
				workspaces_find(current, query->desktop,   false);
			if (!target || strcasecmp(view_workspace, target->name)) {
				return false;
			}
		}
	}
	if (query->decoration != LAB_SSD_MODE_INVALID
			&& query->decoration != view->ssd_mode) {
		return false;
	}
	if (query->monitor) {
		struct output *current = output_nearest_to_cursor();
		if (!strcasecmp(query->monitor, "current")) {
			if (current != view->output) {
				return false;
			}
		} else if (!strcasecmp(query->monitor, "left")) {
			if (output_get_adjacent(current, LAB_EDGE_LEFT, false)
					!= view->output) {
				return false;
			}
		} else if (!strcasecmp(query->monitor, "right")) {
			if (output_get_adjacent(current, LAB_EDGE_RIGHT, false)
					!= view->output) {
				return false;
			}
		} else {
			if (output_from_name(query->monitor)
					!= view->output) {
				return false;
			}
		}
	}
	return true;
}
static struct view *
LABWC_src_view_c_view_get_root(struct view *view)
{
	assert(view);
	if (view->impl->get_root) {
		return view->impl->get_root(view);
	}
	return view;
}
bool
view_matches_criteria(struct view *view, enum lab_view_criteria criteria)
{
	if (!view_is_focusable(view)) {
		return false;
	}
	if (criteria & LAB_VIEW_CRITERIA_CURRENT_WORKSPACE) {
		if (view->workspace != server.workspaces.current) {
			return false;
		}
	}
	if (criteria & LAB_VIEW_CRITERIA_FULLSCREEN) {
		if (!view->fullscreen) {
			return false;
		}
	}
	if (criteria & LAB_VIEW_CRITERIA_ALWAYS_ON_TOP) {
		if (view->layer != VIEW_LAYER_ALWAYS_ON_TOP) {
			return false;
		}
	}
	if (criteria & LAB_VIEW_CRITERIA_NO_DIALOG) {
		if (view_is_modal_dialog(view)) {
			return false;
		}
	}
	if (criteria & LAB_VIEW_CRITERIA_NO_ALWAYS_ON_TOP) {
		if (view->layer == VIEW_LAYER_ALWAYS_ON_TOP) {
			return false;
		}
	}
	if (criteria & LAB_VIEW_CRITERIA_NO_OMNIPRESENT) {
		if (view->visible_on_all_workspaces) {
			return false;
		}
	}
	return true;
}
struct view *
view_next(struct wl_list *head, struct view *view, enum lab_view_criteria criteria)
{
	assert(head);
	struct wl_list *elm = view ? &view->link : head;
	for (elm = elm->next; elm != head; elm = elm->next) {
		view = wl_container_of(elm, view, link);
		if (view_matches_criteria(view, criteria)) {
			return view;
		}
	}
	return NULL;
}
struct view *
view_prev(struct wl_list *head, struct view *view, enum lab_view_criteria criteria)
{
	assert(head);
	struct wl_list *elm = view ? &view->link : head;
	for (elm = elm->prev; elm != head; elm = elm->prev) {
		view = wl_container_of(elm, view, link);
		if (view_matches_criteria(view, criteria)) {
			return view;
		}
	}
	return NULL;
}
void
view_array_append(struct wl_array *views,
		enum lab_view_criteria criteria)
{
	struct view *view;
	for_each_view(view, &server.views, criteria) {
		struct view **entry = wl_array_add(views, sizeof(*entry));
		if (!entry) {
			continue;
		}
		*entry = view;
	}
}
enum view_wants_focus
view_wants_focus(struct view *view)
{
	assert(view);
	if (view->impl->wants_focus) {
		return view->impl->wants_focus(view);
	}
	return VIEW_WANTS_FOCUS_ALWAYS;
}
bool
view_is_focusable(struct view *view)
{
	assert(view);
	if (!view->surface) {
		return false;
	}
	switch (view_wants_focus(view)) {
	case VIEW_WANTS_FOCUS_ALWAYS:
	case VIEW_WANTS_FOCUS_LIKELY:
		return view->mapped;
	default:
		return false;
	}
}
void
view_offer_focus(struct view *view)
{
	assert(view);
	if (view->impl->offer_focus) {
		view->impl->offer_focus(view);
	}
}
struct wlr_box
view_get_edge_snap_box(struct view *view, struct output *output,
		enum lab_edge edge)
{
	struct wlr_box usable = output_usable_area_in_layout_coords(output);
	int x1 = rc.gap;
	int y1 = rc.gap;
	int x2 = usable.width - rc.gap;
	int y2 = usable.height - rc.gap;
	if (edge & LAB_EDGE_RIGHT) {
		x1 = (usable.width + rc.gap) / 2;
	}
	if (edge & LAB_EDGE_LEFT) {
		x2 = (usable.width - rc.gap) / 2;
	}
	if (edge & LAB_EDGE_BOTTOM) {
		y1 = (usable.height + rc.gap) / 2;
	}
	if (edge & LAB_EDGE_TOP) {
		y2 = (usable.height - rc.gap) / 2;
	}
	struct wlr_box dst = {
		.x = x1 + usable.x,
		.y = y1 + usable.y,
		.width = x2 - x1,
		.height = y2 - y1,
	};
	if (view) {
		struct border margin = ssd_get_margin(view->ssd);
		dst.x += margin.left;
		dst.y += margin.top;
		dst.width -= margin.left + margin.right;
		dst.height -= margin.top + margin.bottom;
	}
	return dst;
}
static bool
LABWC_src_view_c_view_discover_output(struct view *view, struct wlr_box *geometry)
{
	assert(view);
	if (!geometry) {
		geometry = &view->current;
	}
	struct output *output =
		output_nearest_to(geometry->x + geometry->width / 2,
			geometry->y + geometry->height / 2);
	if (output && output != view->output) {
		view->output = output;
		return true;
	}
	return false;
}
void
view_set_activated(struct view *view, bool activated)
{
	assert(view);
	ssd_set_active(view->ssd, activated);
	if (view->impl->set_activated) {
		view->impl->set_activated(view, activated);
	}
	wl_signal_emit_mutable(&view->events.activated, &activated);
	if (rc.kb_layout_per_window) {
		if (!activated) {
			view->keyboard_layout =
				server.seat.keyboard_group->keyboard.modifiers.group;
		} else {
			keyboard_update_layout(&server.seat, view->keyboard_layout);
		}
	}
}
void
view_set_output(struct view *view, struct output *output)
{
	assert(view);
	if (!output_is_usable(output)) {
		return;
	}
	view->output = output;
	if (view->fullscreen) {
		desktop_update_top_layer_visibility();
	}
}
void
view_close(struct view *view)
{
	assert(view);
	if (view->impl->close) {
		view->impl->close(view);
	}
}
static void
LABWC_src_view_c_view_update_outputs(struct view *view)
{
	struct output *output;
	struct wlr_output_layout *layout = server.output_layout;
	uint64_t new_outputs = 0;
	wl_list_for_each(output, &server.outputs, link) {
		if (output_is_usable(output) && wlr_output_layout_intersects(
				layout, output->wlr_output, &view->current)) {
			new_outputs |= output->id_bit;
		}
	}
	if (new_outputs != view->outputs) {
		view->outputs = new_outputs;
		wl_signal_emit_mutable(&view->events.new_outputs, NULL);
		desktop_update_top_layer_visibility();
	}
}
bool
view_on_output(struct view *view, struct output *output)
{
	assert(view);
	assert(output);
	return (view->outputs & output->id_bit) != 0;
}
void
view_move(struct view *view, int x, int y)
{
	assert(view);
	view_move_resize(view, (struct wlr_box){
		.x = x, .y = y,
		.width = view->pending.width,
		.height = view->pending.height
	});
}
void
view_moved(struct view *view)
{
	assert(view);
	wlr_scene_node_set_position(&view->scene_tree->node,
		view->current.x, view->current.y);
	if (view_is_floating(view)) {
		LABWC_src_view_c_view_discover_output(view, NULL);
	}
	LABWC_src_view_c_view_update_outputs(view);
	ssd_update_geometry(view->ssd);
	cursor_update_focus();
	if (rc.resize_indicator && server.grabbed_view == view) {
		resize_indicator_update(view);
	}
}
void
view_move_resize(struct view *view, struct wlr_box geo)
{
	assert(view);
	if (view->impl->configure) {
		view->impl->configure(view, geo);
	}
	if (!view->adjusting_for_layout_change) {
		view_save_last_placement(view);
	}
}
void
view_resize_relative(struct view *view, int left, int right, int top, int bottom)
{
	assert(view);
	if (view->fullscreen || view->maximized != VIEW_AXIS_NONE) {
		return;
	}
	view_set_shade(view, false);
	struct wlr_box newgeo = view->pending;
	newgeo.x -= left;
	newgeo.width += left + right;
	newgeo.y -= top;
	newgeo.height += top + bottom;
	view_move_resize(view, newgeo);
	view_set_untiled(view);
}
void
view_move_relative(struct view *view, int x, int y)
{
	assert(view);
	if (view->fullscreen) {
		return;
	}
	view_maximize(view, VIEW_AXIS_NONE);
	if (view_is_tiled(view)) {
		view_set_untiled(view);
		view_move_resize(view, view->natural_geometry);
	}
	view_move(view, view->pending.x + x, view->pending.y + y);
}
static bool
LABWC_src_view_c_view_compute_near_cursor_position(struct view *view, struct wlr_box *geom)
{
	assert(view);
	struct output *pending_output = output_nearest_to_cursor();
	struct wlr_box usable = output_usable_area_in_layout_coords(pending_output);
	usable.x += rc.gap;
	usable.y += rc.gap;
	usable.width -= 2 * rc.gap;
	usable.height -= 2 * rc.gap;
	if (wlr_box_empty(geom) || wlr_box_empty(&usable)) {
		return false;
	}
	struct border margin = ssd_thickness(view);
	struct seat *seat = &server.seat;
	int total_width = geom->width + margin.left + margin.right;
	int total_height = geom->height + margin.top + margin.bottom;
	int x = (int)seat->cursor->x - (total_width / 2);
	int y = (int)seat->cursor->y - (total_height / 2);
	x = MIN(x, usable.x + usable.width - total_width);
	geom->x = MAX(x, usable.x) + margin.left;
	y = MIN(y, usable.y + usable.height - total_height);
	geom->y = MAX(y, usable.y) + margin.top;
	return true;
}
struct view_size_hints
view_get_size_hints(struct view *view)
{
	assert(view);
	if (view->impl->get_size_hints) {
		return view->impl->get_size_hints(view);
	}
	return (struct view_size_hints){0};
}
static void
LABWC_src_view_c_substitute_nonzero(int *a, int *b)
{
	if (!(*a)) {
		*a = *b;
	} else if (!(*b)) {
		*b = *a;
	}
}
static int
LABWC_src_view_c_round_to_increment(int val, int base, int inc)
{
	if (base < 0 || inc <= 0) {
		return val;
	}
	return base + (val - base + inc / 2) / inc * inc;
}
void
view_adjust_size(struct view *view, int *w, int *h)
{
	assert(view);
	struct view_size_hints hints = view_get_size_hints(view);
	LABWC_src_view_c_substitute_nonzero(&hints.min_width, &hints.base_width);
	LABWC_src_view_c_substitute_nonzero(&hints.min_height, &hints.base_height);
	*w = LABWC_src_view_c_round_to_increment(*w, hints.base_width, hints.width_inc);
	*h = LABWC_src_view_c_round_to_increment(*h, hints.base_height, hints.height_inc);
	if (hints.min_width < 1) {
		hints.min_width = LAB_MIN_VIEW_WIDTH;
	}
	if (hints.min_height < 1) {
		hints.min_height = LAB_MIN_VIEW_HEIGHT;
	}
	*w = MAX(*w, hints.min_width);
	*h = MAX(*h, hints.min_height);
}
static void
LABWC_src_view_c__minimize(struct view *view, bool minimized, bool *need_refocus)
{
	assert(view);
	if (view->minimized == minimized) {
		return;
	}
	if (view->impl->minimize) {
		view->impl->minimize(view, minimized);
	}
	view->minimized = minimized;
	wl_signal_emit_mutable(&view->events.minimized, NULL);
	view_update_visibility(view);
	*need_refocus |= (minimized ?
		(view == server.active_view) : view->mapped);
}
static void
LABWC_src_view_c_view_append_children(struct view *view, struct wl_array *children)
{
	assert(view);
	if (view->impl->append_children) {
		view->impl->append_children(view, children);
	}
}
static void
LABWC_src_view_c_minimize_sub_views(struct view *view, bool minimized, bool *need_refocus)
{
	struct view **child;
	struct wl_array children;
	wl_array_init(&children);
	LABWC_src_view_c_view_append_children(view, &children);
	wl_array_for_each(child, &children) {
		LABWC_src_view_c__minimize(*child, minimized, need_refocus);
		LABWC_src_view_c_minimize_sub_views(*child, minimized, need_refocus);
	}
	wl_array_release(&children);
}
void
view_minimize(struct view *view, bool minimized)
{
	assert(view);
	bool need_refocus = false;
	if (false) {
		return;
	}
	struct view *root = LABWC_src_view_c_view_get_root(view);
	LABWC_src_view_c__minimize(root, minimized, &need_refocus);
	LABWC_src_view_c_minimize_sub_views(root, minimized, &need_refocus);
	if (need_refocus) {
		if (minimized) {
			desktop_focus_topmost_view();
		} else {
			desktop_focus_view(view,   true);
		}
	}
}
bool
view_compute_centered_position(struct view *view, const struct wlr_box *ref,
		int w, int h, int *x, int *y)
{
	assert(view);
	if (w <= 0 || h <= 0) {
		return false;
	}
	if (!output_is_usable(view->output)) {
		return false;
	}
	struct border margin = ssd_get_margin(view->ssd);
	struct wlr_box usable = output_usable_area_in_layout_coords(view->output);
	int width = w + margin.left + margin.right;
	int height = h + margin.top + margin.bottom;
	box_center(width, height, ref ? ref : &usable, &usable, x, y);
	*x += margin.left;
	*y += margin.top;
	return true;
}
static bool
LABWC_src_view_c_adjust_floating_geometry(struct view *view, struct wlr_box *geometry,
		bool midpoint_visibility)
{
	assert(view);
	if (!output_is_usable(view->output)) {
		return false;
	}
	if (window_rules_get_property(view, "fixedPosition") == LAB_PROP_TRUE
			|| view_has_strut_partial(view)) {
		return false;
	}
	bool adjusted = false;
	bool onscreen = false;
	if (wlr_output_layout_intersects(server.output_layout,
			view->output->wlr_output, geometry)) {
		struct border margin = ssd_get_margin(view->ssd);
		struct wlr_box usable =
			output_usable_area_in_layout_coords(view->output);
		if (geometry->x < usable.x + margin.left) {
			geometry->x = usable.x + margin.left;
			adjusted = true;
		}
		if (geometry->y < usable.y + margin.top) {
			geometry->y = usable.y + margin.top;
			adjusted = true;
		}
		if (!midpoint_visibility) {
			onscreen = true;
		} else {
			int mx = geometry->x + geometry->width / 2;
			int my = geometry->y + geometry->height / 2;
			onscreen = mx <= usable.x + usable.width &&
				my <= usable.y + usable.height;
		}
	}
	if (onscreen) {
		return adjusted;
	}
	return view_compute_position_by_policy(view, geometry,
		  true, rc.placement_policy);
}
struct wlr_box
view_get_fallback_natural_geometry(struct view *view)
{
	struct wlr_box box = {
		.width = VIEW_FALLBACK_WIDTH,
		.height = VIEW_FALLBACK_HEIGHT,
	};
	view_compute_position_by_policy(view, &box,
		  true, rc.placement_policy);
	return box;
}
void
view_store_natural_geometry(struct view *view)
{
	assert(view);
	if (view->fullscreen || view_is_tiled(view)) {
		return;
	}
	if (!(view->maximized & VIEW_AXIS_HORIZONTAL)) {
		view->natural_geometry.x = view->pending.x;
		view->natural_geometry.width = view->pending.width;
	}
	if (!(view->maximized & VIEW_AXIS_VERTICAL)) {
		view->natural_geometry.y = view->pending.y;
		view->natural_geometry.height = view->pending.height;
	}
}
int
view_effective_height(struct view *view, bool use_pending)
{
	assert(view);
	if (view->shaded) {
		return 0;
	}
	return use_pending ? view->pending.height : view->current.height;
}
void
view_center(struct view *view, const struct wlr_box *ref)
{
	assert(view);
	int x, y;
	if (view_compute_centered_position(view, ref, view->pending.width,
			view->pending.height, &x, &y)) {
		view_move(view, x, y);
	}
}
static bool
LABWC_src_view_c_view_compute_cascaded_position(struct view *view, struct wlr_box *geom)
{
	struct wlr_box center = *geom;
	if (!view_compute_centered_position(view, NULL, center.width,
			center.height, &center.x, &center.y)) {
		return false;
	}
	struct border margin = ssd_get_margin(view->ssd);
	center.x -= margin.left;
	center.y -= margin.top;
	center.width += margin.left + margin.right;
	center.height += margin.top + margin.bottom;
	struct wlr_box candidate = center;
	struct wlr_box usable = output_usable_area_in_layout_coords(view->output);
	int offset_x = rc.placement_cascade_offset_x;
	int offset_y = rc.placement_cascade_offset_y;
	struct theme *theme = rc.theme;
	int default_offset = theme->titlebar_height + theme->border_width + 5;
	if (offset_x <= 0) {
		offset_x = default_offset;
	}
	if (offset_y <= 0) {
		offset_y = default_offset;
	}
	bool candidate_updated = true;
	while (candidate_updated) {
		candidate_updated = false;
		struct wlr_box covered = {0};
		struct view *other_view;
		for_each_view(other_view, &server.views,
				LAB_VIEW_CRITERIA_CURRENT_WORKSPACE) {
			struct wlr_box other = ssd_max_extents(other_view);
			if (other_view == view
					|| view->minimized
					|| !box_intersects(&candidate, &other)) {
				continue;
			}
			if (wlr_box_contains_box(&candidate, &other)
					&& !wlr_box_contains_point(
						&covered, other.x, other.y)) {
				candidate.x = other.x + offset_x;
				candidate.y = other.y + offset_y;
				if (!wlr_box_contains_box(&usable, &candidate)) {
					candidate = center;
					break;
				} else {
					candidate_updated = true;
					break;
				}
			}
			box_union(&covered, &covered, &other);
		}
	}
	geom->x = candidate.x + margin.left;
	geom->y = candidate.y + margin.top;
	return true;
}
bool
view_compute_position_by_policy(struct view *view, struct wlr_box *geom,
		bool allow_cursor, enum lab_placement_policy policy)
{
	if (allow_cursor && policy == LAB_PLACE_CURSOR) {
		return LABWC_src_view_c_view_compute_near_cursor_position(view, geom);
	} else if (policy == LAB_PLACE_AUTOMATIC) {
		return placement_find_best(view, geom);
	} else if (policy == LAB_PLACE_CASCADE) {
		return LABWC_src_view_c_view_compute_cascaded_position(view, geom);
	} else {
		return view_compute_centered_position(view, NULL,
			geom->width, geom->height, &geom->x, &geom->y);
	}
}
void
view_place_by_policy(struct view *view, bool allow_cursor,
		enum lab_placement_policy policy)
{
	view_set_fullscreen(view, false);
	view_maximize(view, VIEW_AXIS_NONE);
	if (view_is_tiled(view)) {
		view_set_untiled(view);
		view_move_resize(view, view->natural_geometry);
	}
	struct wlr_box geom = view->pending;
	if (view_compute_position_by_policy(view, &geom, allow_cursor, policy)) {
		view_move(view, geom.x, geom.y);
	}
}
void
view_constrain_size_to_that_of_usable_area(struct view *view)
{
	if (!view || !output_is_usable(view->output) || view->fullscreen) {
		return;
	}
	struct wlr_box usable_area =
			output_usable_area_in_layout_coords(view->output);
	struct border margin = ssd_get_margin(view->ssd);
	int available_width = usable_area.width - margin.left - margin.right;
	int available_height = usable_area.height - margin.top - margin.bottom;
	if (available_width <= 0 || available_height <= 0) {
		return;
	}
	if (available_height >= view->pending.height &&
			available_width >= view->pending.width) {
		return;
	}
	int width = MIN(view->pending.width, available_width);
	int height = MIN(view->pending.height, available_height);
	int right_edge = usable_area.x + usable_area.width;
	int bottom_edge = usable_area.y + usable_area.height;
	int x =
		MAX(usable_area.x + margin.left,
			MIN(view->pending.x, right_edge - width - margin.right));
	int y =
		MAX(usable_area.y + margin.top,
			MIN(view->pending.y, bottom_edge - height - margin.bottom));
	struct wlr_box box = {
		.x = x,
		.y = y,
		.width = width,
		.height = height,
	};
	view_move_resize(view, box);
}
void
view_apply_natural_geometry(struct view *view)
{
	assert(view);
	assert(view_is_floating(view));
	struct wlr_box geometry = view->natural_geometry;
	if (!wlr_box_empty(&geometry)) {
		LABWC_src_view_c_adjust_floating_geometry(view, &geometry,
			  false);
	}
	view_move_resize(view, geometry);
}
struct wlr_box
view_get_region_snap_box(struct view *view, struct region *region)
{
	struct wlr_box geo = region->geo;
	if (rc.gap) {
		double half_gap = rc.gap / 2.0;
		struct wlr_fbox offset = {
			.x = half_gap,
			.y = half_gap,
			.width = -rc.gap,
			.height = -rc.gap
		};
		struct wlr_box usable =
			output_usable_area_in_layout_coords(region->output);
		if (geo.x == usable.x) {
			offset.x += half_gap;
			offset.width -= half_gap;
		}
		if (geo.y == usable.y) {
			offset.y += half_gap;
			offset.height -= half_gap;
		}
		if (geo.x + geo.width == usable.x + usable.width) {
			offset.width -= half_gap;
		}
		if (geo.y + geo.height == usable.y + usable.height) {
			offset.height -= half_gap;
		}
		geo.x = (int)(geo.x + offset.x);
		geo.y = (int)(geo.y + offset.y);
		geo.width = (int)(geo.width + offset.width);
		geo.height = (int)(geo.height + offset.height);
	}
	if (view) {
		struct border margin = ssd_get_margin(view->ssd);
		geo.x += margin.left;
		geo.y += margin.top;
		geo.width -= margin.left + margin.right;
		geo.height -= margin.top + margin.bottom;
	}
	return geo;
}
static void
LABWC_src_view_c_view_apply_region_geometry(struct view *view)
{
	assert(view);
	assert(view->tiled_region || view->tiled_region_evacuate);
	struct output *output = view->output;
	assert(output_is_usable(output));
	if (view->tiled_region_evacuate) {
		view->tiled_region = regions_from_name(
			view->tiled_region_evacuate, output);
		zfree(view->tiled_region_evacuate);
		if (!view->tiled_region) {
			view_set_untiled(view);
			view_apply_natural_geometry(view);
			return;
		}
	}
	struct wlr_box geo = view_get_region_snap_box(view, view->tiled_region);
	view_move_resize(view, geo);
}
static void
LABWC_src_view_c_view_apply_tiled_geometry(struct view *view)
{
	assert(view);
	assert(view->tiled);
	assert(output_is_usable(view->output));
	view_move_resize(view, view_get_edge_snap_box(view,
		view->output, view->tiled));
}
static void
LABWC_src_view_c_view_apply_fullscreen_geometry(struct view *view)
{
	assert(view);
	assert(view->fullscreen);
	assert(output_is_usable(view->output));
	struct wlr_box box = { 0 };
	wlr_output_layout_get_box(server.output_layout,
		view->output->wlr_output, &box);
	view_move_resize(view, box);
}
static void
LABWC_src_view_c_view_apply_maximized_geometry(struct view *view)
{
	assert(view);
	assert(view->maximized != VIEW_AXIS_NONE);
	struct output *output = view->output;
	assert(output_is_usable(output));
	struct wlr_box box = output_usable_area_in_layout_coords(output);
	struct wlr_box natural = view->natural_geometry;
	if (view->maximized != VIEW_AXIS_BOTH
			&& !box_intersects(&box, &natural)) {
		view_compute_centered_position(view, NULL,
			natural.width, natural.height,
			&natural.x, &natural.y);
	}
	if (view->ssd_mode) {
		struct border border = ssd_thickness(view);
		box.x += border.left;
		box.y += border.top;
		box.width -= border.right + border.left;
		box.height -= border.top + border.bottom;
	}
	if (view->maximized == VIEW_AXIS_VERTICAL) {
		box.x = natural.x;
		box.width = natural.width;
	} else if (view->maximized == VIEW_AXIS_HORIZONTAL) {
		box.y = natural.y;
		box.height = natural.height;
	}
	view_move_resize(view, box);
}
static void
LABWC_src_view_c_view_apply_special_geometry(struct view *view)
{
	assert(view);
	assert(!view_is_floating(view));
	if (!output_is_usable(view->output)) {
		return;
	}
	if (view->fullscreen) {
		LABWC_src_view_c_view_apply_fullscreen_geometry(view);
	} else if (view->maximized != VIEW_AXIS_NONE) {
		LABWC_src_view_c_view_apply_maximized_geometry(view);
	} else if (view->tiled) {
		LABWC_src_view_c_view_apply_tiled_geometry(view);
	} else if (view->tiled_region || view->tiled_region_evacuate) {
		LABWC_src_view_c_view_apply_region_geometry(view);
	} else {
		assert(false);
	}
}
void
view_set_maximized(struct view *view, enum view_axis maximized)
{
	assert(view);
	if (view->maximized == maximized) {
		return;
	}
	if (view->impl->maximize) {
		view->impl->maximize(view, maximized);
	}
	view->maximized = maximized;
	wl_signal_emit_mutable(&view->events.maximized, NULL);
	ssd_update_margin(view->ssd);
}
bool
view_is_tiled(struct view *view)
{
	assert(view);
	return (view->tiled || view->tiled_region
		|| view->tiled_region_evacuate);
}
bool
view_is_tiled_and_notify_tiled(struct view *view)
{
	switch (rc.snap_tiling_events_mode) {
	case LAB_TILING_EVENTS_NEVER:
		return false;
	case LAB_TILING_EVENTS_REGION:
		return view->tiled_region || view->tiled_region_evacuate;
	case LAB_TILING_EVENTS_EDGE:
		return view->tiled;
	case LAB_TILING_EVENTS_ALWAYS:
		return view_is_tiled(view);
	}
	return false;
}
bool
view_is_floating(struct view *view)
{
	assert(view);
	return !(view->fullscreen || (view->maximized != VIEW_AXIS_NONE)
		|| view_is_tiled(view));
}
static void
LABWC_src_view_c_view_notify_tiled(struct view *view)
{
	assert(view);
	if (view->impl->notify_tiled) {
		view->impl->notify_tiled(view);
	}
}
void
view_set_untiled(struct view *view)
{
	assert(view);
	view->tiled = LAB_EDGE_NONE;
	view->tiled_region = NULL;
	zfree(view->tiled_region_evacuate);
	LABWC_src_view_c_view_notify_tiled(view);
}
static bool
LABWC_src_view_c_in_interactive_move(struct view *view)
{
	return (server.input_mode == LAB_INPUT_STATE_MOVE
		&& server.grabbed_view == view);
}
void
view_maximize(struct view *view, enum view_axis axis)
{
	assert(view);
	if (view->maximized == axis) {
		return;
	}
	if (view->fullscreen) {
		return;
	}
	bool store_natural_geometry = !LABWC_src_view_c_in_interactive_move(view);
	view_set_shade(view, false);
	interactive_cancel(view);
	if (store_natural_geometry) {
		view_store_natural_geometry(view);
	}
	if ((axis == VIEW_AXIS_HORIZONTAL || axis == VIEW_AXIS_VERTICAL)
			&& wlr_box_empty(&view->natural_geometry)) {
		view->natural_geometry = view_get_fallback_natural_geometry(view);
	}
	view_set_maximized(view, axis);
	if (view_is_floating(view)) {
		view_apply_natural_geometry(view);
	} else {
		LABWC_src_view_c_view_apply_special_geometry(view);
	}
}
void
view_toggle_maximize(struct view *view, enum view_axis axis)
{
	assert(view);
	switch (axis) {
	case VIEW_AXIS_HORIZONTAL:
	case VIEW_AXIS_VERTICAL:
		view_maximize(view, view->maximized ^ axis);
		break;
	case VIEW_AXIS_BOTH:
		view_maximize(view, (view->maximized == VIEW_AXIS_BOTH) ?
			VIEW_AXIS_NONE : VIEW_AXIS_BOTH);
		break;
	default:
		break;
	}
}
bool
view_wants_decorations(struct view *view)
{
	switch (window_rules_get_property(view, "serverDecoration")) {
	case LAB_PROP_TRUE:
		return true;
	case LAB_PROP_FALSE:
		return false;
	default:
		break;
	}
	switch (view->ssd_preference) {
	case LAB_SSD_PREF_SERVER:
		return true;
	case LAB_SSD_PREF_CLIENT:
		return false;
	default:
		return rc.xdg_shell_server_side_deco;
	}
}
void
view_set_decorations(struct view *view, enum lab_ssd_mode mode, bool force_ssd)
{
	assert(view);
	if (force_ssd || view_wants_decorations(view)
			|| mode < view->ssd_mode) {
		view_set_ssd_mode(view, mode);
	}
}
void
view_toggle_decorations(struct view *view)
{
	assert(view);
	if (rc.ssd_keep_border && view->ssd_mode == LAB_SSD_MODE_FULL) {
		view_set_ssd_mode(view, LAB_SSD_MODE_BORDER);
	} else if (view->ssd_mode != LAB_SSD_MODE_NONE) {
		view_set_ssd_mode(view, LAB_SSD_MODE_NONE);
	} else {
		view_set_ssd_mode(view, LAB_SSD_MODE_FULL);
	}
}
void
view_set_layer(struct view *view, enum view_layer layer)
{
	assert(view);
	if (view->layer == layer) {
		return;
	}
	view->layer = layer;
	wlr_scene_node_reparent(&view->scene_tree->node,
		view->workspace->view_trees[layer]);
	wl_signal_emit_mutable(&view->events.always_on_top, NULL);
}
void
view_toggle_always_on_top(struct view *view)
{
	assert(view);
	if (view->layer == VIEW_LAYER_ALWAYS_ON_TOP) {
		view_set_layer(view, VIEW_LAYER_NORMAL);
	} else {
		view_set_layer(view, VIEW_LAYER_ALWAYS_ON_TOP);
	}
}
void
view_toggle_always_on_bottom(struct view *view)
{
	assert(view);
	if (view->layer == VIEW_LAYER_ALWAYS_ON_BOTTOM) {
		view_set_layer(view, VIEW_LAYER_NORMAL);
	} else {
		view_set_layer(view, VIEW_LAYER_ALWAYS_ON_BOTTOM);
	}
}
void
view_toggle_visible_on_all_workspaces(struct view *view)
{
	assert(view);
	view->visible_on_all_workspaces = !view->visible_on_all_workspaces;
	ssd_update_geometry(view->ssd);
}
void
view_move_to_workspace(struct view *view, struct workspace *workspace)
{
	assert(view);
	assert(workspace);
	if (view->workspace != workspace) {
		view->workspace = workspace;
		wlr_scene_node_reparent(&view->scene_tree->node,
			workspace->view_trees[view->layer]);
	}
}
static void
LABWC_src_view_c_decorate(struct view *view)
{
	if (!view->ssd) {
		view->ssd = ssd_create(view,
			view == server.active_view);
	}
}
static void
LABWC_src_view_c_undecorate(struct view *view)
{
	ssd_destroy(view->ssd);
	view->ssd = NULL;
}
bool
view_titlebar_visible(struct view *view)
{
	if (view->maximized == VIEW_AXIS_BOTH
			&& rc.hide_maximized_window_titlebar) {
		return false;
	}
	return view->ssd_mode == LAB_SSD_MODE_FULL;
}
void
view_set_ssd_mode(struct view *view, enum lab_ssd_mode mode)
{
	assert(view);
	if (view->shaded || view->fullscreen
			|| mode == view->ssd_mode) {
		return;
	}
	view->ssd_mode = mode;
	if (mode) {
		LABWC_src_view_c_decorate(view);
		ssd_set_titlebar(view->ssd, view_titlebar_visible(view));
	} else {
		LABWC_src_view_c_undecorate(view);
	}
	if (!view_is_floating(view)) {
		LABWC_src_view_c_view_apply_special_geometry(view);
	}
}
void
view_toggle_fullscreen(struct view *view)
{
	assert(view);
	view_set_fullscreen(view, !view->fullscreen);
}
static void
LABWC_src_view_c_set_fullscreen(struct view *view, bool fullscreen)
{
	if (fullscreen) {
		view_set_shade(view, false);
	}
	if (fullscreen && view->ssd_mode) {
		LABWC_src_view_c_undecorate(view);
	}
	if (view->impl->set_fullscreen) {
		view->impl->set_fullscreen(view, fullscreen);
	}
	view->fullscreen = fullscreen;
	wl_signal_emit_mutable(&view->events.fullscreened, NULL);
	if (!fullscreen && view->ssd_mode) {
		LABWC_src_view_c_decorate(view);
	}
	if (view->output) {
		desktop_update_top_layer_visibility();
	}
}
void
view_set_fullscreen(struct view *view, bool fullscreen)
{
	assert(view);
	if (fullscreen == view->fullscreen) {
		return;
	}
	if (fullscreen) {
		if (!output_is_usable(view->output)) {
			return;
		}
		interactive_cancel(view);
		view_store_natural_geometry(view);
	}
	LABWC_src_view_c_set_fullscreen(view, fullscreen);
	if (view_is_floating(view)) {
		view_apply_natural_geometry(view);
	} else {
		LABWC_src_view_c_view_apply_special_geometry(view);
	}
	cursor_update_focus();
}
void
view_save_last_placement(struct view *view)
{
	assert(view);
	struct output *output = view->output;
	if (!output_is_usable(output)) {
		return;
	}
	if (!str_equal(view->last_placement.output_name, output->wlr_output->name)) {
		xstrdup_replace(view->last_placement.output_name,
			output->wlr_output->name);
	}
	view->last_placement.layout_geo = view->pending;
	view->last_placement.relative_geo = view->pending;
	view->last_placement.relative_geo.x -= output->scene_output->x;
	view->last_placement.relative_geo.y -= output->scene_output->y;
}
static void
LABWC_src_view_c_clear_last_placement(struct view *view)
{
	assert(view);
	zfree(view->last_placement.output_name);
	view->last_placement.relative_geo = (struct wlr_box){0};
	view->last_placement.layout_geo = (struct wlr_box){0};
}
void
view_adjust_for_layout_change(struct view *view)
{
	assert(view);
	if (wlr_box_empty(&view->last_placement.layout_geo)) {
		return;
	}
	view->adjusting_for_layout_change = true;
	struct wlr_box new_geo;
	struct output *output = output_from_name(view->last_placement.output_name);
	if (output_is_usable(output)) {
		new_geo = view->last_placement.relative_geo;
		new_geo.x += output->scene_output->x;
		new_geo.y += output->scene_output->y;
		view->output = output;
	} else {
		LABWC_src_view_c_view_discover_output(view, &view->last_placement.layout_geo);
		new_geo = view->last_placement.layout_geo;
	}
	if (!view_is_floating(view)) {
		LABWC_src_view_c_view_apply_special_geometry(view);
	} else {
		LABWC_src_view_c_adjust_floating_geometry(view, &new_geo,
			  true);
		view_move_resize(view, new_geo);
	}
	LABWC_src_view_c_view_update_outputs(view);
	view->adjusting_for_layout_change = false;
}
void
view_evacuate_region(struct view *view)
{
	assert(view);
	assert(view->tiled_region);
	if (!view->tiled_region_evacuate) {
		view->tiled_region_evacuate = xstrdup(view->tiled_region->name);
	}
	view->tiled_region = NULL;
}
void
view_on_output_destroy(struct view *view)
{
	assert(view);
	view->output = NULL;
}
static int
LABWC_src_view_c_shift_view_to_usable_1d(int size,
		int cur_pos, int cur_lo, int cur_extent,
		int next_pos, int next_lo, int next_extent,
		int margin_lo, int margin_hi)
{
	int cur_min = cur_lo + rc.gap + margin_lo;
	int cur_max = cur_lo + cur_extent - rc.gap - margin_hi;
	int next_min = next_lo + rc.gap + margin_lo;
	int next_max = next_lo + next_extent - rc.gap - margin_hi;
	if (cur_pos >= cur_min && cur_pos + size <= cur_max) {
		if (next_pos >= next_min && next_pos + size > next_max) {
			next_pos = next_max - size;
		}
		return MAX(next_pos, next_min);
	}
	int midpoint = next_pos + size / 2;
	if (next_pos >= next_min && midpoint > next_lo + next_extent) {
		next_pos = next_max - size;
	}
	return MAX(next_pos, next_min);
}
void
view_move_to_edge(struct view *view, enum lab_edge direction, bool snap_to_windows)
{
	assert(view);
	if (!output_is_usable(view->output)) {
		return;
	}
	int dx = 0, dy = 0;
	snap_move_to_edge(view, direction, snap_to_windows, &dx, &dy);
	if (dx != 0 || dy != 0) {
		view_move(view, view->pending.x + dx, view->pending.y + dy);
		return;
	}
	if (view->maximized != VIEW_AXIS_NONE) {
		return;
	}
	struct output *output =
		output_get_adjacent(view->output, direction,   false);
	if (!output_is_usable(output)) {
		return;
	}
	struct wlr_box usable = output_usable_area_in_layout_coords(output);
	struct border margin = ssd_get_margin(view->ssd);
	int left = usable.x + rc.gap + margin.left;
	int right = usable.x + usable.width - rc.gap - margin.right;
	int top = usable.y + rc.gap + margin.top;
	int bottom = usable.y + usable.height - rc.gap - margin.bottom;
	int destination_x = view->pending.x;
	int destination_y = view->pending.y;
	direction = lab_edge_invert(direction);
	switch (direction) {
	case LAB_EDGE_LEFT:
		destination_x = left;
		break;
	case LAB_EDGE_RIGHT:
		destination_x = right - view->pending.width;
		break;
	case LAB_EDGE_TOP:
		destination_y = top;
		break;
	case LAB_EDGE_BOTTOM:
		destination_y = bottom
			- view_effective_height(view,   true);
		break;
	default:
		return;
	}
	struct wlr_box original_usable =
		output_usable_area_in_layout_coords(view->output);
	destination_x = LABWC_src_view_c_shift_view_to_usable_1d(view->pending.width,
		view->pending.x, original_usable.x, original_usable.width,
		destination_x, usable.x, usable.width, margin.left, margin.right);
	int eff_height = view_effective_height(view,   true);
	destination_y = LABWC_src_view_c_shift_view_to_usable_1d(eff_height,
		view->pending.y, original_usable.y, original_usable.height,
		destination_y, usable.y, usable.height, margin.top, margin.bottom);
	view_set_untiled(view);
	view_set_output(view, output);
	view_move(view, destination_x, destination_y);
}
void
view_grow_to_edge(struct view *view, enum lab_edge direction)
{
	assert(view);
	if (view->fullscreen || view->maximized != VIEW_AXIS_NONE) {
		return;
	}
	if (!output_is_usable(view->output)) {
		return;
	}
	view_set_shade(view, false);
	struct wlr_box geo;
	snap_grow_to_next_edge(view, direction, &geo);
	view_move_resize(view, geo);
}
void
view_shrink_to_edge(struct view *view, enum lab_edge direction)
{
	assert(view);
	if (view->fullscreen || view->maximized != VIEW_AXIS_NONE) {
		return;
	}
	if (!output_is_usable(view->output)) {
		return;
	}
	view_set_shade(view, false);
	struct wlr_box geo = view->pending;
	snap_shrink_to_next_edge(view, direction, &geo);
	view_move_resize(view, geo);
}
enum view_axis
view_axis_parse(const char *direction)
{
	if (!direction) {
		return VIEW_AXIS_INVALID;
	}
	if (!strcasecmp(direction, "horizontal")) {
		return VIEW_AXIS_HORIZONTAL;
	} else if (!strcasecmp(direction, "vertical")) {
		return VIEW_AXIS_VERTICAL;
	} else if (!strcasecmp(direction, "both")) {
		return VIEW_AXIS_BOTH;
	} else if (!strcasecmp(direction, "none")) {
		return VIEW_AXIS_NONE;
	} else {
		return VIEW_AXIS_INVALID;
	}
}
enum lab_placement_policy
view_placement_parse(const char *policy)
{
	if (!policy) {
		return LAB_PLACE_CENTER;
	}
	if (!strcasecmp(policy, "automatic")) {
		return LAB_PLACE_AUTOMATIC;
	} else if (!strcasecmp(policy, "cursor")) {
		return LAB_PLACE_CURSOR;
	} else if (!strcasecmp(policy, "center")) {
		return LAB_PLACE_CENTER;
	} else if (!strcasecmp(policy, "cascade")) {
		return LAB_PLACE_CASCADE;
	}
	return LAB_PLACE_INVALID;
}
void
view_snap_to_edge(struct view *view, enum lab_edge edge,
		bool across_outputs, bool combine)
{
	assert(view);
	if (view->fullscreen) {
		return;
	}
	struct output *output = view->output;
	if (!output_is_usable(output)) {
		return;
	}
	bool store_natural_geometry = !LABWC_src_view_c_in_interactive_move(view);
	view_set_shade(view, false);
	if (lab_edge_is_cardinal(edge) && view->maximized == VIEW_AXIS_NONE
			&& view->tiled != LAB_EDGE_CENTER) {
		enum lab_edge invert_edge = lab_edge_invert(edge);
		enum lab_edge parallel_mask = edge | invert_edge;
		enum lab_edge parallel_tiled = view->tiled & parallel_mask;
		enum lab_edge orthogonal_tiled = view->tiled & ~parallel_mask;
		if (across_outputs && view->tiled == edge) {
			output = output_get_adjacent(view->output, edge,
				  false);
			if (!output_is_usable(output)) {
				return;
			}
			edge = invert_edge;
		} else if (combine && parallel_tiled == invert_edge
				&& orthogonal_tiled != LAB_EDGE_NONE) {
			edge = view->tiled & ~parallel_mask;
		} else if (combine && parallel_tiled == LAB_EDGE_NONE) {
			edge = view->tiled | edge;
		}
	}
	if (view->maximized != VIEW_AXIS_NONE) {
		view_maximize(view, VIEW_AXIS_NONE);
	} else if (store_natural_geometry) {
		view_store_natural_geometry(view);
	}
	view_set_untiled(view);
	view_set_output(view, output);
	view->tiled = edge;
	LABWC_src_view_c_view_notify_tiled(view);
	LABWC_src_view_c_view_apply_tiled_geometry(view);
}
void
view_snap_to_region(struct view *view, struct region *region)
{
	assert(view);
	assert(region);
	if (view->fullscreen) {
		return;
	}
	if (!output_is_usable(view->output)) {
		return;
	}
	bool store_natural_geometry = !LABWC_src_view_c_in_interactive_move(view);
	view_set_shade(view, false);
	if (view->maximized != VIEW_AXIS_NONE) {
		view_maximize(view, VIEW_AXIS_NONE);
	} else if (store_natural_geometry) {
		view_store_natural_geometry(view);
	}
	view_set_untiled(view);
	view->tiled_region = region;
	LABWC_src_view_c_view_notify_tiled(view);
	LABWC_src_view_c_view_apply_region_geometry(view);
}
void
view_move_to_output(struct view *view, struct output *output)
{
	assert(view);
	view_set_output(view, output);
	if (view_is_floating(view)) {
		struct wlr_box output_area = output_usable_area_in_layout_coords(output);
		view->pending.x = output_area.x;
		view->pending.y = output_area.y;
		view_place_by_policy(view,
				  false, rc.placement_policy);
	} else if (view->fullscreen) {
		LABWC_src_view_c_view_apply_fullscreen_geometry(view);
	} else if (view->maximized != VIEW_AXIS_NONE) {
		LABWC_src_view_c_view_apply_maximized_geometry(view);
	} else if (view->tiled) {
		LABWC_src_view_c_view_apply_tiled_geometry(view);
	} else if (view->tiled_region) {
		struct region *region = regions_from_name(view->tiled_region->name, output);
		view_snap_to_region(view, region);
	}
}
static void
LABWC_src_view_c_for_each_subview(struct view *view, void (*action)(struct view *))
{
	struct wl_array subviews;
	struct view **subview;
	wl_array_init(&subviews);
	LABWC_src_view_c_view_append_children(view, &subviews);
	wl_array_for_each(subview, &subviews) {
		action(*subview);
	}
	wl_array_release(&subviews);
}
static void
LABWC_src_view_c_move_to_front(struct view *view)
{
	wl_list_remove(&view->link);
	wl_list_insert(&server.views, &view->link);
	wlr_scene_node_raise_to_top(&view->scene_tree->node);
}
static void
LABWC_src_view_c_move_to_back(struct view *view)
{
	wl_list_remove(&view->link);
	wl_list_append(&server.views, &view->link);
	wlr_scene_node_lower_to_bottom(&view->scene_tree->node);
}
void
view_move_to_front(struct view *view)
{
	assert(view);
	assert(!wl_list_empty(&server.views));
	struct view *front = wl_container_of(server.views.next, front, link);
	if (view == front || view == LABWC_src_view_c_view_get_root(front)) {
		return;
	}
	struct view *root = LABWC_src_view_c_view_get_root(view);
	assert(root);
	LABWC_src_view_c_move_to_front(root);
	LABWC_src_view_c_for_each_subview(root, LABWC_src_view_c_move_to_front);
	if (view != root) {
		LABWC_src_view_c_move_to_front(view);
	}
#if HAVE_XWAYLAND
	if (view->type == LAB_XWAYLAND_VIEW) {
		xwayland_flush();
	}
#endif
	cursor_update_focus();
	desktop_update_top_layer_visibility();
}
void
view_move_to_back(struct view *view)
{
	assert(view);
	struct view *root = LABWC_src_view_c_view_get_root(view);
	assert(root);
	LABWC_src_view_c_for_each_subview(root, LABWC_src_view_c_move_to_back);
	LABWC_src_view_c_move_to_back(root);
	cursor_update_focus();
	desktop_update_top_layer_visibility();
}
bool
view_is_modal_dialog(struct view *view)
{
	assert(view);
	assert(view->impl->is_modal_dialog);
	return view->impl->is_modal_dialog(view);
}
struct view *
view_get_modal_dialog(struct view *view)
{
	assert(view);
	if (view_is_modal_dialog(view)) {
		return view;
	}
	struct view *dialog = NULL;
	struct view *root = LABWC_src_view_c_view_get_root(view);
	struct wl_array children;
	struct view **child;
	wl_array_init(&children);
	LABWC_src_view_c_view_append_children(root, &children);
	wl_array_for_each(child, &children) {
		if (view_is_modal_dialog(*child)) {
			dialog = *child;
			break;
		}
	}
	wl_array_release(&children);
	return dialog;
}
bool
view_has_strut_partial(struct view *view)
{
	assert(view);
	return view->impl->has_strut_partial &&
		view->impl->has_strut_partial(view);
}
void
view_set_title(struct view *view, const char *title)
{
	assert(view);
	if (!title) {
		title = "";
	}
	if (!strcmp(view->title, title)) {
		return;
	}
	xstrdup_replace(view->title, title);
	wl_signal_emit_mutable(&view->events.new_title, NULL);
}
void
view_set_app_id(struct view *view, const char *app_id)
{
	assert(view);
	if (!app_id) {
		app_id = "";
	}
	if (!strcmp(view->app_id, app_id)) {
		return;
	}
	xstrdup_replace(view->app_id, app_id);
	wl_signal_emit_mutable(&view->events.new_app_id, NULL);
}
void
view_reload_ssd(struct view *view)
{
	assert(view);
	if (view->ssd_mode && !view->fullscreen) {
		LABWC_src_view_c_undecorate(view);
		LABWC_src_view_c_decorate(view);
	}
}
void
view_toggle_keybinds(struct view *view)
{
	assert(view);
	view->inhibits_keybinds = !view->inhibits_keybinds;
	if (view->ssd_mode) {
		ssd_enable_keybind_inhibit_indicator(view->ssd,
			view->inhibits_keybinds);
	}
}
bool
view_inhibits_actions(struct view *view, struct wl_list *actions)
{
	return view && view->inhibits_keybinds && !actions_contain_toggle_keybinds(actions);
}
void
mappable_connect(struct mappable *mappable, struct wlr_surface *surface,
		wl_notify_func_t notify_map, wl_notify_func_t notify_unmap)
{
	assert(mappable);
	assert(!mappable->connected);
	mappable->map.notify = notify_map;
	wl_signal_add(&surface->events.map, &mappable->map);
	mappable->unmap.notify = notify_unmap;
	wl_signal_add(&surface->events.unmap, &mappable->unmap);
	mappable->connected = true;
}
void
mappable_disconnect(struct mappable *mappable)
{
	assert(mappable);
	assert(mappable->connected);
	wl_list_remove(&mappable->map.link);
	wl_list_remove(&mappable->unmap.link);
	mappable->connected = false;
}
void
view_update_visibility(struct view *view)
{
	bool visible = view->mapped && !view->minimized;
	if (visible == view->scene_tree->node.enabled) {
		return;
	}
	wlr_scene_node_set_enabled(&view->scene_tree->node, visible);
	desktop_update_top_layer_visibility();
	if (view_has_strut_partial(view)) {
		output_update_all_usable_areas(false);
	}
	if (!visible) {
		interactive_cancel(view);
	}
}
void
view_set_shade(struct view *view, bool shaded)
{
	assert(view);
	if (view->shaded == shaded) {
		return;
	}
	if (shaded && (!view->ssd || !view_titlebar_visible(view))) {
		return;
	}
	if (shaded && server.input_mode == LAB_INPUT_STATE_RESIZE) {
		interactive_cancel(view);
	}
	view->shaded = shaded;
	ssd_enable_shade(view->ssd, view->shaded);
	if (view->content_tree) {
		wlr_scene_node_set_enabled(&view->content_tree->node,
			!view->shaded);
	}
}
void
view_init(struct view *view)
{
	assert(view);
	wl_signal_init(&view->events.new_app_id);
	wl_signal_init(&view->events.new_title);
	wl_signal_init(&view->events.new_outputs);
	wl_signal_init(&view->events.maximized);
	wl_signal_init(&view->events.minimized);
	wl_signal_init(&view->events.fullscreened);
	wl_signal_init(&view->events.activated);
	wl_signal_init(&view->events.always_on_top);
	wl_signal_init(&view->events.destroy);
	view->title = xstrdup("");
	view->app_id = xstrdup("");
	view->capture.scene = wlr_scene_create();
	view->capture.scene->restack_xwayland_surfaces = false;
	wl_list_init(&view->capture.on_capture_source_destroy.link);
}
void
view_destroy(struct view *view)
{
	assert(view);
	wl_signal_emit_mutable(&view->events.destroy, NULL);
	snap_constraints_invalidate(view);
	if (view->mappable.connected) {
		mappable_disconnect(&view->mappable);
	}
	wl_list_remove(&view->request_move.link);
	wl_list_remove(&view->request_resize.link);
	wl_list_remove(&view->request_minimize.link);
	wl_list_remove(&view->request_maximize.link);
	wl_list_remove(&view->request_fullscreen.link);
	wl_list_remove(&view->set_title.link);
	wl_list_remove(&view->destroy.link);
	wl_list_remove(&view->capture.on_capture_source_destroy.link);
	wlr_scene_node_destroy(&view->capture.scene->tree.node);
	if (view->foreign_toplevel) {
		foreign_toplevel_destroy(view->foreign_toplevel);
		view->foreign_toplevel = NULL;
	}
	if (server.grabbed_view == view) {
		interactive_cancel(view);
	}
	if (server.active_view == view) {
		server.active_view = NULL;
	}
	if (server.pending_auto_raise_view == view) {
		desktop_cancel_pending_auto_raise();
	}
	if (server.session_lock_manager->last_active_view == view) {
		server.session_lock_manager->last_active_view = NULL;
	}
	if (view->tiled_region_evacuate) {
		zfree(view->tiled_region_evacuate);
	}
	LABWC_src_view_c_undecorate(view);
	LABWC_src_view_c_clear_last_placement(view);
	menu_on_view_destroy(view);
	if (view->scene_tree) {
		wlr_scene_node_destroy(&view->scene_tree->node);
		view->scene_tree = NULL;
	}
	assert(wl_list_empty(&view->events.new_app_id.listener_list));
	assert(wl_list_empty(&view->events.new_title.listener_list));
	assert(wl_list_empty(&view->events.new_outputs.listener_list));
	assert(wl_list_empty(&view->events.maximized.listener_list));
	assert(wl_list_empty(&view->events.minimized.listener_list));
	assert(wl_list_empty(&view->events.fullscreened.listener_list));
	assert(wl_list_empty(&view->events.activated.listener_list));
	assert(wl_list_empty(&view->events.always_on_top.listener_list));
	assert(wl_list_empty(&view->events.destroy.listener_list));
	zfree(view->title);
	zfree(view->app_id);
	wl_list_remove(&view->link);
	free(view);
	cursor_update_focus();
}
#include <assert.h>
#include <stdbool.h>
#include <strings.h>
static bool
LABWC_src_window_rules_c_other_instances_exist(struct view *self, struct view_query *query)
{
	struct wl_list *views = &server.views;
	struct view *view;
	wl_list_for_each(view, views, link) {
		if (view != self && view_matches_query(view, query)) {
			return true;
		}
	}
	return false;
}
static bool
LABWC_src_window_rules_c_view_matches_rule(struct window_rule *rule, struct view *view)
{
	struct view_query query = {
		.identifier = rule->identifier,
		.title = rule->title,
		.window_type = rule->window_type,
		.sandbox_engine = rule->sandbox_engine,
		.sandbox_app_id = rule->sandbox_app_id,
		.maximized = VIEW_AXIS_INVALID,
		.decoration = LAB_SSD_MODE_INVALID,
	};
	if (rule->match_once && LABWC_src_window_rules_c_other_instances_exist(view, &query)) {
		return false;
	}
	return view_matches_query(view, &query);
}
void
window_rules_apply(struct view *view, enum window_rule_event event)
{
	struct window_rule *rule;
	wl_list_for_each(rule, &rc.window_rules, link) {
		if (rule->event != event) {
			continue;
		}
		if (LABWC_src_window_rules_c_view_matches_rule(rule, view)) {
			actions_run(view, &rule->actions, NULL);
		}
	}
}
enum property
window_rules_get_property(struct view *view, const char *property)
{
	assert(property);
	struct window_rule *rule;
	wl_list_for_each_reverse(rule, &rc.window_rules, link) {
		if (LABWC_src_window_rules_c_view_matches_rule(rule, view)) {
			if (rule->server_decoration
					&& !strcasecmp(property, "serverDecoration")) {
				return rule->server_decoration;
			}
			if (rule->skip_taskbar
					&& !strcasecmp(property, "skipTaskbar")) {
				return rule->skip_taskbar;
			}
			if (rule->skip_window_switcher
					&& !strcasecmp(property, "skipWindowSwitcher")) {
				return LAB_PROP_FALSE;
			}
			if (rule->ignore_focus_request
					&& !strcasecmp(property, "ignoreFocusRequest")) {
				return rule->ignore_focus_request;
			}
			if (rule->ignore_configure_request
					&& !strcasecmp(property, "ignoreConfigureRequest")) {
				return rule->ignore_configure_request;
			}
			if (rule->fixed_position
					&& !strcasecmp(property, "fixedPosition")) {
				return rule->fixed_position;
			}
			if (rule->allow_always_on_top
					&& !strcasecmp(property, "allowAlwaysOnTop")) {
				return rule->allow_always_on_top;
			}
		}
	}
	return LAB_PROP_UNSPECIFIED;
}
#include <assert.h>
#include <cairo.h>
#include <pango/pangocairo.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#define EXT_WORKSPACES_VERSION 1
static size_t
LABWC_src_workspaces_c_parse_workspace_index(const char *name)
{
	long index;
	char *endptr;
	errno = 0;
	index = strtol(name, &endptr, 10);
	if (errno || *endptr != '\0' || index < 0) {
		return 0;
	}
	return (size_t)index;
}
static void
LABWC_src_workspaces_c__osd_update(void)
{
	struct theme *theme = rc.theme;
	uint16_t margin = 10;
	uint16_t padding = 2;
	uint16_t rect_height = (uint16_t)theme->osd_workspace_switcher_boxes_height;
	uint16_t rect_width = (uint16_t)theme->osd_workspace_switcher_boxes_width;
	bool hide_boxes = theme->osd_workspace_switcher_boxes_width == 0 ||
		theme->osd_workspace_switcher_boxes_height == 0;
	size_t workspace_count = (size_t)wl_list_length(&server.workspaces.all);
	uint16_t marker_width = (uint16_t)(workspace_count * (rect_width + padding) - padding);
	uint16_t width = (uint16_t)(margin * 2 + (marker_width < 200 ? 200 : marker_width));
	uint16_t height = (uint16_t)(margin * (hide_boxes ? 2 : 3) + rect_height + font_height(&rc.font_osd));
	cairo_t *cairo;
	cairo_surface_t *surface;
	struct workspace *workspace;
	struct output *output;
	wl_list_for_each(output, &server.outputs, link) {
		if (!output_is_usable(output)) {
			continue;
		}
		struct lab_data_buffer *buffer = buffer_create_cairo(width, height,
			output->wlr_output->scale);
		if (!buffer) {
			continue;
		}
		cairo = cairo_create(buffer->surface);
		set_cairo_color(cairo, theme->osd_bg_color);
		cairo_rectangle(cairo, 0, 0, width, height);
		cairo_fill(cairo);
		set_cairo_color(cairo, theme->osd_border_color);
		struct wlr_fbox border_fbox = {
			.width = width,
			.height = height,
		};
		draw_cairo_border(cairo, border_fbox, theme->osd_border_width);
		uint16_t x;
		if (!hide_boxes) {
			x = (uint16_t)((width - marker_width) / 2);
			wl_list_for_each(workspace, &server.workspaces.all, link) {
				bool active = workspace == server.workspaces.current;
				set_cairo_color(cairo, rc.theme->osd_label_text_color);
				struct wlr_fbox fbox = {
					.x = x,
					.y = margin,
					.width = rect_width,
					.height = rect_height,
				};
				draw_cairo_border(cairo, fbox,
					theme->osd_workspace_switcher_boxes_border_width);
				if (active) {
					cairo_rectangle(cairo, x, margin,
						rect_width, rect_height);
					cairo_fill(cairo);
				}
				x += (uint16_t)(rect_width + padding);
			}
		}
		set_cairo_color(cairo, rc.theme->osd_label_text_color);
		PangoLayout *layout = pango_cairo_create_layout(cairo);
		pango_context_set_round_glyph_positions(pango_layout_get_context(layout), false);
		pango_layout_set_ellipsize(layout, PANGO_ELLIPSIZE_END);
		int req_width = font_width(&rc.font_osd, server.workspaces.current->name);
		req_width = MIN(req_width, width - 2 * margin);
		x = (uint16_t)((width - req_width) / 2);
		if (!hide_boxes) {
			cairo_move_to(cairo, x, margin * 2 + rect_height);
		} else {
			cairo_move_to(cairo, x, (height - font_height(&rc.font_osd)) / 2.0);
		}
		PangoFontDescription *desc = font_to_pango_desc(&rc.font_osd);
		pango_layout_set_font_description(layout, desc);
		pango_layout_set_width(layout, req_width * PANGO_SCALE);
		pango_font_description_free(desc);
		pango_layout_set_text(layout, server.workspaces.current->name, -1);
		pango_cairo_show_layout(cairo, layout);
		g_object_unref(layout);
		surface = cairo_get_target(cairo);
		cairo_surface_flush(surface);
		cairo_destroy(cairo);
		if (!output->workspace_osd) {
			output->workspace_osd = lab_wlr_scene_buffer_create(
				&server.scene->tree, NULL);
		}
		struct wlr_box output_box;
		wlr_output_layout_get_box(server.output_layout,
			output->wlr_output, &output_box);
		int lx = output_box.x + (output_box.width - width) / 2;
		int ly = output_box.y + (output_box.height - height) / 2;
		wlr_scene_node_set_position(&output->workspace_osd->node, lx, ly);
		wlr_scene_buffer_set_buffer(output->workspace_osd, &buffer->base);
		wlr_scene_buffer_set_dest_size(output->workspace_osd,
			(int)buffer->logical_width, (int)buffer->logical_height);
		wlr_buffer_drop(&buffer->base);
	}
}
static struct workspace *
LABWC_src_workspaces_c_workspace_find_by_name(const char *name)
{
	struct workspace *workspace;
	size_t parsed_index = LABWC_src_workspaces_c_parse_workspace_index(name);
	if (parsed_index) {
		size_t index = 0;
		wl_list_for_each(workspace, &server.workspaces.all, link) {
			if (parsed_index == ++index) {
				return workspace;
			}
		}
	}
	wl_list_for_each(workspace, &server.workspaces.all, link) {
		if (!strcmp(workspace->name, name)) {
			return workspace;
		}
	}
	return NULL;
}
static void
LABWC_src_workspaces_c_handle_ext_workspace_commit(struct wl_listener *listener, void *data)
{
	struct wlr_ext_workspace_v1_commit_event *event = data;
	struct wlr_ext_workspace_v1_request *req;
	wl_list_for_each(req, event->requests, link) {
		if (req->type == WLR_EXT_WORKSPACE_V1_REQUEST_ACTIVATE) {
			struct workspace *workspace = req->activate.workspace->data;
			workspaces_switch_to(workspace,   true);
		}
	}
}
static void
LABWC_src_workspaces_c_add_workspace(const char *name)
{
	struct workspace *workspace = znew(*workspace);
	workspace->name = xstrdup(name);
	workspace->tree = lab_wlr_scene_tree_create(server.workspace_tree);
	workspace->view_trees[VIEW_LAYER_ALWAYS_ON_BOTTOM] =
		lab_wlr_scene_tree_create(workspace->tree);
	workspace->view_trees[VIEW_LAYER_NORMAL] =
		lab_wlr_scene_tree_create(workspace->tree);
	workspace->view_trees[VIEW_LAYER_ALWAYS_ON_TOP] =
		lab_wlr_scene_tree_create(workspace->tree);
	wl_list_append(&server.workspaces.all, &workspace->link);
	wlr_scene_node_set_enabled(&workspace->tree->node, false);
	workspace->ext_workspace = wlr_ext_workspace_handle_v1_create(
		server.workspaces.ext_manager,   NULL,
		EXT_WORKSPACE_HANDLE_V1_WORKSPACE_CAPABILITIES_ACTIVATE);
	workspace->ext_workspace->data = workspace;
	wlr_ext_workspace_handle_v1_set_group(
		workspace->ext_workspace, server.workspaces.ext_group);
	wlr_ext_workspace_handle_v1_set_name(workspace->ext_workspace, name);
}
static struct workspace *
LABWC_src_workspaces_c_get_prev(struct workspace *current, struct wl_list *workspaces, bool wrap)
{
	struct wl_list *target_link = current->link.prev;
	if (target_link == workspaces) {
		if (!wrap) {
			return NULL;
		}
		target_link = target_link->prev;
	}
	return wl_container_of(target_link, current, link);
}
static struct workspace *
LABWC_src_workspaces_c_get_next(struct workspace *current, struct wl_list *workspaces, bool wrap)
{
	struct wl_list *target_link = current->link.next;
	if (target_link == workspaces) {
		if (!wrap) {
			return NULL;
		}
		target_link = target_link->next;
	}
	return wl_container_of(target_link, current, link);
}
static bool
LABWC_src_workspaces_c_workspace_has_views(struct workspace *workspace)
{
	struct view *view;
	for_each_view(view, &server.views, LAB_VIEW_CRITERIA_NO_OMNIPRESENT) {
		if (view->workspace == workspace) {
			return true;
		}
	}
	return false;
}
static struct workspace *
LABWC_src_workspaces_c_get_adjacent_occupied(struct workspace *current, struct wl_list *workspaces,
		bool wrap, bool reverse)
{
	struct wl_list *start = &current->link;
	struct wl_list *link = reverse ? start->prev : start->next;
	bool has_wrapped = false;
	while (true) {
		if (link == workspaces) {
			if (!wrap) {
				break;
			}
			if (has_wrapped) {
				break;
			}
			link = reverse ? workspaces->prev : workspaces->next;
			has_wrapped = true;
			continue;
		}
		struct workspace *target = wl_container_of(link, target, link);
		if (link == start) {
			break;
		}
		if (target != current && LABWC_src_workspaces_c_workspace_has_views(target)) {
			return target;
		}
		link = reverse ? link->prev : link->next;
	}
	return NULL;
}
static struct workspace *
LABWC_src_workspaces_c_get_prev_occupied(struct workspace *current, struct wl_list *workspaces, bool wrap)
{
	return LABWC_src_workspaces_c_get_adjacent_occupied(current, workspaces, wrap, true);
}
static struct workspace *
LABWC_src_workspaces_c_get_next_occupied(struct workspace *current, struct wl_list *workspaces, bool wrap)
{
	return LABWC_src_workspaces_c_get_adjacent_occupied(current, workspaces, wrap, false);
}
static int
LABWC_src_workspaces_c__osd_handle_timeout(void *data)
{
	struct seat *seat = data;
	workspaces_osd_hide(seat);
	return 0;
}
static void
LABWC_src_workspaces_c__osd_show(void)
{
	if (!rc.workspace_config.popuptime) {
		return;
	}
	LABWC_src_workspaces_c__osd_update();
	struct output *output;
	wl_list_for_each(output, &server.outputs, link) {
		if (output_is_usable(output) && output->workspace_osd) {
			wlr_scene_node_set_enabled(&output->workspace_osd->node, true);
		}
	}
	if (keyboard_get_all_modifiers(&server.seat)) {
		server.seat.workspace_osd_shown_by_modifier = true;
	} else {
		if (!server.seat.workspace_osd_timer) {
			server.seat.workspace_osd_timer = wl_event_loop_add_timer(
				server.wl_event_loop, LABWC_src_workspaces_c__osd_handle_timeout, &server.seat);
		}
		wl_event_source_timer_update(server.seat.workspace_osd_timer,
			rc.workspace_config.popuptime);
	}
}
void
workspaces_init(void)
{
	server.workspaces.ext_manager = wlr_ext_workspace_manager_v1_create(
		server.wl_display, EXT_WORKSPACES_VERSION);
	server.workspaces.ext_group = wlr_ext_workspace_group_handle_v1_create(
		server.workspaces.ext_manager,   0);
	server.workspaces.on_ext_manager.commit.notify = LABWC_src_workspaces_c_handle_ext_workspace_commit;
	wl_signal_add(&server.workspaces.ext_manager->events.commit,
		&server.workspaces.on_ext_manager.commit);
	wl_list_init(&server.workspaces.all);
	struct workspace_config *conf;
	wl_list_for_each(conf, &rc.workspace_config.workspaces, link) {
		LABWC_src_workspaces_c_add_workspace(conf->name);
	}
	char *initial_name = rc.workspace_config.initial_workspace_name;
	struct workspace *initial = NULL;
	struct workspace *first = wl_container_of(
		server.workspaces.all.next, first, link);
	if (initial_name) {
		initial = LABWC_src_workspaces_c_workspace_find_by_name(initial_name);
	}
	if (!initial) {
		initial = first;
	}
	server.workspaces.current = initial;
	wlr_scene_node_set_enabled(&initial->tree->node, true);
	wlr_ext_workspace_handle_v1_set_active(initial->ext_workspace, true);
}
void
workspaces_switch_to(struct workspace *target, bool update_focus)
{
	assert(target);
	if (target == server.workspaces.current) {
		return;
	}
	wlr_scene_node_set_enabled(
		&server.workspaces.current->tree->node, false);
	wlr_ext_workspace_handle_v1_set_active(
		server.workspaces.current->ext_workspace, false);
	struct view *view;
	wl_list_for_each_reverse(view, &server.views, link) {
		if (view->visible_on_all_workspaces) {
			view_move_to_workspace(view, target);
		}
	}
	wlr_scene_node_set_enabled(&target->tree->node, true);
	server.workspaces.last = server.workspaces.current;
	server.workspaces.current = target;
	struct view *grabbed_view = server.grabbed_view;
	if (grabbed_view) {
		view_move_to_workspace(grabbed_view, target);
	}
	if (update_focus) {
		struct view *active_view = server.active_view;
		if (!(active_view && active_view->visible_on_all_workspaces)) {
			desktop_focus_topmost_view();
		}
	}
	LABWC_src_workspaces_c__osd_show();
	cursor_update_focus();
	desktop_update_top_layer_visibility();
	wlr_ext_workspace_handle_v1_set_active(target->ext_workspace, true);
	show_desktop_reset();
}
void
workspaces_osd_hide(struct seat *seat)
{
	assert(seat);
	struct output *output;
	wl_list_for_each(output, &server.outputs, link) {
		if (!output->workspace_osd) {
			continue;
		}
		wlr_scene_node_set_enabled(&output->workspace_osd->node, false);
		wlr_scene_buffer_set_buffer(output->workspace_osd, NULL);
	}
	seat->workspace_osd_shown_by_modifier = false;
	cursor_update_focus();
}
struct workspace *
workspaces_find(struct workspace *anchor, const char *name, bool wrap)
{
	assert(anchor);
	if (!name) {
		return NULL;
	}
	struct wl_list *workspaces = &server.workspaces.all;
	if (!strcasecmp(name, "current")) {
		return anchor;
	} else if (!strcasecmp(name, "last")) {
		return server.workspaces.last;
	} else if (!strcasecmp(name, "left")) {
		return LABWC_src_workspaces_c_get_prev(anchor, workspaces, wrap);
	} else if (!strcasecmp(name, "right")) {
		return LABWC_src_workspaces_c_get_next(anchor, workspaces, wrap);
	} else if (!strcasecmp(name, "left-occupied")) {
		return LABWC_src_workspaces_c_get_prev_occupied(anchor, workspaces, wrap);
	} else if (!strcasecmp(name, "right-occupied")) {
		return LABWC_src_workspaces_c_get_next_occupied(anchor, workspaces, wrap);
	}
	return LABWC_src_workspaces_c_workspace_find_by_name(name);
}
static void
LABWC_src_workspaces_c_destroy_workspace(struct workspace *workspace)
{
	wlr_scene_node_destroy(&workspace->tree->node);
	zfree(workspace->name);
	wl_list_remove(&workspace->link);
	wlr_ext_workspace_handle_v1_destroy(workspace->ext_workspace);
	free(workspace);
}
void
workspaces_destroy(void)
{
	struct workspace *workspace, *tmp;
	wl_list_for_each_safe(workspace, tmp, &server.workspaces.all, link) {
		LABWC_src_workspaces_c_destroy_workspace(workspace);
	}
	assert(wl_list_empty(&server.workspaces.all));
	wl_list_remove(&server.workspaces.on_ext_manager.commit.link);
}
#undef EXT_WORKSPACES_VERSION
#if HAVE_XWAYLAND
#define handle_associate LABWC_src_xwayland_unmanaged_c_handle_associate
#define handle_destroy LABWC_src_xwayland_unmanaged_c_handle_destroy
#define handle_dissociate LABWC_src_xwayland_unmanaged_c_handle_dissociate
#define handle_grab_focus LABWC_src_xwayland_unmanaged_c_handle_grab_focus
#define handle_request_activate LABWC_src_xwayland_unmanaged_c_handle_request_activate
#define handle_request_configure LABWC_src_xwayland_unmanaged_c_handle_request_configure
#define handle_set_geometry LABWC_src_xwayland_unmanaged_c_handle_set_geometry
#define handle_set_override_redirect LABWC_src_xwayland_unmanaged_c_handle_set_override_redirect
#include <assert.h>
static void
LABWC_src_xwayland_unmanaged_c_handle_grab_focus(struct wl_listener *listener, void *data)
{
	struct xwayland_unmanaged *unmanaged =
		wl_container_of(listener, unmanaged, grab_focus);
	unmanaged->ever_grabbed_focus = true;
	if (unmanaged->node) {
		assert(unmanaged->xwayland_surface->surface);
		seat_focus_surface(&server.seat,
			unmanaged->xwayland_surface->surface);
	}
}
static void
LABWC_src_xwayland_unmanaged_c_handle_request_configure(struct wl_listener *listener, void *data)
{
	struct xwayland_unmanaged *unmanaged =
		wl_container_of(listener, unmanaged, request_configure);
	struct wlr_xwayland_surface *xsurface = unmanaged->xwayland_surface;
	struct wlr_xwayland_surface_configure_event *ev = data;
	wlr_xwayland_surface_configure(xsurface, ev->x, ev->y, ev->width, ev->height);
	if (unmanaged->node) {
		wlr_scene_node_set_position(unmanaged->node, ev->x, ev->y);
		cursor_update_focus();
	}
}
static void
LABWC_src_xwayland_unmanaged_c_handle_set_geometry(struct wl_listener *listener, void *data)
{
	struct xwayland_unmanaged *unmanaged =
		wl_container_of(listener, unmanaged, set_geometry);
	struct wlr_xwayland_surface *xsurface = unmanaged->xwayland_surface;
	if (unmanaged->node) {
		wlr_scene_node_set_position(unmanaged->node, xsurface->x, xsurface->y);
		cursor_update_focus();
	}
}
static void
LABWC_src_xwayland_unmanaged_c_handle_map(struct wl_listener *listener, void *data)
{
	struct xwayland_unmanaged *unmanaged =
		wl_container_of(listener, unmanaged, mappable.map);
	struct wlr_xwayland_surface *xsurface = unmanaged->xwayland_surface;
	assert(!unmanaged->node);
	wl_list_append(&server.unmanaged_surfaces, &unmanaged->link);
	CONNECT_SIGNAL(xsurface, unmanaged, set_geometry);
	if (wlr_xwayland_surface_override_redirect_wants_focus(xsurface)
			|| unmanaged->ever_grabbed_focus) {
		seat_focus_surface(&server.seat, xsurface->surface);
	}
	struct wlr_scene_surface *scene_surface = wlr_scene_surface_create(
		server.unmanaged_tree, xsurface->surface);
	die_if_null(scene_surface);
	unmanaged->node = &scene_surface->buffer->node;
	wlr_scene_node_set_position(unmanaged->node, xsurface->x, xsurface->y);
	cursor_update_focus();
}
static void
LABWC_src_xwayland_unmanaged_c_focus_next_surface(struct wlr_xwayland_surface *xsurface)
{
	struct xwayland_unmanaged *u;
	struct wl_list *list = &server.unmanaged_surfaces;
	wl_list_for_each_reverse(u, list, link) {
		struct wlr_xwayland_surface *prev = u->xwayland_surface;
		if (wlr_xwayland_surface_override_redirect_wants_focus(prev)
				|| u->ever_grabbed_focus) {
			seat_focus_surface(&server.seat, prev->surface);
			return;
		}
	}
	if (server.active_view) {
		seat_focus_surface(&server.seat, server.active_view->surface);
	}
}
static void
LABWC_src_xwayland_unmanaged_c_handle_unmap(struct wl_listener *listener, void *data)
{
	struct xwayland_unmanaged *unmanaged =
		wl_container_of(listener, unmanaged, mappable.unmap);
	struct wlr_xwayland_surface *xsurface = unmanaged->xwayland_surface;
	struct seat *seat = &server.seat;
	assert(unmanaged->node);
	wl_list_remove(&unmanaged->link);
	wl_list_remove(&unmanaged->set_geometry.link);
	wlr_scene_node_destroy(unmanaged->node);
	unmanaged->node = NULL;
	cursor_update_focus();
	if (seat->wlr_seat->keyboard_state.focused_surface == xsurface->surface) {
		LABWC_src_xwayland_unmanaged_c_focus_next_surface(xsurface);
	}
}
static void
LABWC_src_xwayland_unmanaged_c_handle_associate(struct wl_listener *listener, void *data)
{
	struct xwayland_unmanaged *unmanaged =
		wl_container_of(listener, unmanaged, associate);
	assert(unmanaged->xwayland_surface &&
		unmanaged->xwayland_surface->surface);
	mappable_connect(&unmanaged->mappable,
		unmanaged->xwayland_surface->surface,
		LABWC_src_xwayland_unmanaged_c_handle_map, LABWC_src_xwayland_unmanaged_c_handle_unmap);
}
static void
LABWC_src_xwayland_unmanaged_c_handle_dissociate(struct wl_listener *listener, void *data)
{
	struct xwayland_unmanaged *unmanaged =
		wl_container_of(listener, unmanaged, dissociate);
	mappable_disconnect(&unmanaged->mappable);
}
static void
LABWC_src_xwayland_unmanaged_c_handle_destroy(struct wl_listener *listener, void *data)
{
	struct xwayland_unmanaged *unmanaged =
		wl_container_of(listener, unmanaged, destroy);
	if (unmanaged->mappable.connected) {
		mappable_disconnect(&unmanaged->mappable);
	}
	wl_list_remove(&unmanaged->associate.link);
	wl_list_remove(&unmanaged->dissociate.link);
	wl_list_remove(&unmanaged->grab_focus.link);
	wl_list_remove(&unmanaged->request_activate.link);
	wl_list_remove(&unmanaged->request_configure.link);
	wl_list_remove(&unmanaged->set_override_redirect.link);
	wl_list_remove(&unmanaged->destroy.link);
	free(unmanaged);
}
static void
LABWC_src_xwayland_unmanaged_c_handle_set_override_redirect(struct wl_listener *listener, void *data)
{
	struct xwayland_unmanaged *unmanaged =
		wl_container_of(listener, unmanaged, set_override_redirect);
	struct wlr_xwayland_surface *xsurface = unmanaged->xwayland_surface;
	bool mapped = xsurface->surface && xsurface->surface->mapped;
	if (mapped) {
		LABWC_src_xwayland_unmanaged_c_handle_unmap(&unmanaged->mappable.unmap, NULL);
	}
	LABWC_src_xwayland_unmanaged_c_handle_destroy(&unmanaged->destroy, NULL);
	xwayland_view_create(xsurface, mapped);
}
static void
LABWC_src_xwayland_unmanaged_c_handle_request_activate(struct wl_listener *listener, void *data)
{
	struct xwayland_unmanaged *unmanaged =
		wl_container_of(listener, unmanaged, request_activate);
	struct wlr_xwayland_surface *xsurface = unmanaged->xwayland_surface;
	if (!xsurface->surface || !xsurface->surface->mapped) {
		return;
	}
	struct seat *seat = &server.seat;
	struct view *view = server.active_view;
	if (view && view->type == LAB_XWAYLAND_VIEW) {
		struct wlr_xwayland_surface *surf =
			wlr_xwayland_surface_try_from_wlr_surface(view->surface);
		if (surf && surf->pid != xsurface->pid) {
			return;
		}
	}
	seat_focus_surface(seat, xsurface->surface);
}
void
xwayland_unmanaged_create(struct wlr_xwayland_surface *xsurface, bool mapped)
{
	struct xwayland_unmanaged *unmanaged = znew(*unmanaged);
	unmanaged->xwayland_surface = xsurface;
	assert(!xsurface->data);
	CONNECT_SIGNAL(xsurface, unmanaged, associate);
	CONNECT_SIGNAL(xsurface, unmanaged, dissociate);
	CONNECT_SIGNAL(xsurface, unmanaged, destroy);
	CONNECT_SIGNAL(xsurface, unmanaged, grab_focus);
	CONNECT_SIGNAL(xsurface, unmanaged, request_activate);
	CONNECT_SIGNAL(xsurface, unmanaged, request_configure);
	CONNECT_SIGNAL(xsurface, unmanaged, set_override_redirect);
	if (xsurface->surface) {
		LABWC_src_xwayland_unmanaged_c_handle_associate(&unmanaged->associate, NULL);
	}
	if (mapped) {
		LABWC_src_xwayland_unmanaged_c_handle_map(&unmanaged->mappable.map, NULL);
	}
}
#undef handle_associate
#undef handle_destroy
#undef handle_dissociate
#undef handle_grab_focus
#undef handle_request_activate
#undef handle_request_configure
#undef handle_set_geometry
#undef handle_set_override_redirect
#define handle_always_on_top LABWC_src_xwayland_c_handle_always_on_top
#define handle_associate LABWC_src_xwayland_c_handle_associate
#define handle_commit LABWC_src_xwayland_c_handle_commit
#define handle_destroy LABWC_src_xwayland_c_handle_destroy
#define handle_dissociate LABWC_src_xwayland_c_handle_dissociate
#define handle_focus_in LABWC_src_xwayland_c_handle_focus_in
#define handle_request_above LABWC_src_xwayland_c_handle_request_above
#define handle_request_activate LABWC_src_xwayland_c_handle_request_activate
#define handle_request_close LABWC_src_xwayland_c_handle_request_close
#define handle_request_configure LABWC_src_xwayland_c_handle_request_configure
#define handle_request_fullscreen LABWC_src_xwayland_c_handle_request_fullscreen
#define handle_request_maximize LABWC_src_xwayland_c_handle_request_maximize
#define handle_request_minimize LABWC_src_xwayland_c_handle_request_minimize
#define handle_request_move LABWC_src_xwayland_c_handle_request_move
#define handle_request_resize LABWC_src_xwayland_c_handle_request_resize
#define handle_set_class LABWC_src_xwayland_c_handle_set_class
#define handle_set_decorations LABWC_src_xwayland_c_handle_set_decorations
#define handle_set_override_redirect LABWC_src_xwayland_c_handle_set_override_redirect
#define handle_set_strut_partial LABWC_src_xwayland_c_handle_set_strut_partial
#define handle_set_title LABWC_src_xwayland_c_handle_set_title
#define handle_set_window_type LABWC_src_xwayland_c_handle_set_window_type
#include <assert.h>
#include <stdlib.h>
static void LABWC_src_xwayland_c_set_surface(struct view *view, struct wlr_surface *surface);
static void LABWC_src_xwayland_c_handle_map(struct wl_listener *listener, void *data);
static void LABWC_src_xwayland_c_handle_unmap(struct wl_listener *listener, void *data);
static struct xwayland_view *
LABWC_src_xwayland_c_xwayland_view_from_view(struct view *view)
{
	assert(view->type == LAB_XWAYLAND_VIEW);
	return (struct xwayland_view *)view;
}
static struct wlr_xwayland_surface *
LABWC_src_xwayland_c_xwayland_surface_from_view(struct view *view)
{
	struct xwayland_view *xwayland_view = LABWC_src_xwayland_c_xwayland_view_from_view(view);
	assert(xwayland_view->xwayland_surface);
	return xwayland_view->xwayland_surface;
}
static bool
LABWC_src_xwayland_c_xwayland_view_contains_window_type(struct view *view,
		enum lab_window_type window_type)
{
	static_assert(LAB_WINDOW_TYPE_DESKTOP ==
			(int)WLR_XWAYLAND_NET_WM_WINDOW_TYPE_DESKTOP
		&& LAB_WINDOW_TYPE_DOCK ==
			(int)WLR_XWAYLAND_NET_WM_WINDOW_TYPE_DOCK
		&& LAB_WINDOW_TYPE_TOOLBAR ==
			(int)WLR_XWAYLAND_NET_WM_WINDOW_TYPE_TOOLBAR
		&& LAB_WINDOW_TYPE_MENU ==
			(int)WLR_XWAYLAND_NET_WM_WINDOW_TYPE_MENU
		&& LAB_WINDOW_TYPE_UTILITY ==
			(int)WLR_XWAYLAND_NET_WM_WINDOW_TYPE_UTILITY
		&& LAB_WINDOW_TYPE_SPLASH ==
			(int)WLR_XWAYLAND_NET_WM_WINDOW_TYPE_SPLASH
		&& LAB_WINDOW_TYPE_DIALOG ==
			(int)WLR_XWAYLAND_NET_WM_WINDOW_TYPE_DIALOG
		&& LAB_WINDOW_TYPE_DROPDOWN_MENU ==
			(int)WLR_XWAYLAND_NET_WM_WINDOW_TYPE_DROPDOWN_MENU
		&& LAB_WINDOW_TYPE_POPUP_MENU ==
			(int)WLR_XWAYLAND_NET_WM_WINDOW_TYPE_POPUP_MENU
		&& LAB_WINDOW_TYPE_TOOLTIP ==
			(int)WLR_XWAYLAND_NET_WM_WINDOW_TYPE_TOOLTIP
		&& LAB_WINDOW_TYPE_NOTIFICATION ==
			(int)WLR_XWAYLAND_NET_WM_WINDOW_TYPE_NOTIFICATION
		&& LAB_WINDOW_TYPE_COMBO ==
			(int)WLR_XWAYLAND_NET_WM_WINDOW_TYPE_COMBO
		&& LAB_WINDOW_TYPE_DND ==
			(int)WLR_XWAYLAND_NET_WM_WINDOW_TYPE_DND
		&& LAB_WINDOW_TYPE_NORMAL ==
			(int)WLR_XWAYLAND_NET_WM_WINDOW_TYPE_NORMAL
		&& LAB_WINDOW_TYPE_LEN ==
			(int)WLR_XWAYLAND_NET_WM_WINDOW_TYPE_NORMAL + 1,
		"lab_window_type does not match wlr_xwayland_net_wm_window_type");
	assert(view);
	struct wlr_xwayland_surface *surface = LABWC_src_xwayland_c_xwayland_surface_from_view(view);
	return wlr_xwayland_surface_has_window_type(surface,
		(enum wlr_xwayland_net_wm_window_type)window_type);
}
static struct view_size_hints
LABWC_src_xwayland_c_xwayland_view_get_size_hints(struct view *view)
{
	xcb_size_hints_t *hints = LABWC_src_xwayland_c_xwayland_surface_from_view(view)->size_hints;
	if (!hints) {
		return (struct view_size_hints){0};
	}
	return (struct view_size_hints){
		.min_width = hints->min_width,
		.min_height = hints->min_height,
		.width_inc = hints->width_inc,
		.height_inc = hints->height_inc,
		.base_width = hints->base_width,
		.base_height = hints->base_height,
	};
}
static enum view_wants_focus
LABWC_src_xwayland_c_xwayland_view_wants_focus(struct view *view)
{
	struct wlr_xwayland_surface *xsurface =
		LABWC_src_xwayland_c_xwayland_surface_from_view(view);
	switch (wlr_xwayland_surface_icccm_input_model(xsurface)) {
	case WLR_ICCCM_INPUT_MODEL_PASSIVE:
	case WLR_ICCCM_INPUT_MODEL_LOCAL:
		return VIEW_WANTS_FOCUS_ALWAYS;
	case WLR_ICCCM_INPUT_MODEL_GLOBAL:
		return (wlr_xwayland_surface_has_window_type(xsurface,
				WLR_XWAYLAND_NET_WM_WINDOW_TYPE_NORMAL)
			|| wlr_xwayland_surface_has_window_type(xsurface,
				WLR_XWAYLAND_NET_WM_WINDOW_TYPE_DIALOG)) ?
			VIEW_WANTS_FOCUS_LIKELY : VIEW_WANTS_FOCUS_UNLIKELY;
	case WLR_ICCCM_INPUT_MODEL_NONE:
		break;
	}
	return VIEW_WANTS_FOCUS_NEVER;
}
static bool
LABWC_src_xwayland_c_xwayland_view_has_strut_partial(struct view *view)
{
	struct wlr_xwayland_surface *xsurface =
		LABWC_src_xwayland_c_xwayland_surface_from_view(view);
	return (bool)xsurface->strut_partial;
}
static void
LABWC_src_xwayland_c_xwayland_view_offer_focus(struct view *view)
{
	wlr_xwayland_surface_offer_focus(LABWC_src_xwayland_c_xwayland_surface_from_view(view));
}
static struct view *
LABWC_src_xwayland_c_xwayland_view_get_parent(struct view *view)
{
	struct wlr_xwayland_surface *xsurface = LABWC_src_xwayland_c_xwayland_surface_from_view(view);
	return xsurface->parent ? (struct view *)xsurface->parent->data : NULL;
}
static struct wlr_xwayland_surface *
LABWC_src_xwayland_c_top_parent_of(struct view *view)
{
	struct wlr_xwayland_surface *s = LABWC_src_xwayland_c_xwayland_surface_from_view(view);
	while (s->parent) {
		s = s->parent;
	}
	return s;
}
static bool
LABWC_src_xwayland_c_want_deco(struct wlr_xwayland_surface *xwayland_surface)
{
	struct view *view = (struct view *)xwayland_surface->data;
	switch (window_rules_get_property(view, "serverDecoration")) {
	case LAB_PROP_TRUE:
		return true;
	case LAB_PROP_FALSE:
		return false;
	default:
		break;
	}
	return xwayland_surface->decorations ==
		WLR_XWAYLAND_SURFACE_DECORATIONS_ALL;
}
static void
LABWC_src_xwayland_c_set_output_from_pending_geometry(struct view *view)
{
	view_set_output(view, output_nearest_to(
		view->pending.x + view->pending.width / 2,
		view->pending.y + view->pending.height / 2));
}
static void
LABWC_src_xwayland_c_ensure_initial_geometry_and_output(struct view *view)
{
	struct xwayland_view *xwayland_view = LABWC_src_xwayland_c_xwayland_view_from_view(view);
	if (xwayland_view->initial_geometry_set) {
		if (!output_is_usable(view->output)) {
			LABWC_src_xwayland_c_set_output_from_pending_geometry(view);
		}
		return;
	}
	xwayland_view->initial_geometry_set = true;
	struct wlr_xwayland_surface *xsurface = LABWC_src_xwayland_c_xwayland_surface_from_view(view);
	if (wlr_box_empty(&view->pending)) {
		view->pending.x = xsurface->x;
		view->pending.y = xsurface->y;
		view->pending.width = xsurface->width;
		view->pending.height = xsurface->height;
	}
	bool has_position = xsurface->size_hints && (xsurface->size_hints->flags
		& (XCB_ICCCM_SIZE_HINT_US_POSITION | XCB_ICCCM_SIZE_HINT_P_POSITION));
	if (has_position) {
		LABWC_src_xwayland_c_set_output_from_pending_geometry(view);
	} else {
		view_set_output(view, output_nearest_to_cursor());
	}
	if (LABWC_src_xwayland_c_want_deco(xsurface)) {
		view_set_ssd_mode(view, LAB_SSD_MODE_FULL);
	} else {
		view_set_ssd_mode(view, LAB_SSD_MODE_NONE);
	}
	view_constrain_size_to_that_of_usable_area(view);
	if (!has_position) {
		view_place_by_policy(view,   true,
			rc.placement_policy);
	}
}
static void
LABWC_src_xwayland_c_handle_commit(struct wl_listener *listener, void *data)
{
	struct view *view = wl_container_of(listener, view, commit);
	assert(!data || data == view->surface);
	assert(view->surface);
	struct wlr_surface_state *state = &view->surface->current;
	struct wlr_box *current = &view->current;
	if (current->width != state->width || current->height != state->height) {
		view_impl_apply_geometry(view, state->width, state->height);
		view_moved(view);
	}
}
static void
LABWC_src_xwayland_c_handle_request_move(struct wl_listener *listener, void *data)
{
	struct view *view = wl_container_of(listener, view, request_move);
	interactive_begin(view, LAB_INPUT_STATE_MOVE, LAB_EDGE_NONE);
}
static void
LABWC_src_xwayland_c_handle_request_resize(struct wl_listener *listener, void *data)
{
	struct wlr_xwayland_resize_event *event = data;
	struct view *view = wl_container_of(listener, view, request_resize);
	interactive_begin(view, LAB_INPUT_STATE_RESIZE, event->edges);
}
static void
LABWC_src_xwayland_c_handle_associate(struct wl_listener *listener, void *data)
{
	struct xwayland_view *xwayland_view =
		wl_container_of(listener, xwayland_view, associate);
	struct view *view = &xwayland_view->base;
	struct wlr_xwayland_surface *xsurface = xwayland_view->xwayland_surface;
	assert(xsurface && xsurface->surface);
	LABWC_src_xwayland_c_set_surface(view, xsurface->surface);
	LABWC_src_xwayland_c_ensure_initial_geometry_and_output(view);
	view_set_fullscreen(view, xsurface->fullscreen);
	if (!view->been_mapped) {
		if (LABWC_src_xwayland_c_want_deco(xsurface)) {
			view_set_ssd_mode(view, LAB_SSD_MODE_FULL);
		} else {
			view_set_ssd_mode(view, LAB_SSD_MODE_NONE);
		}
	}
	enum view_axis axis = VIEW_AXIS_NONE;
	if (xsurface->maximized_horz) {
		axis |= VIEW_AXIS_HORIZONTAL;
	}
	if (xsurface->maximized_vert) {
		axis |= VIEW_AXIS_VERTICAL;
	}
	view_maximize(view, axis);
	if (window_rules_get_property(view, "allowAlwaysOnTop") == LAB_PROP_TRUE) {
		view_set_layer(view, xsurface->above
		? VIEW_LAYER_ALWAYS_ON_TOP : VIEW_LAYER_NORMAL);
	}
}
static void
LABWC_src_xwayland_c_handle_dissociate(struct wl_listener *listener, void *data)
{
	struct xwayland_view *xwayland_view =
		wl_container_of(listener, xwayland_view, dissociate);
	LABWC_src_xwayland_c_set_surface(&xwayland_view->base, NULL);
}
static void
LABWC_src_xwayland_c_handle_destroy(struct wl_listener *listener, void *data)
{
	struct view *view = wl_container_of(listener, view, destroy);
	struct xwayland_view *xwayland_view = LABWC_src_xwayland_c_xwayland_view_from_view(view);
	assert(xwayland_view->xwayland_surface->data == view);
	LABWC_src_xwayland_c_set_surface(view, NULL);
	xwayland_view->xwayland_surface->data = NULL;
	xwayland_view->xwayland_surface = NULL;
	wl_list_remove(&xwayland_view->associate.link);
	wl_list_remove(&xwayland_view->dissociate.link);
	wl_list_remove(&xwayland_view->request_above.link);
	wl_list_remove(&xwayland_view->request_activate.link);
	wl_list_remove(&xwayland_view->request_close.link);
	wl_list_remove(&xwayland_view->request_configure.link);
	wl_list_remove(&xwayland_view->set_class.link);
	wl_list_remove(&xwayland_view->set_decorations.link);
	wl_list_remove(&xwayland_view->set_override_redirect.link);
	wl_list_remove(&xwayland_view->set_strut_partial.link);
	wl_list_remove(&xwayland_view->set_window_type.link);
	wl_list_remove(&xwayland_view->focus_in.link);
	wl_list_remove(&xwayland_view->on_view.always_on_top.link);
	view_destroy(view);
}
static void
LABWC_src_xwayland_c_xwayland_view_configure(struct view *view, struct wlr_box geo)
{
	view->pending = geo;
	wlr_xwayland_surface_configure(LABWC_src_xwayland_c_xwayland_surface_from_view(view),
		(int16_t)geo.x, (int16_t)geo.y,
		(uint16_t)geo.width, (uint16_t)geo.height);
	bool is_offscreen = !wlr_box_empty(&view->current) &&
		!wlr_output_layout_intersects(server.output_layout, NULL,
			&view->current);
	if (is_offscreen || (view->current.width == geo.width
			&& view->current.height == geo.height)) {
		view->current.x = geo.x;
		view->current.y = geo.y;
		view_moved(view);
	}
}
static void
LABWC_src_xwayland_c_handle_request_configure(struct wl_listener *listener, void *data)
{
	struct xwayland_view *xwayland_view =
		wl_container_of(listener, xwayland_view, request_configure);
	struct view *view = &xwayland_view->base;
	struct wlr_xwayland_surface_configure_event *event = data;
	bool ignore_configure_requests = window_rules_get_property(
		view, "ignoreConfigureRequest") == LAB_PROP_TRUE;
	if (view_is_floating(view) && !ignore_configure_requests) {
		struct wlr_box box = {.x = event->x, .y = event->y,
			.width = event->width, .height = event->height};
		view_adjust_size(view, &box.width, &box.height);
		LABWC_src_xwayland_c_xwayland_view_configure(view, box);
	} else {
		LABWC_src_xwayland_c_xwayland_view_configure(view, view->pending);
	}
}
static void
LABWC_src_xwayland_c_handle_request_above(struct wl_listener *listener, void *data)
{
	struct xwayland_view *xwayland_view =
		wl_container_of(listener, xwayland_view, request_above);
	struct view *view = &xwayland_view->base;
	if (window_rules_get_property(view, "allowAlwaysOnTop") != LAB_PROP_TRUE) {
		return;
	}
	view_set_layer(view, xwayland_view->xwayland_surface->above
		? VIEW_LAYER_ALWAYS_ON_TOP : VIEW_LAYER_NORMAL);
}
static void
LABWC_src_xwayland_c_handle_request_activate(struct wl_listener *listener, void *data)
{
	struct xwayland_view *xwayland_view =
		wl_container_of(listener, xwayland_view, request_activate);
	struct view *view = &xwayland_view->base;
	if (window_rules_get_property(view, "ignoreFocusRequest") == LAB_PROP_TRUE) {
		return;
	}
	desktop_focus_view(view,   true);
}
static void
LABWC_src_xwayland_c_handle_request_close(struct wl_listener *listener, void *data)
{
	struct xwayland_view *xwayland_view =
		wl_container_of(listener, xwayland_view, request_close);
	view_close(&xwayland_view->base);
}
static void
LABWC_src_xwayland_c_handle_request_minimize(struct wl_listener *listener, void *data)
{
	struct wlr_xwayland_minimize_event *event = data;
	struct view *view = wl_container_of(listener, view, request_minimize);
	view_minimize(view, event->minimize);
}
static void
LABWC_src_xwayland_c_handle_request_maximize(struct wl_listener *listener, void *data)
{
	struct view *view = wl_container_of(listener, view, request_maximize);
	struct wlr_xwayland_surface *surf = LABWC_src_xwayland_c_xwayland_surface_from_view(view);
	if (!view->mapped) {
		LABWC_src_xwayland_c_ensure_initial_geometry_and_output(view);
	}
	enum view_axis maximize = VIEW_AXIS_NONE;
	if (surf->maximized_vert) {
		maximize |= VIEW_AXIS_VERTICAL;
	}
	if (surf->maximized_horz) {
		maximize |= VIEW_AXIS_HORIZONTAL;
	}
	view_maximize(view, maximize);
}
static void
LABWC_src_xwayland_c_handle_request_fullscreen(struct wl_listener *listener, void *data)
{
	struct view *view = wl_container_of(listener, view, request_fullscreen);
	bool fullscreen = LABWC_src_xwayland_c_xwayland_surface_from_view(view)->fullscreen;
	if (!view->mapped) {
		LABWC_src_xwayland_c_ensure_initial_geometry_and_output(view);
	}
	view_set_fullscreen(view, fullscreen);
}
static void
LABWC_src_xwayland_c_handle_set_title(struct wl_listener *listener, void *data)
{
	struct view *view = wl_container_of(listener, view, set_title);
	struct xwayland_view *xwayland_view = LABWC_src_xwayland_c_xwayland_view_from_view(view);
	view_set_title(view, xwayland_view->xwayland_surface->title);
}
static void
LABWC_src_xwayland_c_handle_set_class(struct wl_listener *listener, void *data)
{
	struct xwayland_view *xwayland_view =
		wl_container_of(listener, xwayland_view, set_class);
	struct view *view = &xwayland_view->base;
	view_set_app_id(view, xwayland_view->xwayland_surface->class);
}
static void
LABWC_src_xwayland_c_xwayland_view_close(struct view *view)
{
	wlr_xwayland_surface_close(LABWC_src_xwayland_c_xwayland_surface_from_view(view));
}
static void
LABWC_src_xwayland_c_handle_set_decorations(struct wl_listener *listener, void *data)
{
	struct xwayland_view *xwayland_view =
		wl_container_of(listener, xwayland_view, set_decorations);
	struct view *view = &xwayland_view->base;
	if (LABWC_src_xwayland_c_want_deco(xwayland_view->xwayland_surface)) {
		view_set_ssd_mode(view, LAB_SSD_MODE_FULL);
	} else {
		view_set_ssd_mode(view, LAB_SSD_MODE_NONE);
	}
}
static void
LABWC_src_xwayland_c_handle_set_window_type(struct wl_listener *listener, void *data)
{
}
static void
LABWC_src_xwayland_c_handle_set_override_redirect(struct wl_listener *listener, void *data)
{
	struct xwayland_view *xwayland_view =
		wl_container_of(listener, xwayland_view, set_override_redirect);
	struct view *view = &xwayland_view->base;
	struct wlr_xwayland_surface *xsurface = xwayland_view->xwayland_surface;
	bool mapped = xsurface->surface && xsurface->surface->mapped;
	if (mapped) {
		LABWC_src_xwayland_c_handle_unmap(&view->mappable.unmap, NULL);
	}
	LABWC_src_xwayland_c_handle_destroy(&view->destroy, xsurface);
	xwayland_unmanaged_create(xsurface, mapped);
}
static void
LABWC_src_xwayland_c_handle_set_strut_partial(struct wl_listener *listener, void *data)
{
	struct xwayland_view *xwayland_view =
		wl_container_of(listener, xwayland_view, set_strut_partial);
	struct view *view = &xwayland_view->base;
	if (view->mapped) {
		output_update_all_usable_areas(false);
	}
}
static void
LABWC_src_xwayland_c_handle_focus_in(struct wl_listener *listener, void *data)
{
	struct xwayland_view *xwayland_view =
		wl_container_of(listener, xwayland_view, focus_in);
	struct view *view = &xwayland_view->base;
	struct seat *seat = &server.seat;
	if (!view->surface) {
		xwayland_view->focused_before_map = true;
		return;
	}
	if (view->surface != seat->wlr_seat->keyboard_state.focused_surface) {
		seat_focus_surface(seat, view->surface);
	}
}
static void
LABWC_src_xwayland_c_check_natural_geometry(struct view *view)
{
	if (!view_is_floating(view)
			&& (view->natural_geometry.width < LAB_MIN_VIEW_WIDTH
			|| view->natural_geometry.height < LAB_MIN_VIEW_HEIGHT)) {
		view->natural_geometry = view_get_fallback_natural_geometry(view);
	}
}
static void
LABWC_src_xwayland_c_set_surface(struct view *view, struct wlr_surface *surface)
{
	if (view->surface) {
		mappable_disconnect(&view->mappable);
		wl_list_remove(&view->commit.link);
	}
	view->surface = surface;
	if (surface) {
		mappable_connect(&view->mappable, surface,
			LABWC_src_xwayland_c_handle_map, LABWC_src_xwayland_c_handle_unmap);
		CONNECT_SIGNAL(surface, view, commit);
	}
}
static void
LABWC_src_xwayland_c_handle_map(struct wl_listener *listener, void *data)
{
	struct view *view = wl_container_of(listener, view, mappable.map);
	struct xwayland_view *xwayland_view = LABWC_src_xwayland_c_xwayland_view_from_view(view);
	assert(view->surface);
	if (view->mapped) {
		return;
	}
	view->mapped = true;
	if (!view->content_tree) {
		view->content_tree = wlr_scene_subsurface_tree_create(
			view->scene_tree, view->surface);
		die_if_null(view->content_tree);
		wlr_scene_subsurface_tree_create(&view->capture.scene->tree, view->surface);
	}
	wlr_scene_node_set_enabled(&view->content_tree->node, !view->shaded);
	if (!view->been_mapped) {
		LABWC_src_xwayland_c_check_natural_geometry(view);
		view_save_last_placement(view);
		view_adjust_for_layout_change(view);
		view->current = view->pending;
		view_moved(view);
	}
	if (xwayland_view->focused_before_map) {
		xwayland_view->focused_before_map = false;
		seat_focus_surface(&server.seat, view->surface);
	}
	view_impl_map(view);
	view->been_mapped = true;
}
static void
LABWC_src_xwayland_c_handle_unmap(struct wl_listener *listener, void *data)
{
	struct view *view = wl_container_of(listener, view, mappable.unmap);
	if (!view->mapped) {
		return;
	}
	view->mapped = false;
	view_impl_unmap(view);
	if (view->content_tree) {
		wlr_scene_node_destroy(&view->content_tree->node);
		view->content_tree = NULL;
	}
}
static void
LABWC_src_xwayland_c_xwayland_view_maximize(struct view *view, enum view_axis maximized)
{
	wlr_xwayland_surface_set_maximized(LABWC_src_xwayland_c_xwayland_surface_from_view(view),
		maximized & VIEW_AXIS_HORIZONTAL, maximized & VIEW_AXIS_VERTICAL);
}
static void
LABWC_src_xwayland_c_xwayland_view_minimize(struct view *view, bool minimized)
{
	wlr_xwayland_surface_set_minimized(LABWC_src_xwayland_c_xwayland_surface_from_view(view),
		minimized);
}
static struct view *
LABWC_src_xwayland_c_xwayland_view_get_root(struct view *view)
{
	struct wlr_xwayland_surface *root = LABWC_src_xwayland_c_top_parent_of(view);
	return (root && root->data) ? (struct view *)root->data : view;
}
static void
LABWC_src_xwayland_c_xwayland_view_append_children(struct view *self, struct wl_array *children)
{
	struct wlr_xwayland_surface *surface = LABWC_src_xwayland_c_xwayland_surface_from_view(self);
	struct view *view;
	wl_list_for_each_reverse(view, &server.views, link) {
		if (view == self) {
			continue;
		}
		if (view->type != LAB_XWAYLAND_VIEW) {
			continue;
		}
		if (!view->surface) {
			continue;
		}
		if (!view->mapped) {
			continue;
		}
		if (LABWC_src_xwayland_c_top_parent_of(view) != surface) {
			continue;
		}
		struct view **child = wl_array_add(children, sizeof(*child));
		*child = view;
	}
}
static bool
LABWC_src_xwayland_c_xwayland_view_is_modal_dialog(struct view *self)
{
	return LABWC_src_xwayland_c_xwayland_surface_from_view(self)->modal;
}
static void
LABWC_src_xwayland_c_xwayland_view_set_activated(struct view *view, bool activated)
{
	struct wlr_xwayland_surface *xwayland_surface =
		LABWC_src_xwayland_c_xwayland_surface_from_view(view);
	if (activated && xwayland_surface->minimized) {
		wlr_xwayland_surface_set_minimized(xwayland_surface, false);
	}
	wlr_xwayland_surface_activate(xwayland_surface, activated);
	xwayland_flush();
}
static void
LABWC_src_xwayland_c_xwayland_view_set_fullscreen(struct view *view, bool fullscreen)
{
	wlr_xwayland_surface_set_fullscreen(LABWC_src_xwayland_c_xwayland_surface_from_view(view),
		fullscreen);
}
static pid_t
LABWC_src_xwayland_c_xwayland_view_get_pid(struct view *view)
{
	assert(view);
	struct wlr_xwayland_surface *xwayland_surface =
		LABWC_src_xwayland_c_xwayland_surface_from_view(view);
	if (!xwayland_surface) {
		return -1;
	}
	return xwayland_surface->pid;
}
static const struct view_impl LABWC_src_xwayland_c_xwayland_view_impl = {
	.configure = LABWC_src_xwayland_c_xwayland_view_configure,
	.close = LABWC_src_xwayland_c_xwayland_view_close,
	.set_activated = LABWC_src_xwayland_c_xwayland_view_set_activated,
	.set_fullscreen = LABWC_src_xwayland_c_xwayland_view_set_fullscreen,
	.maximize = LABWC_src_xwayland_c_xwayland_view_maximize,
	.minimize = LABWC_src_xwayland_c_xwayland_view_minimize,
	.get_parent = LABWC_src_xwayland_c_xwayland_view_get_parent,
	.get_root = LABWC_src_xwayland_c_xwayland_view_get_root,
	.append_children = LABWC_src_xwayland_c_xwayland_view_append_children,
	.is_modal_dialog = LABWC_src_xwayland_c_xwayland_view_is_modal_dialog,
	.get_size_hints = LABWC_src_xwayland_c_xwayland_view_get_size_hints,
	.wants_focus = LABWC_src_xwayland_c_xwayland_view_wants_focus,
	.offer_focus = LABWC_src_xwayland_c_xwayland_view_offer_focus,
	.has_strut_partial = LABWC_src_xwayland_c_xwayland_view_has_strut_partial,
	.contains_window_type = LABWC_src_xwayland_c_xwayland_view_contains_window_type,
	.get_pid = LABWC_src_xwayland_c_xwayland_view_get_pid,
};
static void
LABWC_src_xwayland_c_handle_always_on_top(struct wl_listener *listener, void *data)
{
	struct xwayland_view *xwayland_view =
		wl_container_of(listener, xwayland_view, on_view.always_on_top);
	wlr_xwayland_surface_set_above(xwayland_view->xwayland_surface,
		xwayland_view->base.layer == VIEW_LAYER_ALWAYS_ON_TOP);
}
void
xwayland_view_create(struct wlr_xwayland_surface *xsurface, bool mapped)
{
	struct xwayland_view *xwayland_view = znew(*xwayland_view);
	struct view *view = &xwayland_view->base;
	view->type = LAB_XWAYLAND_VIEW;
	view->impl = &LABWC_src_xwayland_c_xwayland_view_impl;
	view_init(view);
	xwayland_view->xwayland_surface = xsurface;
	xsurface->data = view;
	view->workspace = server.workspaces.current;
	view->scene_tree = lab_wlr_scene_tree_create(
		view->workspace->view_trees[VIEW_LAYER_NORMAL]);
	wlr_scene_node_set_enabled(&view->scene_tree->node, false);
	node_descriptor_create(&view->scene_tree->node,
		LAB_NODE_VIEW, view,   NULL);
	CONNECT_SIGNAL(xsurface, view, destroy);
	CONNECT_SIGNAL(xsurface, view, request_minimize);
	CONNECT_SIGNAL(xsurface, view, request_maximize);
	CONNECT_SIGNAL(xsurface, view, request_fullscreen);
	CONNECT_SIGNAL(xsurface, view, request_move);
	CONNECT_SIGNAL(xsurface, view, request_resize);
	CONNECT_SIGNAL(xsurface, view, set_title);
	CONNECT_SIGNAL(xsurface, xwayland_view, associate);
	CONNECT_SIGNAL(xsurface, xwayland_view, dissociate);
	CONNECT_SIGNAL(xsurface, xwayland_view, request_above);
	CONNECT_SIGNAL(xsurface, xwayland_view, request_activate);
	CONNECT_SIGNAL(xsurface, xwayland_view, request_close);
	CONNECT_SIGNAL(xsurface, xwayland_view, request_configure);
	CONNECT_SIGNAL(xsurface, xwayland_view, set_class);
	CONNECT_SIGNAL(xsurface, xwayland_view, set_decorations);
	CONNECT_SIGNAL(xsurface, xwayland_view, set_override_redirect);
	CONNECT_SIGNAL(xsurface, xwayland_view, set_strut_partial);
	CONNECT_SIGNAL(xsurface, xwayland_view, set_window_type);
	CONNECT_SIGNAL(xsurface, xwayland_view, focus_in);
	CONNECT_SIGNAL(view, &xwayland_view->on_view, always_on_top);
	wl_list_insert(&server.views, &view->link);
	view->creation_id = server.next_view_creation_id++;
	if (xsurface->surface) {
		LABWC_src_xwayland_c_handle_associate(&xwayland_view->associate, NULL);
		LABWC_src_xwayland_c_handle_set_title(&view->set_title, NULL);
		LABWC_src_xwayland_c_handle_set_class(&xwayland_view->set_class, NULL);
	}
	if (mapped) {
		LABWC_src_xwayland_c_handle_map(&xwayland_view->base.mappable.map, NULL);
	}
}
static void
LABWC_src_xwayland_c_handle_new_surface(struct wl_listener *listener, void *data)
{
	struct wlr_xwayland_surface *xsurface = data;
	if (xsurface->override_redirect) {
		xwayland_unmanaged_create(xsurface,   false);
	} else {
		xwayland_view_create(xsurface,   false);
	}
}
static void
LABWC_src_xwayland_c_handle_xwm_ready(struct wl_listener *listener, void *data)
{
	wlr_xwayland_set_seat(server.xwayland, server.seat.wlr_seat);
	xwayland_update_workarea();
}
void
xwayland_server_init(struct wlr_compositor *compositor)
{
	server.xwayland =
		wlr_xwayland_create(server.wl_display,
			compositor,   !rc.xwayland_persistence);
	if (!server.xwayland) {
		unsetenv("DISPLAY");
		return;
	}
	server.xwayland_new_surface.notify = LABWC_src_xwayland_c_handle_new_surface;
	wl_signal_add(&server.xwayland->events.new_surface,
		&server.xwayland_new_surface);
	server.xwayland_xwm_ready.notify = LABWC_src_xwayland_c_handle_xwm_ready;
	wl_signal_add(&server.xwayland->events.ready,
		&server.xwayland_xwm_ready);
	if (setenv("DISPLAY", server.xwayland->display_name, true) < 0) {
	} else {
	}
	struct wlr_xcursor *xcursor;
	xcursor = wlr_xcursor_manager_get_xcursor(
		server.seat.xcursor_manager, XCURSOR_DEFAULT, 1);
	if (xcursor) {
		struct wlr_xcursor_image *image = xcursor->images[0];
		struct wlr_buffer *cursor_buffer = wlr_xcursor_image_get_buffer(image);
		if (cursor_buffer) {
			wlr_xwayland_set_cursor(server.xwayland, cursor_buffer,
				(int32_t)image->hotspot_x, (int32_t)image->hotspot_y);
		}
	}
}
void
xwayland_server_finish(void)
{
	struct wlr_xwayland *xwayland = server.xwayland;
	if (!xwayland) {
		return;
	}
	wl_list_remove(&server.xwayland_new_surface.link);
	wl_list_remove(&server.xwayland_xwm_ready.link);
	server.xwayland = NULL;
	wlr_xwayland_destroy(xwayland);
}
static bool
LABWC_src_xwayland_c_intervals_overlap(int start_a, int end_a, int start_b, int end_b)
{
	if (end_a <= start_a || end_b <= start_b) {
		return false;
	}
	return start_a < start_b ?
		start_b < end_a :
		start_a < end_b;
}
void
xwayland_adjust_usable_area(struct view *view, struct wlr_output_layout *layout,
		struct wlr_output *output, struct wlr_box *usable)
{
	assert(view);
	assert(layout);
	assert(output);
	assert(usable);
	if (view->type != LAB_XWAYLAND_VIEW) {
		return;
	}
	xcb_ewmh_wm_strut_partial_t *strut =
		LABWC_src_xwayland_c_xwayland_surface_from_view(view)->strut_partial;
	if (!strut) {
		return;
	}
	struct wlr_box lb = { 0 };
	wlr_output_layout_get_box(layout, NULL, &lb);
	struct wlr_box ob = { 0 };
	wlr_output_layout_get_box(layout, output, &ob);
	double strut_left = strut->left;
	double strut_right = (uint32_t)(lb.x + lb.width) - strut->right;
	double strut_top = strut->top;
	double strut_bottom = (uint32_t)(lb.y + lb.height) - strut->bottom;
	wlr_output_layout_output_coords(layout, output,
		&strut_left, &strut_top);
	wlr_output_layout_output_coords(layout, output,
		&strut_right, &strut_bottom);
	int usable_right = usable->x + usable->width;
	int usable_bottom = usable->y + usable->height;
	if (strut_left > usable->x && strut_left < usable_right
			&& LABWC_src_xwayland_c_intervals_overlap(ob.y, ob.y + ob.height,
			(int)strut->left_start_y, (int)(strut->left_end_y + 1))) {
		usable->x = (int)strut_left;
	}
	if (strut_right > usable->x && strut_right < usable_right
			&& LABWC_src_xwayland_c_intervals_overlap(ob.y, ob.y + ob.height,
			(int)strut->right_start_y, (int)(strut->right_end_y + 1))) {
		usable_right = (int)strut_right;
	}
	if (strut_top > usable->y && strut_top < usable_bottom
			&& LABWC_src_xwayland_c_intervals_overlap(ob.x, ob.x + ob.width,
			(int)strut->top_start_x, (int)(strut->top_end_x + 1))) {
		usable->y = (int)strut_top;
	}
	if (strut_bottom > usable->y && strut_bottom < usable_bottom
			&& LABWC_src_xwayland_c_intervals_overlap(ob.x, ob.x + ob.width,
			(int)strut->bottom_start_x, (int)(strut->bottom_end_x + 1))) {
		usable_bottom = (int)strut_bottom;
	}
	usable->width = usable_right - usable->x;
	usable->height = usable_bottom - usable->y;
}
void
xwayland_update_workarea(void)
{
	if (!server.xwayland || !server.xwayland->xwm) {
		return;
	}
	struct wlr_box lb;
	wlr_output_layout_get_box(server.output_layout, NULL, &lb);
	int layout_left = MAX(0, lb.x);
	int layout_right = MAX(0, lb.x + lb.width);
	int layout_top = MAX(0, lb.y);
	int layout_bottom = MAX(0, lb.y + lb.height);
	int workarea_left = layout_left;
	int workarea_right = layout_right;
	int workarea_top = layout_top;
	int workarea_bottom = layout_bottom;
	struct output *output;
	wl_list_for_each(output, &server.outputs, link) {
		if (!output_is_usable(output)) {
			continue;
		}
		struct wlr_box ob;
		wlr_output_layout_get_box(server.output_layout,
			output->wlr_output, &ob);
		int output_left = ob.x;
		int output_right = ob.x + ob.width;
		int output_top = ob.y;
		int output_bottom = ob.y + ob.height;
		int usable_left = output_left + output->usable_area.x;
		int usable_right = usable_left + output->usable_area.width;
		int usable_top = output_top + output->usable_area.y;
		int usable_bottom = usable_top + output->usable_area.height;
		if (output_left == layout_left) {
			workarea_left = MAX(workarea_left, usable_left);
		}
		if (output_right == layout_right) {
			workarea_right = MIN(workarea_right, usable_right);
		}
		if (output_top == layout_top) {
			workarea_top = MAX(workarea_top, usable_top);
		}
		if (output_bottom == layout_bottom) {
			workarea_bottom = MIN(workarea_bottom, usable_bottom);
		}
	}
	struct wlr_box workarea = {
		.x = workarea_left,
		.y = workarea_top,
		.width = workarea_right - workarea_left,
		.height = workarea_bottom - workarea_top,
	};
	wlr_xwayland_set_workareas(server.xwayland, &workarea, 1);
}
void
xwayland_flush(void)
{
	if (!server.xwayland || !server.xwayland->xwm) {
		return;
	}
	xcb_flush(wlr_xwayland_get_xwm_connection(server.xwayland));
}
#undef handle_always_on_top
#undef handle_associate
#undef handle_commit
#undef handle_destroy
#undef handle_dissociate
#undef handle_focus_in
#undef handle_request_above
#undef handle_request_activate
#undef handle_request_close
#undef handle_request_configure
#undef handle_request_fullscreen
#undef handle_request_maximize
#undef handle_request_minimize
#undef handle_request_move
#undef handle_request_resize
#undef handle_set_class
#undef handle_set_decorations
#undef handle_set_override_redirect
#undef handle_set_strut_partial
#undef handle_set_title
#undef handle_set_window_type
#endif
#define handle_commit LABWC_src_xdg_popup_c_handle_commit
#define handle_destroy LABWC_src_xdg_popup_c_handle_destroy
#define handle_new_popup LABWC_src_xdg_popup_c_handle_new_popup
#define handle_reposition LABWC_src_xdg_popup_c_handle_reposition
struct LABWC_src_xdg_popup_c_xdg_popup {
	struct view *parent_view;
	struct wlr_xdg_popup *wlr_popup;
	struct wl_listener commit;
	struct wl_listener destroy;
	struct wl_listener new_popup;
	struct wl_listener reposition;
};
static void
LABWC_src_xdg_popup_c_popup_unconstrain(struct LABWC_src_xdg_popup_c_xdg_popup *popup)
{
	struct view *view = popup->parent_view;
	int parent_lx, parent_ly;
	struct wlr_scene_tree *parent_tree = popup->wlr_popup->parent->data;
	wlr_scene_node_coords(&parent_tree->node, &parent_lx, &parent_ly);
	struct wlr_box *popup_box = &popup->wlr_popup->scheduled.geometry;
	struct output *output = output_nearest_to(parent_lx + MAX(popup_box->x - 1, 0),
		parent_ly + MAX(popup_box->y - 1, 0));
	struct wlr_box usable = output_usable_area_in_layout_coords(output);
	int toplevel_dx = 0;
	int toplevel_dy = 0;
	struct wlr_xdg_surface *toplevel_surface = xdg_surface_from_view(view);
	if (toplevel_surface) {
		toplevel_dx = toplevel_surface->current.geometry.x;
		toplevel_dy = toplevel_surface->current.geometry.y;
	} else {
	}
	struct wlr_box output_toplevel_box = {
		.x = usable.x - (view->current.x - toplevel_dx),
		.y = usable.y - (view->current.y - toplevel_dy),
		.width = usable.width,
		.height = usable.height,
	};
	wlr_xdg_popup_unconstrain_from_box(popup->wlr_popup, &output_toplevel_box);
}
static void
LABWC_src_xdg_popup_c_handle_destroy(struct wl_listener *listener, void *data)
{
	struct LABWC_src_xdg_popup_c_xdg_popup *popup = wl_container_of(listener, popup, destroy);
	struct wlr_xdg_popup *_popup, *tmp;
	wl_list_for_each_safe(_popup, tmp, &popup->wlr_popup->base->popups, link) {
		wlr_xdg_popup_destroy(_popup);
	}
	wl_list_remove(&popup->destroy.link);
	wl_list_remove(&popup->new_popup.link);
	wl_list_remove(&popup->reposition.link);
	if (popup->commit.notify) {
		wl_list_remove(&popup->commit.link);
	}
	cursor_update_focus();
	free(popup);
}
static void
LABWC_src_xdg_popup_c_handle_commit(struct wl_listener *listener, void *data)
{
	struct LABWC_src_xdg_popup_c_xdg_popup *popup = wl_container_of(listener, popup, commit);
	if (popup->wlr_popup->base->initial_commit) {
		LABWC_src_xdg_popup_c_popup_unconstrain(popup);
		wl_list_remove(&popup->commit.link);
		popup->commit.notify = NULL;
	}
}
static void
LABWC_src_xdg_popup_c_handle_reposition(struct wl_listener *listener, void *data)
{
	struct LABWC_src_xdg_popup_c_xdg_popup *popup = wl_container_of(listener, popup, reposition);
	LABWC_src_xdg_popup_c_popup_unconstrain(popup);
}
static void
LABWC_src_xdg_popup_c_handle_new_popup(struct wl_listener *listener, void *data)
{
	struct LABWC_src_xdg_popup_c_xdg_popup *popup = wl_container_of(listener, popup, new_popup);
	struct wlr_xdg_popup *wlr_popup = data;
	xdg_popup_create(popup->parent_view, wlr_popup);
}
void
xdg_popup_create(struct view *view, struct wlr_xdg_popup *wlr_popup)
{
	struct wlr_xdg_surface *parent =
		wlr_xdg_surface_try_from_wlr_surface(wlr_popup->parent);
	if (!parent) {
		return;
	}
	struct LABWC_src_xdg_popup_c_xdg_popup *popup = znew(*popup);
	popup->parent_view = view;
	popup->wlr_popup = wlr_popup;
	CONNECT_SIGNAL(wlr_popup, popup, destroy);
	CONNECT_SIGNAL(wlr_popup->base, popup, new_popup);
	CONNECT_SIGNAL(wlr_popup->base->surface, popup, commit);
	CONNECT_SIGNAL(wlr_popup, popup, reposition);
	struct wlr_scene_tree *parent_tree = NULL;
	if (parent->role == WLR_XDG_SURFACE_ROLE_POPUP) {
		parent_tree = parent->surface->data;
	} else {
		parent_tree = server.xdg_popup_tree;
		wlr_scene_node_set_position(&server.xdg_popup_tree->node,
			view->current.x, view->current.y);
	}
	wlr_popup->base->surface->data =
		wlr_scene_xdg_surface_create(parent_tree, wlr_popup->base);
	die_if_null(wlr_popup->base->surface->data);
	node_descriptor_create(wlr_popup->base->surface->data,
		LAB_NODE_XDG_POPUP, view,   NULL);
	wlr_scene_xdg_surface_create(&view->capture.scene->tree, wlr_popup->base);
}
#undef handle_commit
#undef handle_destroy
#undef handle_new_popup
#undef handle_reposition
#define handle_commit LABWC_src_xdg_c_handle_commit
#define handle_destroy LABWC_src_xdg_c_handle_destroy
#define handle_new_popup LABWC_src_xdg_c_handle_new_popup
#define handle_request_fullscreen LABWC_src_xdg_c_handle_request_fullscreen
#define handle_request_maximize LABWC_src_xdg_c_handle_request_maximize
#define handle_request_minimize LABWC_src_xdg_c_handle_request_minimize
#define handle_request_move LABWC_src_xdg_c_handle_request_move
#define handle_request_resize LABWC_src_xdg_c_handle_request_resize
#define handle_request_show_window_menu LABWC_src_xdg_c_handle_request_show_window_menu
#define handle_set_app_id LABWC_src_xdg_c_handle_set_app_id
#define handle_set_parent LABWC_src_xdg_c_handle_set_parent
#define handle_set_title LABWC_src_xdg_c_handle_set_title
#include <assert.h>
#define LAB_XDG_SHELL_VERSION 6
#define CONFIGURE_TIMEOUT_MS 100
static struct view *LABWC_src_xdg_c_xdg_toplevel_view_get_root(struct view *view);
static struct xdg_toplevel_view *
LABWC_src_xdg_c_xdg_toplevel_view_from_view(struct view *view)
{
	assert(view->type == LAB_XDG_SHELL_VIEW);
	return (struct xdg_toplevel_view *)view;
}
struct wlr_xdg_surface *
xdg_surface_from_view(struct view *view)
{
	assert(view->type == LAB_XDG_SHELL_VIEW);
	struct xdg_toplevel_view *xdg_toplevel_view =
		(struct xdg_toplevel_view *)view;
	assert(xdg_toplevel_view->xdg_surface);
	return xdg_toplevel_view->xdg_surface;
}
static struct wlr_xdg_toplevel *
LABWC_src_xdg_c_xdg_toplevel_from_view(struct view *view)
{
	struct wlr_xdg_surface *xdg_surface = xdg_surface_from_view(view);
	assert(xdg_surface->role == WLR_XDG_SURFACE_ROLE_TOPLEVEL);
	assert(xdg_surface->toplevel);
	return xdg_surface->toplevel;
}
static struct view *
LABWC_src_xdg_c_xdg_toplevel_view_get_parent(struct view *view)
{
	struct wlr_xdg_toplevel *toplevel = LABWC_src_xdg_c_xdg_toplevel_from_view(view);
	return toplevel->parent ? toplevel->parent->base->data : NULL;
}
static struct view_size_hints
LABWC_src_xdg_c_xdg_toplevel_view_get_size_hints(struct view *view)
{
	assert(view);
	struct wlr_xdg_toplevel *toplevel = LABWC_src_xdg_c_xdg_toplevel_from_view(view);
	struct wlr_xdg_toplevel_state *state = &toplevel->current;
	return (struct view_size_hints){
		.min_width = state->min_width,
		.min_height = state->min_height,
	};
}
static bool
LABWC_src_xdg_c_xdg_toplevel_view_contains_window_type(struct view *view,
		enum lab_window_type window_type)
{
	assert(view);
	struct wlr_xdg_toplevel *toplevel = LABWC_src_xdg_c_xdg_toplevel_from_view(view);
	struct wlr_xdg_toplevel_state *state = &toplevel->current;
	bool is_dialog = (state->min_width != 0 && state->min_height != 0
		&& (state->min_width == state->max_width
		|| state->min_height == state->max_height))
		|| toplevel->parent;
	switch (window_type) {
	case LAB_WINDOW_TYPE_NORMAL:
		return !is_dialog;
	case LAB_WINDOW_TYPE_DIALOG:
		return is_dialog;
	default:
		return false;
	}
}
static void
LABWC_src_xdg_c_handle_new_popup(struct wl_listener *listener, void *data)
{
	struct xdg_toplevel_view *xdg_toplevel_view =
		wl_container_of(listener, xdg_toplevel_view, new_popup);
	struct view *view = &xdg_toplevel_view->base;
	struct wlr_xdg_popup *wlr_popup = data;
	xdg_popup_create(view, wlr_popup);
}
static void
LABWC_src_xdg_c_set_fullscreen_from_request(struct view *view,
		struct wlr_xdg_toplevel_requested *requested)
{
	if (!view->fullscreen && requested->fullscreen
			&& requested->fullscreen_output) {
		view_set_output(view, output_from_wlr_output(requested->fullscreen_output));
	}
	view_set_fullscreen(view, requested->fullscreen);
}
static void
LABWC_src_xdg_c_set_initial_position(struct view *view)
{
	if (!view_is_floating(view)) {
		return;
	}
	view_constrain_size_to_that_of_usable_area(view);
	if (server.input_mode == LAB_INPUT_STATE_MOVE
			&& view == server.grabbed_view) {
		interactive_anchor_to_cursor(&view->pending);
	} else {
		struct view *parent = LABWC_src_xdg_c_xdg_toplevel_view_get_parent(view);
		if (parent) {
			view_compute_centered_position(view, &parent->pending,
				view->pending.width, view->pending.height,
				&view->pending.x, &view->pending.y);
		} else {
			view_compute_position_by_policy(view, &view->pending,
				  true, rc.placement_policy);
		}
	}
	view_save_last_placement(view);
}
static void
LABWC_src_xdg_c_disable_fullscreen_bg(struct view *view)
{
	struct xdg_toplevel_view *xdg_view = LABWC_src_xdg_c_xdg_toplevel_view_from_view(view);
	if (xdg_view->fullscreen_bg) {
		wlr_scene_node_set_enabled(&xdg_view->fullscreen_bg->node, false);
	}
}
static void
LABWC_src_xdg_c_center_fullscreen_if_needed(struct view *view)
{
	if (!view->fullscreen || !output_is_usable(view->output)) {
		LABWC_src_xdg_c_disable_fullscreen_bg(view);
		return;
	}
	struct wlr_box output_box = {0};
	wlr_output_layout_get_box(server.output_layout,
		view->output->wlr_output, &output_box);
	box_center(view->current.width, view->current.height, &output_box,
		&output_box, &view->current.x, &view->current.y);
	view->pending.x = view->current.x;
	view->pending.y = view->current.y;
	if (view->current.width >= output_box.width
			&& view->current.width >= output_box.height) {
		LABWC_src_xdg_c_disable_fullscreen_bg(view);
		return;
	}
	struct xdg_toplevel_view *xdg_view = LABWC_src_xdg_c_xdg_toplevel_view_from_view(view);
	if (!xdg_view->fullscreen_bg) {
		const float black[4] = {0, 0, 0, 1};
		xdg_view->fullscreen_bg =
			lab_wlr_scene_rect_create(view->scene_tree, 0, 0, black);
		wlr_scene_node_lower_to_bottom(&xdg_view->fullscreen_bg->node);
	}
	wlr_scene_node_set_position(&xdg_view->fullscreen_bg->node,
		output_box.x - view->current.x, output_box.y - view->current.y);
	wlr_scene_rect_set_size(xdg_view->fullscreen_bg,
		output_box.width, output_box.height);
	wlr_scene_node_set_enabled(&xdg_view->fullscreen_bg->node, true);
}
static void LABWC_src_xdg_c_set_pending_configure_serial(struct view *view, uint32_t serial);
static void
LABWC_src_xdg_c_handle_commit(struct wl_listener *listener, void *data)
{
	struct view *view = wl_container_of(listener, view, commit);
	struct wlr_xdg_surface *xdg_surface = xdg_surface_from_view(view);
	struct wlr_xdg_toplevel *toplevel = LABWC_src_xdg_c_xdg_toplevel_from_view(view);
	assert(view->surface);
	if (xdg_surface->initial_commit) {
		uint32_t serial =
			wlr_xdg_surface_schedule_configure(xdg_surface);
		if (serial > 0) {
			LABWC_src_xdg_c_set_pending_configure_serial(view, serial);
		}
		uint32_t wm_caps = WLR_XDG_TOPLEVEL_WM_CAPABILITIES_WINDOW_MENU
			| WLR_XDG_TOPLEVEL_WM_CAPABILITIES_MAXIMIZE
			| WLR_XDG_TOPLEVEL_WM_CAPABILITIES_FULLSCREEN
			| WLR_XDG_TOPLEVEL_WM_CAPABILITIES_MINIMIZE;
		wlr_xdg_toplevel_set_wm_capabilities(toplevel, wm_caps);
		struct view *parent = LABWC_src_xdg_c_xdg_toplevel_view_get_parent(view);
		if (parent && output_is_usable(parent->output)) {
			view_set_output(view, parent->output);
		} else {
			view_set_output(view, output_nearest_to_cursor());
		}
		if (output_is_usable(view->output)) {
			wlr_xdg_toplevel_set_bounds(toplevel,
				view->output->usable_area.width,
				view->output->usable_area.height);
		}
		if (toplevel->requested.fullscreen) {
			LABWC_src_xdg_c_set_fullscreen_from_request(view, &toplevel->requested);
		}
		if (toplevel->requested.maximized) {
			view_maximize(view, VIEW_AXIS_BOTH);
		}
		return;
	}
	struct wlr_box size = xdg_surface->geometry;
	bool update_required = false;
	if (wlr_box_empty(&view->pending) && !wlr_box_empty(&size)) {
		view->pending.width = size.width;
		view->pending.height = size.height;
		LABWC_src_xdg_c_set_initial_position(view);
		update_required = true;
	}
	if (size.width != view->pending.width
			|| size.height != view->pending.height) {
		struct wlr_box extent = {
			.width = view->surface->current.width,
			.height = view->surface->current.height,
		};
		if (extent.width == view->pending.width
				&& extent.height == view->pending.height) {
			size = extent;
		}
	}
	struct wlr_box *current = &view->current;
	if (current->width != size.width || current->height != size.height) {
		update_required = true;
	}
	uint32_t serial = view->pending_configure_serial;
	if (serial > 0 && serial == xdg_surface->current.configure_serial) {
		assert(view->pending_configure_timeout);
		wl_event_source_remove(view->pending_configure_timeout);
		view->pending_configure_serial = 0;
		view->pending_configure_timeout = NULL;
		update_required = true;
	}
	if (update_required) {
		view_impl_apply_geometry(view, size.width, size.height);
		LABWC_src_xdg_c_center_fullscreen_if_needed(view);
		view_moved(view);
		if (!view->pending_configure_serial) {
			snap_constraints_update(view);
			view->pending = view->current;
			toplevel->scheduled.width = view->current.width;
			toplevel->scheduled.height = view->current.height;
		}
	}
}
static int
LABWC_src_xdg_c_handle_configure_timeout(void *data)
{
	struct view *view = data;
	assert(view->pending_configure_serial > 0);
	assert(view->pending_configure_timeout);
	wl_event_source_remove(view->pending_configure_timeout);
	view->pending_configure_serial = 0;
	view->pending_configure_timeout = NULL;
	if (!view->mapped) {
		return 0;
	}
	bool empty_pending = wlr_box_empty(&view->pending);
	if (empty_pending || view->pending.x != view->current.x
			|| view->pending.y != view->current.y) {
		if (empty_pending) {
			view->pending.x = VIEW_FALLBACK_X;
			view->pending.y = VIEW_FALLBACK_Y;
			if (output_is_usable(view->output)) {
				struct wlr_box box =
					output_usable_area_in_layout_coords(view->output);
				view->pending.x += box.x;
				view->pending.y += box.y;
			}
		}
		view->current.x = view->pending.x;
		view->current.y = view->pending.y;
	}
	LABWC_src_xdg_c_center_fullscreen_if_needed(view);
	view_moved(view);
	snap_constraints_update(view);
	view->pending = view->current;
	return 0;
}
static void
LABWC_src_xdg_c_set_pending_configure_serial(struct view *view, uint32_t serial)
{
	view->pending_configure_serial = serial;
	if (!view->pending_configure_timeout) {
		view->pending_configure_timeout =
			wl_event_loop_add_timer(server.wl_event_loop,
				LABWC_src_xdg_c_handle_configure_timeout, view);
	}
	wl_event_source_timer_update(view->pending_configure_timeout,
		CONFIGURE_TIMEOUT_MS);
}
static void
LABWC_src_xdg_c_handle_destroy(struct wl_listener *listener, void *data)
{
	struct view *view = wl_container_of(listener, view, destroy);
	assert(view->type == LAB_XDG_SHELL_VIEW);
	struct xdg_toplevel_view *xdg_toplevel_view =
		LABWC_src_xdg_c_xdg_toplevel_view_from_view(view);
	struct wlr_xdg_popup *popup, *tmp;
	wl_list_for_each_safe(popup, tmp, &xdg_toplevel_view->xdg_surface->popups, link) {
		wlr_xdg_popup_destroy(popup);
	}
	xdg_toplevel_view->xdg_surface->data = NULL;
	xdg_toplevel_view->xdg_surface = NULL;
	wl_list_remove(&xdg_toplevel_view->set_app_id.link);
	wl_list_remove(&xdg_toplevel_view->request_show_window_menu.link);
	wl_list_remove(&xdg_toplevel_view->set_parent.link);
	wl_list_remove(&xdg_toplevel_view->new_popup.link);
	wl_list_remove(&view->commit.link);
	if (view->pending_configure_timeout) {
		wl_event_source_remove(view->pending_configure_timeout);
		view->pending_configure_timeout = NULL;
	}
	view_destroy(view);
}
static void
LABWC_src_xdg_c_handle_request_move(struct wl_listener *listener, void *data)
{
	struct view *view = wl_container_of(listener, view, request_move);
	interactive_begin(view, LAB_INPUT_STATE_MOVE, LAB_EDGE_NONE);
}
static void
LABWC_src_xdg_c_handle_request_resize(struct wl_listener *listener, void *data)
{
	struct wlr_xdg_toplevel_resize_event *event = data;
	struct view *view = wl_container_of(listener, view, request_resize);
	interactive_begin(view, LAB_INPUT_STATE_RESIZE, event->edges);
}
static void
LABWC_src_xdg_c_handle_request_minimize(struct wl_listener *listener, void *data)
{
	struct view *view = wl_container_of(listener, view, request_minimize);
	view_minimize(view, LABWC_src_xdg_c_xdg_toplevel_from_view(view)->requested.minimized);
}
static void
LABWC_src_xdg_c_handle_request_maximize(struct wl_listener *listener, void *data)
{
	struct view *view = wl_container_of(listener, view, request_maximize);
	struct wlr_xdg_toplevel *toplevel = LABWC_src_xdg_c_xdg_toplevel_from_view(view);
	if (!toplevel->base->initialized) {
		return;
	}
	bool maximized = toplevel->requested.maximized;
	view_maximize(view, maximized ? VIEW_AXIS_BOTH : VIEW_AXIS_NONE);
}
static void
LABWC_src_xdg_c_handle_request_fullscreen(struct wl_listener *listener, void *data)
{
	struct view *view = wl_container_of(listener, view, request_fullscreen);
	struct wlr_xdg_toplevel *toplevel = LABWC_src_xdg_c_xdg_toplevel_from_view(view);
	if (!toplevel->base->initialized) {
		return;
	}
	LABWC_src_xdg_c_set_fullscreen_from_request(view,
		&LABWC_src_xdg_c_xdg_toplevel_from_view(view)->requested);
}
static void
LABWC_src_xdg_c_handle_request_show_window_menu(struct wl_listener *listener, void *data)
{
	struct xdg_toplevel_view *xdg_toplevel_view = wl_container_of(
		listener, xdg_toplevel_view, request_show_window_menu);
	struct menu *menu = menu_get_by_id("client-menu");
	assert(menu);
	menu->triggered_by_view = &xdg_toplevel_view->base;
	struct wlr_cursor *cursor = server.seat.cursor;
	menu_open(menu, (int)cursor->x, (int)cursor->y);
}
static void
LABWC_src_xdg_c_handle_set_parent(struct wl_listener *listener, void *data)
{
	struct xdg_toplevel_view *xdg_toplevel_view = wl_container_of(
		listener, xdg_toplevel_view, set_parent);
	struct view *view = &xdg_toplevel_view->base;
	struct view *view_root = LABWC_src_xdg_c_xdg_toplevel_view_get_root(view);
	if (view_root == view) {
		return;
	}
	struct wlr_scene_node *node, *tmp;
	wl_list_for_each_safe(node, tmp, &view->capture.scene->tree.children, link) {
		wlr_scene_node_reparent(node, &view_root->capture.scene->tree);
	}
}
static void
LABWC_src_xdg_c_handle_set_title(struct wl_listener *listener, void *data)
{
	struct view *view = wl_container_of(listener, view, set_title);
	struct wlr_xdg_toplevel *toplevel = LABWC_src_xdg_c_xdg_toplevel_from_view(view);
	view_set_title(view, toplevel->title);
}
static void
LABWC_src_xdg_c_handle_set_app_id(struct wl_listener *listener, void *data)
{
	struct xdg_toplevel_view *xdg_toplevel_view =
		wl_container_of(listener, xdg_toplevel_view, set_app_id);
	struct view *view = &xdg_toplevel_view->base;
	struct wlr_xdg_toplevel *toplevel = LABWC_src_xdg_c_xdg_toplevel_from_view(view);
	view_set_app_id(view, toplevel->app_id);
}
static void
LABWC_src_xdg_c_xdg_toplevel_view_configure(struct view *view, struct wlr_box geo)
{
	uint32_t serial = 0;
	struct wlr_xdg_toplevel *toplevel = LABWC_src_xdg_c_xdg_toplevel_from_view(view);
	if (geo.width != view->pending.width
			|| geo.height != view->pending.height) {
		if (toplevel->base->initialized) {
			if (geo.width > 0 && geo.height > 0) {
				serial = wlr_xdg_toplevel_set_size(toplevel,
						geo.width, geo.height);
			} else {
			}
		} else {
		}
	}
	view->pending = geo;
	if (serial > 0) {
		LABWC_src_xdg_c_set_pending_configure_serial(view, serial);
	} else if (view->pending_configure_serial == 0) {
		view->current.x = geo.x;
		view->current.y = geo.y;
		LABWC_src_xdg_c_center_fullscreen_if_needed(view);
		view_moved(view);
	}
}
static void
LABWC_src_xdg_c_xdg_toplevel_view_close(struct view *view)
{
	wlr_xdg_toplevel_send_close(LABWC_src_xdg_c_xdg_toplevel_from_view(view));
}
static void
LABWC_src_xdg_c_xdg_toplevel_view_maximize(struct view *view, enum view_axis maximized)
{
	if (!LABWC_src_xdg_c_xdg_toplevel_from_view(view)->base->initialized) {
		return;
	}
	uint32_t serial = wlr_xdg_toplevel_set_maximized(
		LABWC_src_xdg_c_xdg_toplevel_from_view(view), maximized == VIEW_AXIS_BOTH);
	if (serial > 0) {
		LABWC_src_xdg_c_set_pending_configure_serial(view, serial);
	}
}
static void
LABWC_src_xdg_c_xdg_toplevel_view_minimize(struct view *view, bool minimized)
{
}
static struct wlr_xdg_toplevel *
LABWC_src_xdg_c_top_parent_of(struct view *view)
{
	struct wlr_xdg_toplevel *toplevel = LABWC_src_xdg_c_xdg_toplevel_from_view(view);
	while (toplevel->parent) {
		toplevel = toplevel->parent;
	}
	return toplevel;
}
static struct view *
LABWC_src_xdg_c_xdg_toplevel_view_get_root(struct view *view)
{
	struct wlr_xdg_toplevel *root = LABWC_src_xdg_c_top_parent_of(view);
	return (struct view *)root->base->data;
}
static void
LABWC_src_xdg_c_xdg_toplevel_view_append_children(struct view *self, struct wl_array *children)
{
	struct wlr_xdg_toplevel *toplevel = LABWC_src_xdg_c_xdg_toplevel_from_view(self);
	struct view *view;
	wl_list_for_each_reverse(view, &server.views, link) {
		if (view == self) {
			continue;
		}
		if (view->type != LAB_XDG_SHELL_VIEW) {
			continue;
		}
		if (!view->mapped) {
			continue;
		}
		if (LABWC_src_xdg_c_top_parent_of(view) != toplevel) {
			continue;
		}
		struct view **child = wl_array_add(children, sizeof(*child));
		*child = view;
	}
}
static bool
LABWC_src_xdg_c_xdg_toplevel_view_is_modal_dialog(struct view *view)
{
	struct wlr_xdg_toplevel *toplevel = LABWC_src_xdg_c_xdg_toplevel_from_view(view);
	struct wlr_xdg_dialog_v1 *dialog =
		wlr_xdg_dialog_v1_try_from_wlr_xdg_toplevel(toplevel);
	if (!dialog) {
		return false;
	}
	return dialog->modal;
}
static void
LABWC_src_xdg_c_xdg_toplevel_view_set_activated(struct view *view, bool activated)
{
	if (!LABWC_src_xdg_c_xdg_toplevel_from_view(view)->base->initialized) {
		return;
	}
	uint32_t serial = wlr_xdg_toplevel_set_activated(
		LABWC_src_xdg_c_xdg_toplevel_from_view(view), activated);
	if (serial > 0) {
		LABWC_src_xdg_c_set_pending_configure_serial(view, serial);
	}
}
static void
LABWC_src_xdg_c_xdg_toplevel_view_set_fullscreen(struct view *view, bool fullscreen)
{
	if (!LABWC_src_xdg_c_xdg_toplevel_from_view(view)->base->initialized) {
		return;
	}
	uint32_t serial = wlr_xdg_toplevel_set_fullscreen(
		LABWC_src_xdg_c_xdg_toplevel_from_view(view), fullscreen);
	if (serial > 0) {
		LABWC_src_xdg_c_set_pending_configure_serial(view, serial);
	}
	if (!fullscreen) {
		LABWC_src_xdg_c_disable_fullscreen_bg(view);
	}
}
static void
LABWC_src_xdg_c_xdg_toplevel_view_notify_tiled(struct view *view)
{
	if (rc.snap_tiling_events_mode == LAB_TILING_EVENTS_NEVER) {
		return;
	}
	if (!LABWC_src_xdg_c_xdg_toplevel_from_view(view)->base->initialized) {
		return;
	}
	enum lab_edge edge = LAB_EDGE_NONE;
	bool want_edge = rc.snap_tiling_events_mode & LAB_TILING_EVENTS_EDGE;
	bool want_region = rc.snap_tiling_events_mode & LAB_TILING_EVENTS_REGION;
	if (want_edge) {
		switch (view->tiled) {
		case LAB_EDGE_LEFT:
			edge = LAB_EDGES_EXCEPT_RIGHT;
			break;
		case LAB_EDGE_RIGHT:
			edge = LAB_EDGES_EXCEPT_LEFT;
			break;
		case LAB_EDGE_TOP:
			edge = LAB_EDGES_EXCEPT_BOTTOM;
			break;
		case LAB_EDGE_BOTTOM:
			edge = LAB_EDGES_EXCEPT_TOP;
			break;
		case LAB_EDGES_TOP_LEFT:
		case LAB_EDGES_TOP_RIGHT:
		case LAB_EDGES_BOTTOM_LEFT:
		case LAB_EDGES_BOTTOM_RIGHT:
			edge = view->tiled;
			break;
		default:
			edge = LAB_EDGE_NONE;
		}
	}
	if (want_region && view->tiled_region) {
		edge = LAB_EDGES_ALL;
	}
	uint32_t serial =
		wlr_xdg_toplevel_set_tiled(LABWC_src_xdg_c_xdg_toplevel_from_view(view), edge);
	if (serial > 0) {
		LABWC_src_xdg_c_set_pending_configure_serial(view, serial);
	}
}
static void
LABWC_src_xdg_c_handle_map(struct wl_listener *listener, void *data)
{
	struct view *view = wl_container_of(listener, view, mappable.map);
	if (view->mapped) {
		return;
	}
	view->mapped = true;
	if (!view->been_mapped) {
		if (view_wants_decorations(view)) {
			view_set_ssd_mode(view, LAB_SSD_MODE_FULL);
		} else {
			view_set_ssd_mode(view, LAB_SSD_MODE_NONE);
		}
		if (wlr_box_empty(&view->pending)) {
			struct wlr_xdg_surface *xdg_surface =
				xdg_surface_from_view(view);
			view->pending.width = xdg_surface->geometry.width;
			view->pending.height = xdg_surface->geometry.height;
			LABWC_src_xdg_c_set_initial_position(view);
		}
		view->current.x = view->pending.x;
		view->current.y = view->pending.y;
		view_moved(view);
	}
	view_impl_map(view);
	view->been_mapped = true;
}
static void
LABWC_src_xdg_c_handle_unmap(struct wl_listener *listener, void *data)
{
	struct view *view = wl_container_of(listener, view, mappable.unmap);
	if (view->mapped) {
		view->mapped = false;
		view_impl_unmap(view);
	}
}
static pid_t
LABWC_src_xdg_c_xdg_view_get_pid(struct view *view)
{
	assert(view);
	pid_t pid = -1;
	if (view->surface && view->surface->resource
			&& view->surface->resource->client) {
		struct wl_client *client = view->surface->resource->client;
		wl_client_get_credentials(client, &pid, NULL, NULL);
	}
	return pid;
}
static const struct view_impl LABWC_src_xdg_c_xdg_toplevel_view_impl = {
	.configure = LABWC_src_xdg_c_xdg_toplevel_view_configure,
	.close = LABWC_src_xdg_c_xdg_toplevel_view_close,
	.set_activated = LABWC_src_xdg_c_xdg_toplevel_view_set_activated,
	.set_fullscreen = LABWC_src_xdg_c_xdg_toplevel_view_set_fullscreen,
	.notify_tiled = LABWC_src_xdg_c_xdg_toplevel_view_notify_tiled,
	.maximize = LABWC_src_xdg_c_xdg_toplevel_view_maximize,
	.minimize = LABWC_src_xdg_c_xdg_toplevel_view_minimize,
	.get_parent = LABWC_src_xdg_c_xdg_toplevel_view_get_parent,
	.get_root = LABWC_src_xdg_c_xdg_toplevel_view_get_root,
	.append_children = LABWC_src_xdg_c_xdg_toplevel_view_append_children,
	.is_modal_dialog = LABWC_src_xdg_c_xdg_toplevel_view_is_modal_dialog,
	.get_size_hints = LABWC_src_xdg_c_xdg_toplevel_view_get_size_hints,
	.contains_window_type = LABWC_src_xdg_c_xdg_toplevel_view_contains_window_type,
	.get_pid = LABWC_src_xdg_c_xdg_view_get_pid,
};
struct LABWC_src_xdg_c_token_data {
	bool had_valid_surface;
	bool had_valid_seat;
	struct wl_listener destroy;
};
static void
LABWC_src_xdg_c_handle_xdg_activation_token_destroy(struct wl_listener *listener, void *data)
{
	struct LABWC_src_xdg_c_token_data *token_data = wl_container_of(listener, token_data, destroy);
	wl_list_remove(&token_data->destroy.link);
	free(token_data);
}
static void
LABWC_src_xdg_c_handle_xdg_activation_new_token(struct wl_listener *listener, void *data)
{
	struct wlr_xdg_activation_token_v1 *token = data;
	struct LABWC_src_xdg_c_token_data *token_data = znew(*token_data);
	token_data->had_valid_surface = !!token->surface;
	token_data->had_valid_seat = !!token->seat;
	token->data = token_data;
	token_data->destroy.notify = LABWC_src_xdg_c_handle_xdg_activation_token_destroy;
	wl_signal_add(&token->events.destroy, &token_data->destroy);
}
static void
LABWC_src_xdg_c_handle_xdg_activation_request(struct wl_listener *listener, void *data)
{
	const struct wlr_xdg_activation_v1_request_activate_event *event = data;
	struct LABWC_src_xdg_c_token_data *token_data = event->token->data;
	assert(token_data);
	struct wlr_xdg_surface *xdg_surface =
		wlr_xdg_surface_try_from_wlr_surface(event->surface);
	if (!xdg_surface) {
		return;
	}
	struct view *view = xdg_surface->data;
	if (!view) {
		return;
	}
	if (!token_data->had_valid_seat) {
		return;
	}
	if (window_rules_get_property(view, "ignoreFocusRequest") == LAB_PROP_TRUE) {
		return;
	}
	desktop_focus_view(view,   true);
}
static void
LABWC_src_xdg_c_handle_new_xdg_toplevel(struct wl_listener *listener, void *data)
{
	struct wlr_xdg_toplevel *xdg_toplevel = data;
	struct wlr_xdg_surface *xdg_surface = xdg_toplevel->base;
	assert(xdg_surface->role == WLR_XDG_SURFACE_ROLE_TOPLEVEL);
	struct xdg_toplevel_view *xdg_toplevel_view = znew(*xdg_toplevel_view);
	struct view *view = &xdg_toplevel_view->base;
	view->type = LAB_XDG_SHELL_VIEW;
	view->impl = &LABWC_src_xdg_c_xdg_toplevel_view_impl;
	view_init(view);
	xdg_toplevel_view->xdg_surface = xdg_surface;
	view_set_output(view, output_nearest_to_cursor());
	view->workspace = server.workspaces.current;
	view->scene_tree = lab_wlr_scene_tree_create(
		view->workspace->view_trees[VIEW_LAYER_NORMAL]);
	wlr_scene_node_set_enabled(&view->scene_tree->node, false);
	struct wlr_scene_tree *tree = wlr_scene_xdg_surface_create(
		view->scene_tree, xdg_surface);
	die_if_null(tree);
	view->content_tree = tree;
	node_descriptor_create(&view->scene_tree->node,
		LAB_NODE_VIEW, view,   NULL);
	view->ssd_preference = LAB_SSD_PREF_CLIENT;
	xdg_surface->data = view;
	kde_server_decoration_set_view(view, xdg_surface->surface);
	view->surface = xdg_surface->surface;
	view->surface->data = tree;
	mappable_connect(&view->mappable, xdg_surface->surface,
		LABWC_src_xdg_c_handle_map, LABWC_src_xdg_c_handle_unmap);
	struct view *root_view = LABWC_src_xdg_c_xdg_toplevel_view_get_root(view);
	wlr_scene_xdg_surface_create(&root_view->capture.scene->tree, xdg_surface);
	struct wlr_xdg_toplevel *toplevel = xdg_surface->toplevel;
	CONNECT_SIGNAL(toplevel, view, destroy);
	CONNECT_SIGNAL(toplevel, view, request_move);
	CONNECT_SIGNAL(toplevel, view, request_resize);
	CONNECT_SIGNAL(toplevel, view, request_minimize);
	CONNECT_SIGNAL(toplevel, view, request_maximize);
	CONNECT_SIGNAL(toplevel, view, request_fullscreen);
	CONNECT_SIGNAL(toplevel, view, set_title);
	CONNECT_SIGNAL(view->surface, view, commit);
	CONNECT_SIGNAL(toplevel, xdg_toplevel_view, set_app_id);
	CONNECT_SIGNAL(toplevel, xdg_toplevel_view, request_show_window_menu);
	CONNECT_SIGNAL(toplevel, xdg_toplevel_view, set_parent);
	CONNECT_SIGNAL(xdg_surface, xdg_toplevel_view, new_popup);
	wl_list_insert(&server.views, &view->link);
	view->creation_id = server.next_view_creation_id++;
}
void
xdg_shell_init(void)
{
	server.xdg_shell = wlr_xdg_shell_create(server.wl_display,
		LAB_XDG_SHELL_VERSION);
	if (!server.xdg_shell) {
		exit(EXIT_FAILURE);
	}
	server.new_xdg_toplevel.notify = LABWC_src_xdg_c_handle_new_xdg_toplevel;
	wl_signal_add(&server.xdg_shell->events.new_toplevel, &server.new_xdg_toplevel);
	server.xdg_activation = wlr_xdg_activation_v1_create(server.wl_display);
	if (!server.xdg_activation) {
		exit(EXIT_FAILURE);
	}
	server.xdg_activation_request.notify = LABWC_src_xdg_c_handle_xdg_activation_request;
	wl_signal_add(&server.xdg_activation->events.request_activate,
		&server.xdg_activation_request);
	server.xdg_activation_new_token.notify = LABWC_src_xdg_c_handle_xdg_activation_new_token;
	wl_signal_add(&server.xdg_activation->events.new_token,
		&server.xdg_activation_new_token);
	wlr_xdg_wm_dialog_v1_create(server.wl_display, 1);
}
void
xdg_shell_finish(void)
{
	wl_list_remove(&server.new_xdg_toplevel.link);
	wl_list_remove(&server.xdg_activation_request.link);
	wl_list_remove(&server.xdg_activation_new_token.link);
}
#undef handle_commit
#undef handle_destroy
#undef handle_new_popup
#undef handle_request_fullscreen
#undef handle_request_maximize
#undef handle_request_minimize
#undef handle_request_move
#undef handle_request_resize
#undef handle_request_show_window_menu
#undef handle_set_app_id
#undef handle_set_parent
#undef handle_set_title
#undef CONFIGURE_TIMEOUT_MS
#undef LAB_XDG_SHELL_VERSION
#define handle_activated LABWC_src_foreign_toplevel_wlr_foreign_c_handle_activated
#define handle_fullscreened LABWC_src_foreign_toplevel_wlr_foreign_c_handle_fullscreened
#define handle_maximized LABWC_src_foreign_toplevel_wlr_foreign_c_handle_maximized
#define handle_minimized LABWC_src_foreign_toplevel_wlr_foreign_c_handle_minimized
#define handle_new_app_id LABWC_src_foreign_toplevel_wlr_foreign_c_handle_new_app_id
#define handle_new_outputs LABWC_src_foreign_toplevel_wlr_foreign_c_handle_new_outputs
#define handle_new_title LABWC_src_foreign_toplevel_wlr_foreign_c_handle_new_title
#define handle_request_activate LABWC_src_foreign_toplevel_wlr_foreign_c_handle_request_activate
#define handle_request_close LABWC_src_foreign_toplevel_wlr_foreign_c_handle_request_close
#define handle_request_fullscreen LABWC_src_foreign_toplevel_wlr_foreign_c_handle_request_fullscreen
#define handle_request_maximize LABWC_src_foreign_toplevel_wlr_foreign_c_handle_request_maximize
#define handle_request_minimize LABWC_src_foreign_toplevel_wlr_foreign_c_handle_request_minimize
#include <assert.h>
static void
LABWC_src_foreign_toplevel_wlr_foreign_c_handle_request_minimize(struct wl_listener *listener, void *data)
{
	struct wlr_foreign_toplevel *wlr_toplevel =
		wl_container_of(listener, wlr_toplevel, on.request_minimize);
	struct wlr_foreign_toplevel_handle_v1_minimized_event *event = data;
	view_minimize(wlr_toplevel->view, event->minimized);
}
static void
LABWC_src_foreign_toplevel_wlr_foreign_c_handle_request_maximize(struct wl_listener *listener, void *data)
{
	struct wlr_foreign_toplevel *wlr_toplevel =
		wl_container_of(listener, wlr_toplevel, on.request_maximize);
	struct wlr_foreign_toplevel_handle_v1_maximized_event *event = data;
	view_maximize(wlr_toplevel->view,
		event->maximized ? VIEW_AXIS_BOTH : VIEW_AXIS_NONE);
}
static void
LABWC_src_foreign_toplevel_wlr_foreign_c_handle_request_fullscreen(struct wl_listener *listener, void *data)
{
	struct wlr_foreign_toplevel *wlr_toplevel =
		wl_container_of(listener, wlr_toplevel, on.request_fullscreen);
	struct wlr_foreign_toplevel_handle_v1_fullscreen_event *event = data;
	view_set_fullscreen(wlr_toplevel->view, event->fullscreen);
}
static void
LABWC_src_foreign_toplevel_wlr_foreign_c_handle_request_activate(struct wl_listener *listener, void *data)
{
	struct wlr_foreign_toplevel *wlr_toplevel =
		wl_container_of(listener, wlr_toplevel, on.request_activate);
	desktop_focus_view(wlr_toplevel->view,   true);
}
static void
LABWC_src_foreign_toplevel_wlr_foreign_c_handle_request_close(struct wl_listener *listener, void *data)
{
	struct wlr_foreign_toplevel *wlr_toplevel =
		wl_container_of(listener, wlr_toplevel, on.request_close);
	view_close(wlr_toplevel->view);
}
static void
LABWC_src_foreign_toplevel_wlr_foreign_c_handle_handle_destroy(struct wl_listener *listener, void *data)
{
	struct wlr_foreign_toplevel *wlr_toplevel =
		wl_container_of(listener, wlr_toplevel, on.handle_destroy);
	wl_list_remove(&wlr_toplevel->on.request_maximize.link);
	wl_list_remove(&wlr_toplevel->on.request_minimize.link);
	wl_list_remove(&wlr_toplevel->on.request_fullscreen.link);
	wl_list_remove(&wlr_toplevel->on.request_activate.link);
	wl_list_remove(&wlr_toplevel->on.request_close.link);
	wl_list_remove(&wlr_toplevel->on.handle_destroy.link);
	wl_list_remove(&wlr_toplevel->on_view.new_app_id.link);
	wl_list_remove(&wlr_toplevel->on_view.new_title.link);
	wl_list_remove(&wlr_toplevel->on_view.new_outputs.link);
	wl_list_remove(&wlr_toplevel->on_view.maximized.link);
	wl_list_remove(&wlr_toplevel->on_view.minimized.link);
	wl_list_remove(&wlr_toplevel->on_view.fullscreened.link);
	wl_list_remove(&wlr_toplevel->on_view.activated.link);
	wlr_toplevel->handle = NULL;
}
static void
LABWC_src_foreign_toplevel_wlr_foreign_c_handle_new_app_id(struct wl_listener *listener, void *data)
{
	struct wlr_foreign_toplevel *wlr_toplevel =
		wl_container_of(listener, wlr_toplevel, on_view.new_app_id);
	assert(wlr_toplevel->handle);
	wlr_foreign_toplevel_handle_v1_set_app_id(wlr_toplevel->handle,
		wlr_toplevel->view->app_id);
}
static void
LABWC_src_foreign_toplevel_wlr_foreign_c_handle_new_title(struct wl_listener *listener, void *data)
{
	struct wlr_foreign_toplevel *wlr_toplevel =
		wl_container_of(listener, wlr_toplevel, on_view.new_title);
	assert(wlr_toplevel->handle);
	wlr_foreign_toplevel_handle_v1_set_title(wlr_toplevel->handle,
		wlr_toplevel->view->title);
}
static void
LABWC_src_foreign_toplevel_wlr_foreign_c_handle_new_outputs(struct wl_listener *listener, void *data)
{
	struct wlr_foreign_toplevel *wlr_toplevel =
		wl_container_of(listener, wlr_toplevel, on_view.new_outputs);
	assert(wlr_toplevel->handle);
	struct output *output;
	wl_list_for_each(output, &server.outputs, link) {
		if (view_on_output(wlr_toplevel->view, output)) {
			wlr_foreign_toplevel_handle_v1_output_enter(
				wlr_toplevel->handle, output->wlr_output);
		} else {
			wlr_foreign_toplevel_handle_v1_output_leave(
				wlr_toplevel->handle, output->wlr_output);
		}
	}
}
static void
LABWC_src_foreign_toplevel_wlr_foreign_c_handle_maximized(struct wl_listener *listener, void *data)
{
	struct wlr_foreign_toplevel *wlr_toplevel =
		wl_container_of(listener, wlr_toplevel, on_view.maximized);
	assert(wlr_toplevel->handle);
	wlr_foreign_toplevel_handle_v1_set_maximized(wlr_toplevel->handle,
		wlr_toplevel->view->maximized == VIEW_AXIS_BOTH);
}
static void
LABWC_src_foreign_toplevel_wlr_foreign_c_handle_minimized(struct wl_listener *listener, void *data)
{
	struct wlr_foreign_toplevel *wlr_toplevel =
		wl_container_of(listener, wlr_toplevel, on_view.minimized);
	assert(wlr_toplevel->handle);
	wlr_foreign_toplevel_handle_v1_set_minimized(wlr_toplevel->handle,
		wlr_toplevel->view->minimized);
}
static void
LABWC_src_foreign_toplevel_wlr_foreign_c_handle_fullscreened(struct wl_listener *listener, void *data)
{
	struct wlr_foreign_toplevel *wlr_toplevel =
		wl_container_of(listener, wlr_toplevel, on_view.fullscreened);
	assert(wlr_toplevel->handle);
	wlr_foreign_toplevel_handle_v1_set_fullscreen(wlr_toplevel->handle,
		wlr_toplevel->view->fullscreen);
}
static void
LABWC_src_foreign_toplevel_wlr_foreign_c_handle_activated(struct wl_listener *listener, void *data)
{
	struct wlr_foreign_toplevel *wlr_toplevel =
		wl_container_of(listener, wlr_toplevel, on_view.activated);
	assert(wlr_toplevel->handle);
	bool *activated = data;
	wlr_foreign_toplevel_handle_v1_set_activated(wlr_toplevel->handle,
		*activated);
}
void
wlr_foreign_toplevel_init(struct wlr_foreign_toplevel *wlr_toplevel,
		struct view *view)
{
	assert(server.foreign_toplevel_manager);
	wlr_toplevel->view = view;
	wlr_toplevel->handle = wlr_foreign_toplevel_handle_v1_create(
		server.foreign_toplevel_manager);
	if (!wlr_toplevel->handle) {
		return;
	}
	LABWC_src_foreign_toplevel_wlr_foreign_c_handle_new_app_id(&wlr_toplevel->on_view.new_app_id, NULL);
	LABWC_src_foreign_toplevel_wlr_foreign_c_handle_new_title(&wlr_toplevel->on_view.new_title, NULL);
	LABWC_src_foreign_toplevel_wlr_foreign_c_handle_new_outputs(&wlr_toplevel->on_view.new_outputs, NULL);
	LABWC_src_foreign_toplevel_wlr_foreign_c_handle_maximized(&wlr_toplevel->on_view.maximized, NULL);
	LABWC_src_foreign_toplevel_wlr_foreign_c_handle_minimized(&wlr_toplevel->on_view.minimized, NULL);
	LABWC_src_foreign_toplevel_wlr_foreign_c_handle_fullscreened(&wlr_toplevel->on_view.fullscreened, NULL);
	LABWC_src_foreign_toplevel_wlr_foreign_c_handle_activated(&wlr_toplevel->on_view.activated,
		&(bool){view == server.active_view});
	CONNECT_SIGNAL(wlr_toplevel->handle, &wlr_toplevel->on, request_maximize);
	CONNECT_SIGNAL(wlr_toplevel->handle, &wlr_toplevel->on, request_minimize);
	CONNECT_SIGNAL(wlr_toplevel->handle, &wlr_toplevel->on, request_fullscreen);
	CONNECT_SIGNAL(wlr_toplevel->handle, &wlr_toplevel->on, request_activate);
	CONNECT_SIGNAL(wlr_toplevel->handle, &wlr_toplevel->on, request_close);
	wlr_toplevel->on.handle_destroy.notify = LABWC_src_foreign_toplevel_wlr_foreign_c_handle_handle_destroy;
	wl_signal_add(&wlr_toplevel->handle->events.destroy, &wlr_toplevel->on.handle_destroy);
	CONNECT_SIGNAL(view, &wlr_toplevel->on_view, new_app_id);
	CONNECT_SIGNAL(view, &wlr_toplevel->on_view, new_title);
	CONNECT_SIGNAL(view, &wlr_toplevel->on_view, new_outputs);
	CONNECT_SIGNAL(view, &wlr_toplevel->on_view, maximized);
	CONNECT_SIGNAL(view, &wlr_toplevel->on_view, minimized);
	CONNECT_SIGNAL(view, &wlr_toplevel->on_view, fullscreened);
	CONNECT_SIGNAL(view, &wlr_toplevel->on_view, activated);
}
void
wlr_foreign_toplevel_set_parent(struct wlr_foreign_toplevel *wlr_toplevel,
		struct wlr_foreign_toplevel *parent)
{
	if (!wlr_toplevel->handle) {
		return;
	}
	wlr_foreign_toplevel_handle_v1_set_parent(wlr_toplevel->handle,
		parent ? parent->handle : NULL);
}
void
wlr_foreign_toplevel_finish(struct wlr_foreign_toplevel *wlr_toplevel)
{
	if (!wlr_toplevel->handle) {
		return;
	}
	wlr_foreign_toplevel_handle_v1_destroy(wlr_toplevel->handle);
	assert(!wlr_toplevel->handle);
}
#undef handle_activated
#undef handle_fullscreened
#undef handle_maximized
#undef handle_minimized
#undef handle_new_app_id
#undef handle_new_outputs
#undef handle_new_title
#undef handle_request_activate
#undef handle_request_close
#undef handle_request_fullscreen
#undef handle_request_maximize
#undef handle_request_minimize
#include <assert.h>
struct foreign_toplevel {
	struct wlr_foreign_toplevel wlr_toplevel;
	struct ext_foreign_toplevel ext_toplevel;
};
struct foreign_toplevel *
foreign_toplevel_create(struct view *view)
{
	assert(view);
	struct foreign_toplevel *toplevel = znew(*toplevel);
	wlr_foreign_toplevel_init(&toplevel->wlr_toplevel, view);
	ext_foreign_toplevel_init(&toplevel->ext_toplevel, view);
	return toplevel;
}
void
foreign_toplevel_set_parent(struct foreign_toplevel *toplevel, struct foreign_toplevel *parent)
{
	assert(toplevel);
	wlr_foreign_toplevel_set_parent(&toplevel->wlr_toplevel,
		parent ? &parent->wlr_toplevel : NULL);
}
void
foreign_toplevel_destroy(struct foreign_toplevel *toplevel)
{
	assert(toplevel);
	wlr_foreign_toplevel_finish(&toplevel->wlr_toplevel);
	ext_foreign_toplevel_finish(&toplevel->ext_toplevel);
	free(toplevel);
}
#define handle_new_app_id LABWC_src_foreign_toplevel_ext_foreign_c_handle_new_app_id
#define handle_new_title LABWC_src_foreign_toplevel_ext_foreign_c_handle_new_title
#include <assert.h>
static void
LABWC_src_foreign_toplevel_ext_foreign_c_handle_handle_destroy(struct wl_listener *listener, void *data)
{
	struct ext_foreign_toplevel *ext_toplevel =
		wl_container_of(listener, ext_toplevel, on.handle_destroy);
	wl_list_remove(&ext_toplevel->on.handle_destroy.link);
	wl_list_remove(&ext_toplevel->on_view.new_app_id.link);
	wl_list_remove(&ext_toplevel->on_view.new_title.link);
	ext_toplevel->handle = NULL;
}
static void
LABWC_src_foreign_toplevel_ext_foreign_c_handle_new_app_id(struct wl_listener *listener, void *data)
{
	struct ext_foreign_toplevel *ext_toplevel =
		wl_container_of(listener, ext_toplevel, on_view.new_app_id);
	assert(ext_toplevel->handle);
	struct wlr_ext_foreign_toplevel_handle_v1_state state = {
		.title = ext_toplevel->view->title,
		.app_id = ext_toplevel->view->app_id,
	};
	wlr_ext_foreign_toplevel_handle_v1_update_state(ext_toplevel->handle,
		&state);
}
static void
LABWC_src_foreign_toplevel_ext_foreign_c_handle_new_title(struct wl_listener *listener, void *data)
{
	struct ext_foreign_toplevel *ext_toplevel =
		wl_container_of(listener, ext_toplevel, on_view.new_title);
	assert(ext_toplevel->handle);
	struct wlr_ext_foreign_toplevel_handle_v1_state state = {
		.title = ext_toplevel->view->title,
		.app_id = ext_toplevel->view->app_id,
	};
	wlr_ext_foreign_toplevel_handle_v1_update_state(ext_toplevel->handle,
		&state);
}
void
ext_foreign_toplevel_init(struct ext_foreign_toplevel *ext_toplevel,
		struct view *view)
{
	assert(server.foreign_toplevel_list);
	ext_toplevel->view = view;
	struct wlr_ext_foreign_toplevel_handle_v1_state state = {
		.title = view->title,
		.app_id = view->app_id,
	};
	ext_toplevel->handle = wlr_ext_foreign_toplevel_handle_v1_create(
		server.foreign_toplevel_list, &state);
	if (!ext_toplevel->handle) {
		return;
	}
	ext_toplevel->handle->data = view;
	ext_toplevel->on.handle_destroy.notify = LABWC_src_foreign_toplevel_ext_foreign_c_handle_handle_destroy;
	wl_signal_add(&ext_toplevel->handle->events.destroy, &ext_toplevel->on.handle_destroy);
	CONNECT_SIGNAL(view, &ext_toplevel->on_view, new_app_id);
	CONNECT_SIGNAL(view, &ext_toplevel->on_view, new_title);
}
void
ext_foreign_toplevel_finish(struct ext_foreign_toplevel *ext_toplevel)
{
	if (!ext_toplevel->handle) {
		return;
	}
	wlr_ext_foreign_toplevel_handle_v1_destroy(ext_toplevel->handle);
	assert(!ext_toplevel->handle);
}
#undef handle_new_app_id
#undef handle_new_title
bool
box_intersects(struct wlr_box *box_a, struct wlr_box *box_b)
{
	if (wlr_box_empty(box_a) || wlr_box_empty(box_b)) {
		return false;
	}
	return box_a->x < box_b->x + box_b->width
		&& box_b->x < box_a->x + box_a->width
		&& box_a->y < box_b->y + box_b->height
		&& box_b->y < box_a->y + box_a->height;
}
void
box_union(struct wlr_box *box_dest, struct wlr_box *box_a, struct wlr_box *box_b)
{
	if (wlr_box_empty(box_a)) {
		*box_dest = *box_b;
		return;
	}
	if (wlr_box_empty(box_b)) {
		*box_dest = *box_a;
		return;
	}
	int x1 = MIN(box_a->x, box_b->x);
	int y1 = MIN(box_a->y, box_b->y);
	int x2 = MAX(box_a->x + box_a->width, box_b->x + box_b->width);
	int y2 = MAX(box_a->y + box_a->height, box_b->y + box_b->height);
	box_dest->x = x1;
	box_dest->y = y1;
	box_dest->width = x2 - x1;
	box_dest->height = y2 - y1;
}
void
box_center(int width, int height, const struct wlr_box *ref,
		const struct wlr_box *bound, int *x, int *y)
{
	*x = ref->x + (ref->width - width) / 2;
	*y = ref->y + (ref->height - height) / 2;
	if (*x < bound->x) {
		*x = bound->x;
	} else if (*x + width > bound->x + bound->width) {
		*x = bound->x + bound->width - width;
	}
	if (*y < bound->y) {
		*y = bound->y;
	} else if (*y + height > bound->y + bound->height) {
		*y = bound->y + bound->height - height;
	}
}
struct wlr_box
box_fit_within(int width, int height, struct wlr_box *bound)
{
	struct wlr_box box;
	if (width <= bound->width && height <= bound->height) {
		box.width = width;
		box.height = height;
	} else if (width * bound->height > height * bound->width) {
		box.width = bound->width;
		box.height = (height * bound->width + (width / 2)) / width;
	} else {
		box.width = (width * bound->height + (height / 2)) / height;
		box.height = bound->height;
	}
	box.x = bound->x + (bound->width - box.width) / 2;
	box.y = bound->y + (bound->height - box.height) / 2;
	return box;
}
struct wlr_fbox
box_to_fbox(struct wlr_box *box)
{
	return (struct wlr_fbox){
		.x = box->x,
		.y = box->y,
		.width = box->width,
		.height = box->height,
	};
}
#include <assert.h>
#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
static struct menuitem *LABWC_src_menu_menu_c_selected_item;
static bool
LABWC_src_menu_menu_c_is_unique_id(const char *id)
{
	struct menu *menu;
	wl_list_for_each(menu, &server.menus, link) {
		if (!strcmp(menu->id, id)) {
			return false;
		}
	}
	return true;
}
static struct menu *
LABWC_src_menu_menu_c_menu_create(struct menu *parent, const char *id,
		const char *label)
{
	if (!LABWC_src_menu_menu_c_is_unique_id(id)) {
	}
	struct menu *menu = znew(*menu);
	wl_list_append(&server.menus, &menu->link);
	wl_list_init(&menu->menuitems);
	menu->id = xstrdup(id);
	menu->label = xstrdup(label ? label : id);
	menu->parent = parent;
	return menu;
}
struct menu *
menu_get_by_id(const char *id)
{
	if (!id) {
		return NULL;
	}
	struct menu *menu;
	wl_list_for_each(menu, &server.menus, link) {
		if (!strcmp(menu->id, id)) {
			return menu;
		}
	}
	return NULL;
}
static void
LABWC_src_menu_menu_c_validate_menu(struct menu *menu)
{
	struct menuitem *item;
	struct action *action, *action_tmp;
	wl_list_for_each(item, &menu->menuitems, link) {
		wl_list_for_each_safe(action, action_tmp, &item->actions, link) {
			bool is_show_menu = action_is_show_menu(action);
			if (!action_is_valid(action) || is_show_menu) {
				if (is_show_menu) {
				}
				wl_list_remove(&action->link);
				action_free(action);
			}
		}
	}
}
static void
LABWC_src_menu_menu_c_validate(void)
{
	struct menu *menu;
	wl_list_for_each(menu, &server.menus, link) {
		LABWC_src_menu_menu_c_validate_menu(menu);
	}
}
static uint32_t
LABWC_src_menu_menu_c_read_unicode_char_lowercase(const char *first_byte, size_t *bytes_read)
{
	assert(bytes_read);
	if (string_null_or_empty(first_byte)) {
		*bytes_read = 0;
		return 0;
	}
	gunichar codepoint = g_utf8_get_char_validated(first_byte, -1);
	bool partial_read = (codepoint == (gunichar)-2);
	bool failed_read = (codepoint == (gunichar)-1);
	if (partial_read || failed_read) {
		*bytes_read = 1;
		return (uint32_t)(unsigned char)first_byte[0];
	}
	*bytes_read = (size_t)(g_utf8_next_char(first_byte) - first_byte);
	return (uint32_t)g_unichar_tolower(codepoint);
}
static void
LABWC_src_menu_menu_c_item_parse_accelerator(struct menuitem *item, const char *text)
{
	const char *accel_ptr = NULL;
	const char *underscore = strchr(text, '_');
	while (underscore) {
		if (underscore[1] == '_') {
			underscore = strchr(underscore + 2, '_');
		} else if (underscore[1] != '\0') {
			accel_ptr = underscore + 1;
			break;
		} else {
			break;
		}
	}
	size_t bytes_read = 0;
	if (!accel_ptr) {
		item->text = xstrdup(text);
		item->accelerator = LABWC_src_menu_menu_c_read_unicode_char_lowercase(text, &bytes_read);
	} else {
		item->use_markup = true;
		item->accelerator = LABWC_src_menu_menu_c_read_unicode_char_lowercase(accel_ptr, &bytes_read);
		item->text = strdup_printf("%.*s<u>%.*s</u>%s",
			(int)(accel_ptr - 1 - text), text,
			(int)bytes_read, accel_ptr,
			accel_ptr + bytes_read);
	}
}
static void
LABWC_src_menu_menu_c_unescape_underscores(char *text)
{
	if (!text) {
		return;
	}
	char *src = text;
	char *dst = text;
	while (*src) {
		if (*src == '_' && *(src + 1) == '_') {
			*dst++ = '_';
			src += 2;
		} else {
			*dst++ = *src++;
		}
	}
	*dst = '\0';
}
static struct menuitem *
LABWC_src_menu_menu_c_item_create(struct menu *menu, const char *text, bool show_arrow)
{
	assert(menu);
	assert(text);
	struct theme *theme = rc.theme;
	struct menuitem *menuitem = znew(*menuitem);
	menuitem->parent = menu;
	menuitem->selectable = true;
	menuitem->type = LAB_MENU_ITEM;
	menuitem->arrow = show_arrow ? "›" : NULL;
	LABWC_src_menu_menu_c_item_parse_accelerator(menuitem, text);
	LABWC_src_menu_menu_c_unescape_underscores(menuitem->text);
	menuitem->native_width = font_width(&rc.font_menuitem, text);
	if (menuitem->arrow) {
		menuitem->native_width += font_width(&rc.font_menuitem, menuitem->arrow)
					+ theme->menu_items_padding_x;
	}
	wl_list_append(&menu->menuitems, &menuitem->link);
	wl_list_init(&menuitem->actions);
	return menuitem;
}
static struct wlr_scene_tree *
LABWC_src_menu_menu_c_item_create_scene_for_state(struct menuitem *item, float *text_color,
	float *bg_color)
{
	struct menu *menu = item->parent;
	struct theme *theme = rc.theme;
	struct wlr_scene_tree *tree = lab_wlr_scene_tree_create(item->tree);
	int bg_width = menu->size.width - 2 * theme->menu_border_width;
	int arrow_width = item->arrow ?
		font_width(&rc.font_menuitem, item->arrow) + theme->menu_items_padding_x : 0;
	int label_max_width = bg_width - 2 * theme->menu_items_padding_x
		- arrow_width;
	if (label_max_width <= 0) {
		return tree;
	}
	lab_wlr_scene_rect_create(tree, bg_width, theme->menu_item_height, bg_color);
	struct scaled_font_buffer *label_buffer = scaled_font_buffer_create(tree);
	assert(label_buffer);
	scaled_font_buffer_update_markup(label_buffer, item->text, label_max_width,
		&rc.font_menuitem, text_color, bg_color, item->use_markup);
	int x = theme->menu_items_padding_x;
	int y = (theme->menu_item_height - label_buffer->height) / 2;
	wlr_scene_node_set_position(&label_buffer->scene_buffer->node, x, y);
	if (!item->arrow) {
		return tree;
	}
	struct scaled_font_buffer *arrow_buffer = scaled_font_buffer_create(tree);
	assert(arrow_buffer);
	scaled_font_buffer_update(arrow_buffer, item->arrow, -1,
		&rc.font_menuitem, text_color, bg_color);
	x += label_max_width + theme->menu_items_padding_x;
	y = (theme->menu_item_height - label_buffer->height) / 2;
	wlr_scene_node_set_position(&arrow_buffer->scene_buffer->node, x, y);
	return tree;
}
static void
LABWC_src_menu_menu_c_item_create_scene(struct menuitem *menuitem, int *item_y)
{
	assert(menuitem);
	assert(menuitem->type == LAB_MENU_ITEM);
	struct menu *menu = menuitem->parent;
	struct theme *theme = rc.theme;
	menuitem->tree = lab_wlr_scene_tree_create(menu->scene_tree);
	node_descriptor_create(&menuitem->tree->node, LAB_NODE_MENUITEM,
		  NULL, menuitem);
	menuitem->normal_tree = LABWC_src_menu_menu_c_item_create_scene_for_state(menuitem,
		theme->menu_items_text_color,
		theme->menu_items_bg_color);
	menuitem->selected_tree = LABWC_src_menu_menu_c_item_create_scene_for_state(menuitem,
		theme->menu_items_active_text_color,
		theme->menu_items_active_bg_color);
	wlr_scene_node_set_enabled(&menuitem->selected_tree->node, false);
	wlr_scene_node_set_position(&menuitem->tree->node,
		theme->menu_border_width, *item_y);
	*item_y += theme->menu_item_height;
}
static struct menuitem *
LABWC_src_menu_menu_c_separator_create(struct menu *menu, const char *label)
{
	assert(menu);
	struct menuitem *menuitem = znew(*menuitem);
	menuitem->parent = menu;
	menuitem->selectable = false;
	menuitem->type = string_null_or_empty(label) ? LAB_MENU_SEPARATOR_LINE
		: LAB_MENU_TITLE;
	if (menuitem->type == LAB_MENU_TITLE) {
		menuitem->text = xstrdup(label);
		menuitem->native_width = font_width(&rc.font_menuheader, label);
	}
	wl_list_append(&menu->menuitems, &menuitem->link);
	wl_list_init(&menuitem->actions);
	return menuitem;
}
static void
LABWC_src_menu_menu_c_separator_create_scene(struct menuitem *menuitem, int *item_y)
{
	assert(menuitem);
	assert(menuitem->type == LAB_MENU_SEPARATOR_LINE);
	struct menu *menu = menuitem->parent;
	struct theme *theme = rc.theme;
	menuitem->tree = lab_wlr_scene_tree_create(menu->scene_tree);
	node_descriptor_create(&menuitem->tree->node, LAB_NODE_MENUITEM,
		  NULL, menuitem);
	menuitem->normal_tree = lab_wlr_scene_tree_create(menuitem->tree);
	int bg_height = theme->menu_separator_line_thickness
		+ 2 * theme->menu_separator_padding_height;
	int bg_width = menu->size.width - 2 * theme->menu_border_width;
	int line_width = bg_width - 2 * theme->menu_separator_padding_width;
	if (line_width <= 0) {
		goto error;
	}
	lab_wlr_scene_rect_create(menuitem->normal_tree, bg_width, bg_height,
		theme->menu_items_bg_color);
	struct wlr_scene_rect *line_rect = lab_wlr_scene_rect_create(
		menuitem->normal_tree, line_width,
		theme->menu_separator_line_thickness,
		theme->menu_separator_color);
	wlr_scene_node_set_position(&line_rect->node,
		theme->menu_separator_padding_width,
		theme->menu_separator_padding_height);
error:
	wlr_scene_node_set_position(&menuitem->tree->node,
		theme->menu_border_width, *item_y);
	*item_y += bg_height;
}
static void
LABWC_src_menu_menu_c_title_create_scene(struct menuitem *menuitem, int *item_y)
{
	assert(menuitem);
	assert(menuitem->type == LAB_MENU_TITLE);
	struct menu *menu = menuitem->parent;
	struct theme *theme = rc.theme;
	float *bg_color = theme->menu_title_bg_color;
	float *text_color = theme->menu_title_text_color;
	menuitem->tree = lab_wlr_scene_tree_create(menu->scene_tree);
	node_descriptor_create(&menuitem->tree->node, LAB_NODE_MENUITEM,
		  NULL, menuitem);
	menuitem->normal_tree = lab_wlr_scene_tree_create(menuitem->tree);
	int bg_width = menu->size.width - 2 * theme->menu_border_width;
	int text_width = bg_width - 2 * theme->menu_items_padding_x;
	if (text_width <= 0) {
		goto error;
	}
	lab_wlr_scene_rect_create(menuitem->normal_tree,
		bg_width, theme->menu_header_height, bg_color);
	struct scaled_font_buffer *title_font_buffer =
		scaled_font_buffer_create(menuitem->normal_tree);
	assert(title_font_buffer);
	scaled_font_buffer_update(title_font_buffer, menuitem->text,
		text_width, &rc.font_menuheader, text_color, bg_color);
	int title_x = 0;
	switch (theme->menu_title_text_justify) {
	case LAB_JUSTIFY_CENTER:
		title_x = (bg_width - menuitem->native_width) / 2;
		title_x = MAX(title_x, 0);
		break;
	case LAB_JUSTIFY_LEFT:
		title_x = theme->menu_items_padding_x;
		break;
	case LAB_JUSTIFY_RIGHT:
		title_x = bg_width - menuitem->native_width
				- theme->menu_items_padding_x;
		break;
	}
	int title_y = (theme->menu_header_height - title_font_buffer->height) / 2;
	wlr_scene_node_set_position(&title_font_buffer->scene_buffer->node,
		title_x, title_y);
error:
	wlr_scene_node_set_position(&menuitem->tree->node,
		theme->menu_border_width, *item_y);
	*item_y += theme->menu_header_height;
}
static void LABWC_src_menu_menu_c_item_destroy(struct menuitem *item);
static void
LABWC_src_menu_menu_c_reset_menu(struct menu *menu)
{
	struct menuitem *item, *next;
	wl_list_for_each_safe(item, next, &menu->menuitems, link) {
		LABWC_src_menu_menu_c_item_destroy(item);
	}
	if (menu->scene_tree) {
		wlr_scene_node_destroy(&menu->scene_tree->node);
		menu->scene_tree = NULL;
	}
}
static void
LABWC_src_menu_menu_c_menu_create_scene(struct menu *menu)
{
	struct menuitem *item;
	struct theme *theme = rc.theme;
	assert(!menu->scene_tree);
	menu->scene_tree = lab_wlr_scene_tree_create(server.menu_tree);
	wlr_scene_node_set_enabled(&menu->scene_tree->node, false);
	menu->size.width = 0;
	wl_list_for_each(item, &menu->menuitems, link) {
		int width = item->native_width
			+ 2 * theme->menu_items_padding_x
			+ 2 * theme->menu_border_width;
		menu->size.width = MAX(menu->size.width, width);
	}
	menu->size.width = MAX(menu->size.width, theme->menu_min_width);
	menu->size.width = MIN(menu->size.width, theme->menu_max_width);
	int item_y = theme->menu_border_width;
	wl_list_for_each(item, &menu->menuitems, link) {
		assert(!item->tree);
		switch (item->type) {
		case LAB_MENU_ITEM:
			LABWC_src_menu_menu_c_item_create_scene(item, &item_y);
			break;
		case LAB_MENU_SEPARATOR_LINE:
			LABWC_src_menu_menu_c_separator_create_scene(item, &item_y);
			break;
		case LAB_MENU_TITLE:
			LABWC_src_menu_menu_c_title_create_scene(item, &item_y);
			break;
		}
	}
	menu->size.height = item_y + theme->menu_border_width;
	struct lab_scene_rect_options opts = {
		.border_colors = (float *[1]) {theme->menu_border_color},
		.nr_borders = 1,
		.border_width = theme->menu_border_width,
		.width = menu->size.width,
		.height = menu->size.height,
	};
	struct lab_scene_rect *bg_rect =
		lab_scene_rect_create(menu->scene_tree, &opts);
	wlr_scene_node_lower_to_bottom(&bg_rect->tree->node);
}
static void
LABWC_src_menu_menu_c_item_destroy(struct menuitem *item)
{
	wl_list_remove(&item->link);
	action_list_free(&item->actions);
	if (item->tree) {
		wlr_scene_node_destroy(&item->tree->node);
	}
	free(item->text);
	free(item);
}
static struct wlr_box
LABWC_src_menu_menu_c_get_item_anchor_rect(struct theme *theme, struct menuitem *item)
{
	struct menu *menu = item->parent;
	int menu_x = menu->scene_tree->node.x;
	int menu_y = menu->scene_tree->node.y;
	int overlap_x = theme->menu_overlap_x + theme->menu_border_width;
	int overlap_y = theme->menu_overlap_y - theme->menu_border_width;
	return (struct wlr_box) {
		.x = menu_x + overlap_x,
		.y = menu_y + item->tree->node.y + overlap_y,
		.width = menu->size.width - 2 * overlap_x,
		.height = theme->menu_item_height - 2 * overlap_y,
	};
}
static void
LABWC_src_menu_menu_c_menu_reposition(struct menu *menu, struct wlr_box anchor_rect)
{
	struct output *output = output_nearest_to(anchor_rect.x, anchor_rect.y);
	if (!output_is_usable(output)) {
		return;
	}
	struct wlr_box usable = output_usable_area_in_layout_coords(output);
	struct wlr_xdg_positioner_rules rules = {0};
	rules.size.width = menu->size.width;
	rules.size.height = menu->size.height;
	rules.anchor_rect = anchor_rect;
	if (menu->parent && menu->parent->align_left) {
		rules.anchor = XDG_POSITIONER_ANCHOR_TOP_LEFT;
		rules.gravity = XDG_POSITIONER_GRAVITY_BOTTOM_LEFT;
	} else {
		rules.anchor = XDG_POSITIONER_ANCHOR_TOP_RIGHT;
		rules.gravity = XDG_POSITIONER_GRAVITY_BOTTOM_RIGHT;
	}
	rules.constraint_adjustment =
		XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_FLIP_X
		| XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_SLIDE_X
		| XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_SLIDE_Y;
	if (!menu->parent) {
		rules.constraint_adjustment |=
			XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_FLIP_Y;
	}
	struct wlr_box box;
	wlr_xdg_positioner_rules_get_geometry(&rules, &box);
	wlr_xdg_positioner_rules_unconstrain_box(&rules, &usable, &box);
	wlr_scene_node_set_position(&menu->scene_tree->node, box.x, box.y);
	menu->align_left = (box.x < anchor_rect.x);
}
static void
LABWC_src_menu_menu_c_menu_hide_submenu(const char *id)
{
	struct menu *menu, *hide_menu;
	hide_menu = menu_get_by_id(id);
	if (!hide_menu) {
		return;
	}
	wl_list_for_each(menu, &server.menus, link) {
		struct menuitem *item, *next;
		wl_list_for_each_safe(item, next, &menu->menuitems, link) {
			if (item->submenu == hide_menu) {
				LABWC_src_menu_menu_c_item_destroy(item);
			}
		}
	}
}
static struct action *
LABWC_src_menu_menu_c_item_add_action(struct menuitem *item, const char *action_name)
{
	struct action *action = action_create(action_name);
	wl_list_append(&item->actions, &action->link);
	return action;
}
static void
LABWC_src_menu_menu_c_update_client_send_to_menu(void)
{
	struct menu *menu = menu_get_by_id("client-send-to-menu");
	assert(menu);
	LABWC_src_menu_menu_c_reset_menu(menu);
	struct workspace *workspace;
	struct buf buf = BUF_INIT;
	wl_list_for_each(workspace, &server.workspaces.all, link) {
		if (workspace == server.workspaces.current) {
			buf_add_fmt(&buf, ">%s<", workspace->name);
		} else {
			buf_add(&buf, workspace->name);
		}
		struct menuitem *item = LABWC_src_menu_menu_c_item_create(menu, buf.data,   false);
		struct action *action = LABWC_src_menu_menu_c_item_add_action(item, "SendToDesktop");
		action_arg_add_str(action, "to", workspace->name);
		buf_clear(&buf);
	}
	buf_reset(&buf);
	LABWC_src_menu_menu_c_separator_create(menu, "");
	struct menuitem *item = LABWC_src_menu_menu_c_item_create(menu,
		"Always on Visible Workspace", false);
	LABWC_src_menu_menu_c_item_add_action(item, "ToggleOmnipresent");
	LABWC_src_menu_menu_c_menu_create_scene(menu);
}
static void
LABWC_src_menu_menu_c_update_client_list_combined_menu(void)
{
	struct menu *menu = menu_get_by_id("client-list-combined-menu");
	assert(menu);
	LABWC_src_menu_menu_c_reset_menu(menu);
	struct menuitem *item;
	struct workspace *workspace;
	struct view *view;
	struct buf buffer = BUF_INIT;
	wl_list_for_each(workspace, &server.workspaces.all, link) {
		buf_add_fmt(&buffer, workspace == server.workspaces.current ? ">%s<" : "%s",
				workspace->name);
		LABWC_src_menu_menu_c_separator_create(menu, buffer.data);
		buf_clear(&buffer);
		wl_list_for_each(view, &server.views, link) {
			if (view->workspace == workspace) {
				if (!view->foreign_toplevel
						|| string_null_or_empty(view->title)) {
					continue;
				}
				if (view == server.active_view) {
					buf_add(&buffer, "*");
				}
				if (view->minimized) {
					buf_add_fmt(&buffer, "(%s)", view->title);
				} else {
					buf_add(&buffer, view->title);
				}
				item = LABWC_src_menu_menu_c_item_create(menu, buffer.data,
					  false);
				item->client_list_view = view;
				LABWC_src_menu_menu_c_item_add_action(item, "Focus");
				LABWC_src_menu_menu_c_item_add_action(item, "Raise");
				buf_clear(&buffer);
			}
		}
		item = LABWC_src_menu_menu_c_item_create(menu, "Go there...",
			  false);
		struct action *action = LABWC_src_menu_menu_c_item_add_action(item, "GoToDesktop");
		action_arg_add_str(action, "to", workspace->name);
	}
	buf_reset(&buffer);
	LABWC_src_menu_menu_c_menu_create_scene(menu);
}
static void
LABWC_src_menu_menu_c_init_windowmenu(void)
{
	struct menu *menu = LABWC_src_menu_menu_c_menu_create(NULL, "client-menu", "");
	struct menuitem *item;
	item = LABWC_src_menu_menu_c_item_create(menu, "Minimize", false);
	LABWC_src_menu_menu_c_item_add_action(item, "Iconify");
	item = LABWC_src_menu_menu_c_item_create(menu, "Maximize", false);
	LABWC_src_menu_menu_c_item_add_action(item, "ToggleMaximize");
	item = LABWC_src_menu_menu_c_item_create(menu, "Fullscreen", false);
	LABWC_src_menu_menu_c_item_add_action(item, "ToggleFullscreen");
	item = LABWC_src_menu_menu_c_item_create(menu, "Roll Up/Down", false);
	LABWC_src_menu_menu_c_item_add_action(item, "ToggleShade");
	item = LABWC_src_menu_menu_c_item_create(menu, "Decorations", false);
	LABWC_src_menu_menu_c_item_add_action(item, "ToggleDecorations");
	item = LABWC_src_menu_menu_c_item_create(menu, "Always on Top", false);
	LABWC_src_menu_menu_c_item_add_action(item, "ToggleAlwaysOnTop");
	item = LABWC_src_menu_menu_c_item_create(menu, "Workspace", true);
	item->submenu = menu_get_by_id("client-send-to-menu");
	item = LABWC_src_menu_menu_c_item_create(menu, "Close", false);
	LABWC_src_menu_menu_c_item_add_action(item, "Close");
	if (wl_list_length(&rc.workspace_config.workspaces) == 1) {
		LABWC_src_menu_menu_c_menu_hide_submenu("workspaces");
	}
}
void
menu_init(void)
{
	wl_list_init(&server.menus);
	LABWC_src_menu_menu_c_menu_create(NULL, "client-list-combined-menu", "Windows");
	LABWC_src_menu_menu_c_menu_create(NULL, "client-send-to-menu", "Workspace");
	LABWC_src_menu_menu_c_init_windowmenu();
	LABWC_src_menu_menu_c_validate();
}
static void
LABWC_src_menu_menu_c_nullify_item_pointing_to_this_menu(struct menu *menu)
{
	struct menu *iter;
	wl_list_for_each(iter, &server.menus, link) {
		struct menuitem *item;
		wl_list_for_each(item, &iter->menuitems, link) {
			if (item->submenu == menu) {
				item->submenu = NULL;
			}
		}
		if (iter->parent == menu) {
			iter->parent = NULL;
		}
		if (iter->selection.menu == menu) {
			iter->selection.menu = NULL;
		}
	}
}
static void
LABWC_src_menu_menu_c_menu_free(struct menu *menu)
{
	LABWC_src_menu_menu_c_nullify_item_pointing_to_this_menu(menu);
	if (server.menu_current == menu) {
		menu_close();
	}
	struct menuitem *item, *next;
	wl_list_for_each_safe(item, next, &menu->menuitems, link) {
		LABWC_src_menu_menu_c_item_destroy(item);
	}
	if (menu->scene_tree) {
		wlr_scene_node_destroy(&menu->scene_tree->node);
	}
	wl_list_remove(&menu->link);
	zfree(menu->id);
	zfree(menu->label);
	zfree(menu);
}
void
menu_finish(void)
{
	struct menu *menu, *tmp_menu;
	wl_list_for_each_safe(menu, tmp_menu, &server.menus, link) {
		LABWC_src_menu_menu_c_menu_free(menu);
	}
}
void
menu_on_view_destroy(struct view *view)
{
	struct menu *menu = menu_get_by_id("client-list-combined-menu");
	if (menu) {
		struct menuitem *item;
		wl_list_for_each(item, &menu->menuitems, link) {
			if (item->client_list_view == view) {
				item->client_list_view = NULL;
				action_list_free(&item->actions);
			}
		}
	}
}
static void
LABWC_src_menu_menu_c_menu_set_selection(struct menu *menu, struct menuitem *item)
{
	if (menu->selection.item) {
		wlr_scene_node_set_enabled(
			&menu->selection.item->normal_tree->node, true);
		wlr_scene_node_set_enabled(
			&menu->selection.item->selected_tree->node, false);
	}
	if (item) {
		wlr_scene_node_set_enabled(&item->normal_tree->node, false);
		wlr_scene_node_set_enabled(&item->selected_tree->node, true);
	}
	menu->selection.item = item;
}
static void
LABWC_src_menu_menu_c__close(struct menu *menu)
{
	if (menu->scene_tree) {
		wlr_scene_node_set_enabled(&menu->scene_tree->node, false);
	}
	LABWC_src_menu_menu_c_menu_set_selection(menu, NULL);
	if (menu->selection.menu) {
		LABWC_src_menu_menu_c__close(menu->selection.menu);
		menu->selection.menu = NULL;
	}
}
static void
LABWC_src_menu_menu_c_menu_close(struct menu *menu)
{
	if (!menu) {
		return;
	}
	LABWC_src_menu_menu_c__close(menu);
}
static void
LABWC_src_menu_menu_c_open_menu(struct menu *menu, struct wlr_box anchor_rect)
{
	if (!strcmp(menu->id, "client-list-combined-menu")) {
		LABWC_src_menu_menu_c_update_client_list_combined_menu();
	} else if (!strcmp(menu->id, "client-send-to-menu")) {
		LABWC_src_menu_menu_c_update_client_send_to_menu();
	}
	if (!menu->scene_tree) {
		LABWC_src_menu_menu_c_menu_create_scene(menu);
		assert(menu->scene_tree);
	}
	LABWC_src_menu_menu_c_menu_reposition(menu, anchor_rect);
	wlr_scene_node_set_enabled(&menu->scene_tree->node, true);
}
void
menu_open(struct menu *menu, int x, int y)
{
	assert(menu);
	if (server.input_mode != LAB_INPUT_STATE_PASSTHROUGH) {
		return;
	}
	assert(!server.menu_current);
	struct wlr_box anchor_rect = {.x = x, .y = y};
	LABWC_src_menu_menu_c_open_menu(menu, anchor_rect);
	server.menu_current = menu;
	LABWC_src_menu_menu_c_selected_item = NULL;
	seat_focus_override_begin(&server.seat,
		LAB_INPUT_STATE_MENU, LAB_CURSOR_DEFAULT);
}
static void
LABWC_src_menu_menu_c_menu_process_item_selection(struct menuitem *item)
{
	assert(item);
	if (item == LABWC_src_menu_menu_c_selected_item) {
		return;
	}
	LABWC_src_menu_menu_c_selected_item = item;
	if (!item->selectable) {
		return;
	}
	LABWC_src_menu_menu_c_menu_set_selection(item->parent, item);
	if (item->parent->selection.menu) {
		LABWC_src_menu_menu_c_menu_close(item->parent->selection.menu);
	}
	if (item->submenu) {
		item->submenu->triggered_by_view = item->parent->triggered_by_view;
		item->submenu->parent = item->parent;
		struct wlr_box anchor_rect =
			LABWC_src_menu_menu_c_get_item_anchor_rect(rc.theme, item);
		LABWC_src_menu_menu_c_open_menu(item->submenu, anchor_rect);
	}
	item->parent->selection.menu = item->submenu;
}
static struct menu *
LABWC_src_menu_menu_c_get_selection_leaf(void)
{
	struct menu *menu = server.menu_current;
	if (!menu) {
		return NULL;
	}
	while (menu->selection.menu) {
		if (!menu->selection.menu->selection.item) {
			return menu;
		}
		menu = menu->selection.menu;
	}
	return menu;
}
static void
LABWC_src_menu_menu_c_menu_item_select(bool forward)
{
	struct menu *menu = LABWC_src_menu_menu_c_get_selection_leaf();
	if (!menu) {
		return;
	}
	struct menuitem *item = NULL;
	struct menuitem *selection = menu->selection.item;
	struct wl_list *start = selection ? &selection->link : &menu->menuitems;
	struct wl_list *current = start;
	while (!item || !item->selectable) {
		current = forward ? current->next : current->prev;
		if (current == start) {
			return;
		}
		if (current == &menu->menuitems) {
			item = NULL;
			continue;
		}
		item = wl_container_of(current, item, link);
	}
	LABWC_src_menu_menu_c_menu_process_item_selection(item);
}
static bool
LABWC_src_menu_menu_c_menu_execute_item(struct menuitem *item)
{
	assert(item);
	if (item->submenu || !item->selectable) {
		return false;
	}
	LABWC_src_menu_menu_c_menu_close(server.menu_current);
	server.menu_current = NULL;
	seat_focus_override_end(&server.seat,   true);
	if (!strcmp(item->parent->id, "client-list-combined-menu")
			&& item->client_list_view) {
		if (item->client_list_view->shaded) {
			view_set_shade(item->client_list_view, false);
		}
		actions_run(item->client_list_view, &item->actions, NULL);
	} else {
		actions_run(item->parent->triggered_by_view, &item->actions, NULL);
	}
	return true;
}
void
menu_item_select_next(void)
{
	LABWC_src_menu_menu_c_menu_item_select(  true);
}
void
menu_item_select_previous(void)
{
	LABWC_src_menu_menu_c_menu_item_select(  false);
}
bool
menu_item_select_by_accelerator(uint32_t accelerator)
{
	struct menu *menu = LABWC_src_menu_menu_c_get_selection_leaf();
	if (!menu || wl_list_empty(&menu->menuitems)) {
		return false;
	}
	bool needs_exec = false;
	bool matched = false;
	struct menuitem *selection = menu->selection.item;
	struct wl_list *start = selection ? &selection->link : &menu->menuitems;
	struct wl_list *current = start;
	struct menuitem *item = NULL;
	struct menuitem *next_selection = NULL;
	do {
		current = current->next;
		if (current == &menu->menuitems) {
			continue;
		}
		item = wl_container_of(current, item, link);
		if (item->accelerator == accelerator) {
			if (!matched) {
				next_selection = item;
				needs_exec = true;
				matched = true;
			} else {
				needs_exec = false;
				break;
			}
		}
	} while (current != start);
	if (!next_selection) {
		return false;
	}
	LABWC_src_menu_menu_c_menu_process_item_selection(next_selection);
	if (needs_exec && next_selection->submenu) {
		needs_exec = false;
		menu_submenu_enter();
	}
	return needs_exec;
}
bool
menu_call_selected_actions(void)
{
	struct menu *menu = LABWC_src_menu_menu_c_get_selection_leaf();
	if (!menu || !menu->selection.item) {
		return false;
	}
	return LABWC_src_menu_menu_c_menu_execute_item(menu->selection.item);
}
void
menu_submenu_enter(void)
{
	struct menu *menu = LABWC_src_menu_menu_c_get_selection_leaf();
	if (!menu || !menu->selection.menu) {
		return;
	}
	struct wl_list *start = &menu->selection.menu->menuitems;
	struct wl_list *current = start;
	struct menuitem *item = NULL;
	while (!item || !item->selectable) {
		current = current->next;
		if (current == start) {
			return;
		}
		item = wl_container_of(current, item, link);
	}
	LABWC_src_menu_menu_c_menu_process_item_selection(item);
}
void
menu_submenu_leave(void)
{
	struct menu *menu = LABWC_src_menu_menu_c_get_selection_leaf();
	if (!menu || !menu->parent || !menu->parent->selection.item) {
		return;
	}
	LABWC_src_menu_menu_c_menu_process_item_selection(menu->parent->selection.item);
}
void
menu_process_cursor_motion(struct wlr_scene_node *node)
{
	assert(node && node->data);
	struct menuitem *item = node_menuitem_from_node(node);
	LABWC_src_menu_menu_c_menu_process_item_selection(item);
}
void
menu_close(void)
{
	assert(server.input_mode == LAB_INPUT_STATE_MENU);
	assert(server.menu_current);
	LABWC_src_menu_menu_c_menu_close(server.menu_current);
	server.menu_current = NULL;
	seat_focus_override_end(&server.seat,   true);
}
void
menu_reconfigure(void)
{
	menu_finish();
	server.menu_current = NULL;
	menu_init();
}
#include <assert.h>
static int
LABWC_src_interactive_c_max_move_scale(double pos_cursor, double pos_old, double size_old,
		double size_new)
{
	double anchor_frac = (pos_cursor - pos_old) / size_old;
	int pos_new = (int)(pos_cursor - (size_new * anchor_frac));
	if (pos_new < pos_old) {
		pos_new = (int)pos_old;
	}
	return pos_new;
}
void
interactive_anchor_to_cursor(struct wlr_box *geo)
{
	assert(server.input_mode == LAB_INPUT_STATE_MOVE);
	if (wlr_box_empty(geo)) {
		return;
	}
	server.grab_box.x = LABWC_src_interactive_c_max_move_scale(server.grab_x, server.grab_box.x,
		server.grab_box.width, geo->width);
	server.grab_box.y = LABWC_src_interactive_c_max_move_scale(server.grab_y, server.grab_box.y,
		server.grab_box.height, geo->height);
	server.grab_box.width = geo->width;
	server.grab_box.height = geo->height;
	geo->x = (int)(server.grab_box.x
		+ (server.seat.cursor->x - server.grab_x));
	geo->y = (int)(server.grab_box.y
		+ (server.seat.cursor->y - server.grab_y));
}
void
interactive_set_grab_context(const struct cursor_context *ctx)
{
	if (!ctx->view) {
		return;
	}
	if (server.input_mode != LAB_INPUT_STATE_PASSTHROUGH) {
		return;
	}
	server.grabbed_view = ctx->view;
	server.grab_x = server.seat.cursor->x;
	server.grab_y = server.seat.cursor->y;
	server.grab_box = ctx->view->current;
	server.resize_edges =
		cursor_get_resize_edges(server.seat.cursor, ctx);
}
void
interactive_begin(struct view *view, enum input_mode mode, enum lab_edge edges)
{
	assert(view);
	struct seat *seat = &server.seat;
	if (server.input_mode != LAB_INPUT_STATE_PASSTHROUGH
			|| view != server.grabbed_view) {
		return;
	}
	if (window_rules_get_property(view, "fixedPosition") == LAB_PROP_TRUE
			|| view_has_strut_partial(view)) {
		return;
	}
	enum lab_cursors cursor_shape = LAB_CURSOR_DEFAULT;
	switch (mode) {
	case LAB_INPUT_STATE_MOVE:
		if (view->fullscreen) {
			return;
		}
		view_store_natural_geometry(view);
		seat->region_prevent_snap = keyboard_get_all_modifiers(seat);
		cursor_shape = LAB_CURSOR_GRAB;
		break;
	case LAB_INPUT_STATE_RESIZE: {
		if (view->shaded || view->fullscreen) {
			return;
		}
		if (edges != LAB_EDGE_NONE) {
			server.resize_edges = edges;
		}
		enum view_axis maximized = view->maximized;
		if (server.resize_edges & LAB_EDGES_LEFT_RIGHT) {
			maximized &= (unsigned int)(~VIEW_AXIS_HORIZONTAL);
		}
		if (server.resize_edges & LAB_EDGES_TOP_BOTTOM) {
			maximized &= (unsigned int)(~VIEW_AXIS_VERTICAL);
		}
		view_set_maximized(view, maximized);
		view_set_untiled(view);
		cursor_shape = cursor_get_from_edge(server.resize_edges);
		break;
	}
	default:
		return;
	}
	seat_focus_override_begin(seat, mode, cursor_shape);
	if (mode == LAB_INPUT_STATE_MOVE && !view_is_floating(view)
			&& rc.unsnap_threshold <= 0) {
		struct wlr_box natural_geo = view->natural_geometry;
		interactive_anchor_to_cursor(&natural_geo);
		view_set_shade(view, false);
		view_set_maximized(view, VIEW_AXIS_NONE);
		view_set_untiled(view);
		view_move_resize(view, natural_geo);
	}
	if (rc.resize_indicator) {
		resize_indicator_show(view);
	}
	if (rc.window_edge_strength) {
		edges_calculate_visibility(view);
	}
}
bool
edge_from_cursor(struct seat *seat, struct output **dest_output,
		enum lab_edge *edge1, enum lab_edge *edge2)
{
	*dest_output = NULL;
	*edge1 = LAB_EDGE_NONE;
	*edge2 = LAB_EDGE_NONE;
	if (!view_is_floating(server.grabbed_view)) {
		return false;
	}
	if (rc.snap_edge_range_inner == 0 && rc.snap_edge_range_outer == 0) {
		return false;
	}
	struct output *output = output_nearest_to_cursor();
	if (!output_is_usable(output)) {
		return false;
	}
	*dest_output = output;
	double cursor_x = seat->cursor->x;
	double cursor_y = seat->cursor->y;
	int top_range = rc.snap_edge_range_outer;
	int bottom_range = rc.snap_edge_range_outer;
	int left_range = rc.snap_edge_range_outer;
	int right_range = rc.snap_edge_range_outer;
	if (wlr_output_layout_adjacent_output(server.output_layout, WLR_DIRECTION_UP,
			output->wlr_output, cursor_x, cursor_y)) {
		top_range = rc.snap_edge_range_inner;
	}
	if (wlr_output_layout_adjacent_output(server.output_layout, WLR_DIRECTION_DOWN,
			output->wlr_output, cursor_x, cursor_y)) {
		bottom_range = rc.snap_edge_range_inner;
	}
	if (wlr_output_layout_adjacent_output(server.output_layout, WLR_DIRECTION_LEFT,
			output->wlr_output, cursor_x, cursor_y)) {
		left_range = rc.snap_edge_range_inner;
	}
	if (wlr_output_layout_adjacent_output(server.output_layout, WLR_DIRECTION_RIGHT,
			output->wlr_output, cursor_x, cursor_y)) {
		right_range = rc.snap_edge_range_inner;
	}
	wlr_output_layout_output_coords(server.output_layout,
		output->wlr_output, &cursor_x, &cursor_y);
	struct wlr_box *area = &output->usable_area;
	int top = (int)(cursor_y - area->y);
	int bottom = (int)(area->y + area->height - cursor_y);
	int left = (int)(cursor_x - area->x);
	int right = (int)(area->x + area->width - cursor_x);
	if (top < top_range) {
		*edge1 = LAB_EDGE_TOP;
	} else if (bottom < bottom_range) {
		*edge1 = LAB_EDGE_BOTTOM;
	} else if (left < left_range) {
		*edge1 = LAB_EDGE_LEFT;
	} else if (right < right_range) {
		*edge1 = LAB_EDGE_RIGHT;
	} else {
		return false;
	}
	if (*edge1 == LAB_EDGE_TOP || *edge1 == LAB_EDGE_BOTTOM) {
		if (left < rc.snap_edge_corner_range) {
			*edge2 = LAB_EDGE_LEFT;
		} else if (right < rc.snap_edge_corner_range) {
			*edge2 = LAB_EDGE_RIGHT;
		}
	} else if (*edge1 == LAB_EDGE_LEFT || *edge1 == LAB_EDGE_RIGHT) {
		if (top < rc.snap_edge_corner_range) {
			*edge2 = LAB_EDGE_TOP;
		} else if (bottom < rc.snap_edge_corner_range) {
			*edge2 = LAB_EDGE_BOTTOM;
		}
	}
	return true;
}
static bool
LABWC_src_interactive_c_snap_to_edge(struct view *view)
{
	struct output *output;
	enum lab_edge edge1, edge2;
	if (!edge_from_cursor(&server.seat, &output, &edge1, &edge2)) {
		return false;
	}
	enum lab_edge edge = edge1 | edge2;
	view_set_output(view, output);
	if (edge == LAB_EDGE_TOP && rc.snap_top_maximize) {
		view_maximize(view, VIEW_AXIS_BOTH);
	} else {
		view_snap_to_edge(view, edge,   false,
			  false);
	}
	return true;
}
static bool
LABWC_src_interactive_c_snap_to_region(struct view *view)
{
	if (!regions_should_snap()) {
		return false;
	}
	struct region *region = regions_from_cursor();
	if (region) {
		view_snap_to_region(view, region);
		return true;
	}
	return false;
}
void
interactive_finish(struct view *view)
{
	assert(view);
	if (server.grabbed_view != view) {
		return;
	}
	if (server.input_mode == LAB_INPUT_STATE_MOVE) {
		if (!LABWC_src_interactive_c_snap_to_region(view)) {
			LABWC_src_interactive_c_snap_to_edge(view);
		}
	}
	interactive_cancel(view);
}
void
interactive_cancel(struct view *view)
{
	assert(view);
	if (server.grabbed_view != view) {
		return;
	}
	server.grabbed_view = NULL;
	if (server.input_mode != LAB_INPUT_STATE_MOVE
			&& server.input_mode != LAB_INPUT_STATE_RESIZE) {
		return;
	}
	overlay_finish(&server.seat);
	resize_indicator_hide(view);
	seat_focus_override_end(&server.seat,   true);
}
#include <assert.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>
uint32_t
parse_modifier(const char *symname)
{
	if (!strcmp(symname, "S")) {
		return WLR_MODIFIER_SHIFT;
	} else if (!strcmp(symname, "C")) {
		return WLR_MODIFIER_CTRL;
	} else if (!strcmp(symname, "A") || !strcmp(symname, "Mod1")) {
		return WLR_MODIFIER_ALT;
	} else if (!strcmp(symname, "W") || !strcmp(symname, "Mod4")) {
		return WLR_MODIFIER_LOGO;
	} else if (!strcmp(symname, "M") || !strcmp(symname, "Mod5")) {
		return WLR_MODIFIER_MOD5;
	} else if (!strcmp(symname, "H") || !strcmp(symname, "Mod3")) {
		return WLR_MODIFIER_MOD3;
	} else {
		return 0;
	}
}
bool
keybind_the_same(struct keybind *a, struct keybind *b)
{
	assert(a && b);
	if (a->modifiers != b->modifiers || a->keysyms_len != b->keysyms_len) {
		return false;
	}
	for (size_t i = 0; i < a->keysyms_len; i++) {
		if (a->keysyms[i] != b->keysyms[i]) {
			return false;
		}
	}
	return true;
}
bool
keybind_contains_keycode(struct keybind *keybind, xkb_keycode_t keycode)
{
	assert(keybind);
	for (size_t i = 0; i < keybind->keycodes_len; i++) {
		if (keybind->keycodes[i] == keycode) {
			return true;
		}
	}
	return false;
}
bool
keybind_contains_keysym(struct keybind *keybind, xkb_keysym_t keysym)
{
	assert(keybind);
	for (size_t i = 0; i < keybind->keysyms_len; i++) {
		if (keybind->keysyms[i] == keysym) {
			return true;
		}
	}
	return false;
}
static bool
LABWC_src_config_keybind_c_keybind_contains_any_keysym(struct keybind *keybind,
		const xkb_keysym_t *syms, int nr_syms)
{
	for (int i = 0; i < nr_syms; i++) {
		if (keybind_contains_keysym(keybind, syms[i])) {
			return true;
		}
	}
	return false;
}
static void
LABWC_src_config_keybind_c_update_keycodes_iter(struct xkb_keymap *keymap, xkb_keycode_t key, void *data)
{
	struct keybind *keybind;
	const xkb_keysym_t *syms;
	xkb_layout_index_t layout = *(xkb_layout_index_t *)data;
	int nr_syms = xkb_keymap_key_get_syms_by_level(keymap, key, layout, 0, &syms);
	if (!nr_syms) {
		return;
	}
	wl_list_for_each(keybind, &rc.keybinds, link) {
		if (keybind->keycodes_layout >= 0
				&& (xkb_layout_index_t)keybind->keycodes_layout != layout) {
			continue;
		}
		if (keybind->use_syms_only) {
			continue;
		}
		if (LABWC_src_config_keybind_c_keybind_contains_any_keysym(keybind, syms, nr_syms)) {
			if (keybind_contains_keycode(keybind, key)) {
				continue;
			}
			if (keybind->keycodes_len == MAX_KEYCODES) {
				continue;
			}
			keybind->keycodes[keybind->keycodes_len++] = key;
			keybind->keycodes_layout = (int)layout;
		}
	}
}
void
keybind_update_keycodes(void)
{
	struct xkb_state *state = server.seat.keyboard_group->keyboard.xkb_state;
	struct xkb_keymap *keymap = xkb_state_get_keymap(state);
	struct keybind *keybind;
	wl_list_for_each(keybind, &rc.keybinds, link) {
		keybind->keycodes_len = 0;
		keybind->keycodes_layout = -1;
	}
	xkb_layout_index_t layouts = xkb_keymap_num_layouts(keymap);
	for (xkb_layout_index_t i = 0; i < layouts; i++) {
		xkb_keymap_key_for_each(keymap, LABWC_src_config_keybind_c_update_keycodes_iter, &i);
	}
}
struct keybind *
keybind_create(const char *keybind)
{
	xkb_keysym_t sym;
	struct keybind *k = znew(*k);
	xkb_keysym_t keysyms[MAX_KEYSYMS];
	gchar **symnames = g_strsplit(keybind, "-", -1);
	for (size_t i = 0; symnames[i]; i++) {
		const char *symname = symnames[i];
		if (!symname[0]) {
			if (symnames[i + 1] && !symnames[i + 1][0]) {
				continue;
			}
			symname = "-";
		}
		uint32_t modifier = parse_modifier(symname);
		if (modifier != 0) {
			k->modifiers |= modifier;
		} else {
			sym = xkb_keysym_from_name(symname, XKB_KEYSYM_CASE_INSENSITIVE);
			if (sym == XKB_KEY_NoSymbol && g_utf8_strlen(symname, -1) == 1) {
				gunichar codepoint = g_utf8_get_char_validated(symname, -1);
				if (codepoint != (gunichar)-1) {
					sym = xkb_utf32_to_keysym(codepoint);
				}
			}
			sym = xkb_keysym_to_lower(sym);
			if (sym == XKB_KEY_NoSymbol) {
				free(k);
				k = NULL;
				break;
			}
			keysyms[k->keysyms_len] = sym;
			k->keysyms_len++;
			if (k->keysyms_len == MAX_KEYSYMS) {
				break;
			}
		}
	}
	g_strfreev(symnames);
	if (!k) {
		return NULL;
	}
	wl_list_append(&rc.keybinds, &k->link);
	k->keysyms = xmalloc(k->keysyms_len * sizeof(xkb_keysym_t));
	memcpy(k->keysyms, keysyms, k->keysyms_len * sizeof(xkb_keysym_t));
	wl_list_init(&k->actions);
	return k;
}
void
keybind_destroy(struct keybind *keybind)
{
	assert(wl_list_empty(&keybind->actions));
	zfree(keybind->keysyms);
	zfree(keybind);
}
#include <string.h>
#include <strings.h>
static void
LABWC_src_config_libinput_c_libinput_category_init(struct libinput_category *l)
{
	l->type = LAB_LIBINPUT_DEVICE_DEFAULT;
	l->name = NULL;
	l->pointer_speed = 0;
	l->natural_scroll = -1;
	l->left_handed = -1;
	l->tap = LIBINPUT_CONFIG_TAP_ENABLED;
	l->tap_button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;
	l->tap_and_drag = -1;
	l->drag_lock = -1;
	l->three_finger_drag = -1;
	l->accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT;
	l->middle_emu = -1;
	l->dwt = -1;
	l->click_method = -1;
	l->scroll_method = -1;
	l->scroll_button = -1;
	l->send_events_mode = -1;
	l->have_calibration_matrix = false;
	l->scroll_factor = 1.0;
}
enum lab_libinput_device_type
get_device_type(const char *s)
{
	if (string_null_or_empty(s)) {
		return LAB_LIBINPUT_DEVICE_NONE;
	}
	if (!strcasecmp(s, "default")) {
		return LAB_LIBINPUT_DEVICE_DEFAULT;
	}
	if (!strcasecmp(s, "touchpad")) {
		return LAB_LIBINPUT_DEVICE_TOUCHPAD;
	}
	if (!strcasecmp(s, "non-touch")) {
		return LAB_LIBINPUT_DEVICE_NON_TOUCH;
	}
	return LAB_LIBINPUT_DEVICE_NONE;
}
const char *
libinput_device_type_name(enum lab_libinput_device_type type)
{
	switch (type) {
	case LAB_LIBINPUT_DEVICE_NONE:
		break;
	case LAB_LIBINPUT_DEVICE_DEFAULT:
		return "default";
	case LAB_LIBINPUT_DEVICE_TOUCHPAD:
		return "touchpad";
	case LAB_LIBINPUT_DEVICE_NON_TOUCH:
		return "non-touch";
	}
	return "(none)";
}
struct libinput_category *
libinput_category_create(void)
{
	struct libinput_category *l = znew(*l);
	LABWC_src_config_libinput_c_libinput_category_init(l);
	wl_list_append(&rc.libinput_categories, &l->link);
	return l;
}
struct libinput_category *
libinput_category_get_default(void)
{
	struct libinput_category *l;
	wl_list_for_each_reverse(l, &rc.libinput_categories, link) {
		if (l->type == LAB_LIBINPUT_DEVICE_DEFAULT) {
			return l;
		}
	}
	return NULL;
}
#include <assert.h>
#include <linux/input-event-codes.h>
#include <strings.h>
uint32_t
mousebind_button_from_str(const char *str, uint32_t *modifiers)
{
	assert(str);
	if (modifiers) {
		*modifiers = 0;
		while (strlen(str) >= 2 && str[1] == '-') {
			char modname[2] = {str[0], 0};
			uint32_t parsed_modifier = parse_modifier(modname);
			if (!parsed_modifier) {
				goto invalid;
			}
			*modifiers |= parsed_modifier;
			str += 2;
		}
	}
	if (!strcasecmp(str, "Left")) {
		return BTN_LEFT;
	} else if (!strcasecmp(str, "Right")) {
		return BTN_RIGHT;
	} else if (!strcasecmp(str, "Middle")) {
		return BTN_MIDDLE;
	} else if (!strcasecmp(str, "Side")) {
		return BTN_SIDE;
	} else if (!strcasecmp(str, "Extra")) {
		return BTN_EXTRA;
	} else if (!strcasecmp(str, "Forward")) {
		return BTN_FORWARD;
	} else if (!strcasecmp(str, "Back")) {
		return BTN_BACK;
	} else if (!strcasecmp(str, "Task")) {
		return BTN_TASK;
	}
invalid:
	return UINT32_MAX;
}
enum direction
mousebind_direction_from_str(const char *str, uint32_t *modifiers)
{
	assert(str);
	if (modifiers) {
		*modifiers = 0;
		while (strlen(str) >= 2 && str[1] == '-') {
			char modname[2] = {str[0], 0};
			uint32_t parsed_modifier = parse_modifier(modname);
			if (!parsed_modifier) {
				goto invalid;
			}
			*modifiers |= parsed_modifier;
			str += 2;
		}
	}
	if (!strcasecmp(str, "Left")) {
		return LAB_DIRECTION_LEFT;
	} else if (!strcasecmp(str, "Right")) {
		return LAB_DIRECTION_RIGHT;
	} else if (!strcasecmp(str, "Up")) {
		return LAB_DIRECTION_UP;
	} else if (!strcasecmp(str, "Down")) {
		return LAB_DIRECTION_DOWN;
	}
invalid:
	return LAB_DIRECTION_INVALID;
}
enum mouse_event
mousebind_event_from_str(const char *str)
{
	assert(str);
	if (!strcasecmp(str, "doubleclick")) {
		return MOUSE_ACTION_DOUBLECLICK;
	} else if (!strcasecmp(str, "click")) {
		return MOUSE_ACTION_CLICK;
	} else if (!strcasecmp(str, "press")) {
		return MOUSE_ACTION_PRESS;
	} else if (!strcasecmp(str, "release")) {
		return MOUSE_ACTION_RELEASE;
	} else if (!strcasecmp(str, "drag")) {
		return MOUSE_ACTION_DRAG;
	} else if (!strcasecmp(str, "scroll")) {
		return MOUSE_ACTION_SCROLL;
	}
	return MOUSE_ACTION_NONE;
}
bool
mousebind_the_same(struct mousebind *a, struct mousebind *b)
{
	assert(a && b);
	return a->context == b->context
		&& a->button == b->button
		&& a->direction == b->direction
		&& a->mouse_event == b->mouse_event
		&& a->modifiers == b->modifiers;
}
struct mousebind *
mousebind_create(const char *context)
{
	if (!context) {
		return NULL;
	}
	struct mousebind *m = znew(*m);
	m->context = node_type_parse(context);
	if (m->context != LAB_NODE_NONE) {
		wl_list_append(&rc.mousebinds, &m->link);
	}
	wl_list_init(&m->actions);
	return m;
}
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
uint32_t
parse_privileged_interface(const char *name)
{
	static const char * const ifaces[] = {
		"wp_drm_lease_device_v1",
		"zwlr_gamma_control_manager_v1",
		"zwlr_output_manager_v1",
		"zwlr_output_power_manager_v1",
		"zwp_input_method_manager_v2",
		"zwlr_virtual_pointer_manager_v1",
		"zwp_virtual_keyboard_manager_v1",
		"zwlr_export_dmabuf_manager_v1",
		"zwlr_screencopy_manager_v1",
		"ext_data_control_manager_v1",
		"zwlr_data_control_manager_v1",
		"wp_security_context_manager_v1",
		"ext_idle_notifier_v1",
		"zwlr_foreign_toplevel_manager_v1",
		"ext_foreign_toplevel_list_v1",
		"ext_session_lock_manager_v1",
		"zwlr_layer_shell_v1",
		"ext_workspace_manager_v1",
		"ext_image_copy_capture_manager_v1",
		"ext_output_image_capture_source_manager_v1",
	};
	static_assert(ARRAY_SIZE(ifaces) <= 32,
		"return type too small for amount of privileged protocols");
	for (size_t i = 0; i < ARRAY_SIZE(ifaces); i++) {
		if (!strcmp(name, ifaces[i])) {
			return 1 << i;
		}
	}
	return 0;
}
static void
LABWC_src_config_rcxml_c_init_font_defaults(struct font *font)
{
	font->size = 10;
	font->slant = PANGO_STYLE_NORMAL;
	font->weight = PANGO_WEIGHT_NORMAL;
}
static void
LABWC_src_config_rcxml_c_fill_default_title_layout(void)
{
	rc.nr_title_buttons_left = 0;
	rc.nr_title_buttons_right = 0;
	rc.title_buttons_right[rc.nr_title_buttons_right++] =
		LAB_NODE_BUTTON_ICONIFY;
	rc.title_buttons_right[rc.nr_title_buttons_right++] =
		LAB_NODE_BUTTON_MAXIMIZE;
	rc.title_buttons_right[rc.nr_title_buttons_right++] =
		LAB_NODE_BUTTON_CLOSE;
}
static void
LABWC_src_config_rcxml_c_init_defaults(void)
{
	static bool has_run;
	if (!has_run) {
		wl_list_init(&rc.usable_area_overrides);
		wl_list_init(&rc.keybinds);
		wl_list_init(&rc.mousebinds);
		wl_list_init(&rc.libinput_categories);
		wl_list_init(&rc.workspace_config.workspaces);
		wl_list_init(&rc.regions);
		wl_list_init(&rc.window_rules);
		}
	has_run = true;
	rc.placement_policy = LAB_PLACE_CASCADE;
	rc.placement_cascade_offset_x = 0;
	rc.placement_cascade_offset_y = 0;
	rc.xdg_shell_server_side_deco = true;
	rc.hide_maximized_window_titlebar = false;
	rc.ssd_keep_border = true;
	rc.corner_radius = 8;
	rc.shadows_enabled = true;
	rc.shadows_on_tiled = false;
	rc.gap = 0;
#ifdef FULLSCREEN_TEARING
	rc.allow_tearing = LAB_TEARING_FULLSCREEN_FORCED;
#else
	rc.allow_tearing = LAB_TEARING_DISABLED;
#endif
	rc.auto_enable_outputs = true;
	rc.reuse_output_mode = false;
	rc.allowed_interfaces = UINT32_MAX;
	rc.xwayland_persistence = false;
	rc.primary_selection = true;
	LABWC_src_config_rcxml_c_init_font_defaults(&rc.font_activewindow);
	LABWC_src_config_rcxml_c_init_font_defaults(&rc.font_inactivewindow);
	LABWC_src_config_rcxml_c_init_font_defaults(&rc.font_menuheader);
	LABWC_src_config_rcxml_c_init_font_defaults(&rc.font_menuitem);
	LABWC_src_config_rcxml_c_init_font_defaults(&rc.font_osd);
	rc.focus_follow_mouse = false;
	rc.focus_follow_mouse_requires_movement = true;
	rc.raise_on_focus = false;
	rc.raise_on_focus_delay_ms = 0;
	rc.doubleclick_time = 200;
	rc.repeat_rate = 45;
	rc.repeat_delay = 250;
	rc.kb_numlock_enable = LAB_STATE_UNSPECIFIED;
	rc.kb_layout_per_window = false;
	rc.screen_edge_strength = 20;
	rc.window_edge_strength = 20;
	rc.unsnap_threshold = 20;
	rc.unmaximize_threshold = 150;
	rc.snap_edge_range_inner = 10;
	rc.snap_edge_range_outer = 10;
	rc.snap_edge_corner_range = 50;
	rc.snap_overlay_enabled = true;
	rc.snap_overlay_delay_inner = 500;
	rc.snap_overlay_delay_outer = 500;
	rc.snap_top_maximize = true;
	rc.snap_tiling_events_mode = LAB_TILING_EVENTS_ALWAYS;
	rc.resize_indicator = LAB_RESIZE_INDICATOR_NEVER;
	rc.resize_draw_contents = true;
	rc.resize_corner_range = -1;
	rc.resize_minimum_area = 8;
	rc.workspace_config.popuptime = INT_MIN;
	rc.menu_ignore_button_release_period = 250;
	LABWC_src_config_rcxml_c_fill_default_title_layout();
}
static void
LABWC_src_config_rcxml_c_load_default_key_bindings(void)
{
	struct keybind *k;
	struct action *action;
	for (int i = 0; key_combos[i].binding; i++) {
		struct key_combos *current = &key_combos[i];
		k = keybind_create(current->binding);
		if (!k) {
			continue;
		}
		action = action_create(current->action);
		if (!action) {
			keybind_destroy(k);
			continue;
		}
		wl_list_append(&k->actions, &action->link);
		for (size_t j = 0; j < ARRAY_SIZE(current->attributes); j++) {
			if (!current->attributes[j].name
					|| !current->attributes[j].value) {
				break;
			}
			action_arg_from_xml_node(action,
				current->attributes[j].name,
				current->attributes[j].value);
		}
		if (actions_contain_toggle_keybinds(&k->actions)) {
			k->override_inhibition = true;
		}
	}
}
static void
LABWC_src_config_rcxml_c_load_default_mouse_bindings(void)
{
	struct mousebind *m;
	struct action *action;
	for (int i = 0; mouse_combos[i].context; i++) {
		struct mouse_combos *current = &mouse_combos[i];
		if (i == 0
				|| strcmp(current->context, mouse_combos[i - 1].context)
				|| strcmp(current->button, mouse_combos[i - 1].button)
				|| strcmp(current->event, mouse_combos[i - 1].event)) {
			m = mousebind_create(current->context);
			m->mouse_event = mousebind_event_from_str(current->event);
			if (m->mouse_event == MOUSE_ACTION_SCROLL) {
				m->direction = mousebind_direction_from_str(current->button,
					&m->modifiers);
			} else {
				m->button = mousebind_button_from_str(current->button,
					&m->modifiers);
			}
		}
		action = action_create(current->action);
		if (action) {
			wl_list_append(&m->actions, &action->link);
			for (size_t j = 0; j < ARRAY_SIZE(current->attributes); j++) {
				if (!current->attributes[j].name
						|| !current->attributes[j].value) {
					break;
				}
				action_arg_from_xml_node(action,
					current->attributes[j].name,
					current->attributes[j].value);
			}
		}
	}
}
static void
LABWC_src_config_rcxml_c_post_processing(void)
{
	LABWC_src_config_rcxml_c_load_default_key_bindings();
	LABWC_src_config_rcxml_c_load_default_mouse_bindings();
	rc.font_activewindow.name = xstrdup("sans");
	rc.font_inactivewindow.name = xstrdup("sans");
	rc.font_menuheader.name = xstrdup("sans");
	rc.font_menuitem.name = xstrdup("sans");
	rc.font_osd.name = xstrdup("sans");
	struct libinput_category *l = libinput_category_create();
	(void)l;
	assert(l && libinput_category_get_default() == l);
	for (int i = 1; i <= 9; i++) {
		struct workspace_config *conf = znew(*conf);
		conf->name = strdup_printf("%d", i);
		wl_list_append(&rc.workspace_config.workspaces, &conf->link);
	}
	rc.workspace_config.popuptime = 1000;
}
void
rcxml_init(void)
{
	LABWC_src_config_rcxml_c_init_defaults();
	LABWC_src_config_rcxml_c_post_processing();
}
void
rcxml_finish(void)
{
	zfree(rc.font_activewindow.name);
	zfree(rc.font_inactivewindow.name);
	zfree(rc.font_menuheader.name);
	zfree(rc.font_menuitem.name);
	zfree(rc.font_osd.name);
	zfree(rc.workspace_config.initial_workspace_name);
	rc.nr_title_buttons_left = 0;
	rc.nr_title_buttons_right = 0;
	struct usable_area_override *area, *area_tmp;
	wl_list_for_each_safe(area, area_tmp, &rc.usable_area_overrides, link) {
		wl_list_remove(&area->link);
		zfree(area->output);
		zfree(area);
	}
	struct keybind *k, *k_tmp;
	wl_list_for_each_safe(k, k_tmp, &rc.keybinds, link) {
		wl_list_remove(&k->link);
		action_list_free(&k->actions);
		keybind_destroy(k);
	}
	struct mousebind *m, *m_tmp;
	wl_list_for_each_safe(m, m_tmp, &rc.mousebinds, link) {
		wl_list_remove(&m->link);
		action_list_free(&m->actions);
		zfree(m);
	}
	struct libinput_category *l, *l_tmp;
	wl_list_for_each_safe(l, l_tmp, &rc.libinput_categories, link) {
		wl_list_remove(&l->link);
		zfree(l->name);
		zfree(l);
	}
	struct workspace_config *w, *w_tmp;
	wl_list_for_each_safe(w, w_tmp, &rc.workspace_config.workspaces, link) {
		wl_list_remove(&w->link);
		zfree(w->name);
		zfree(w);
	}
	regions_destroy(NULL, &rc.regions);
}
#include <assert.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#if WLR_HAS_DRM_BACKEND
#else
	#define wlr_backend_is_drm(backend) (false)
#endif
static const char * const LABWC_src_config_session_c_env_vars[] = {
	"WAYLAND_DISPLAY",
	"XDG_CURRENT_DESKTOP",
	"XCURSOR_SIZE",
	"XCURSOR_THEME",
	"XDG_SESSION_TYPE",
	"LABWC_PID",
	"LABWC_VER",
	NULL
};
#define LAB_ENV_VAR_MAX_SIZE 1024
static void
LABWC_src_config_session_c_process_line(char *line)
{
	if (string_null_or_empty(line) || line[0] == '#') {
		return;
	}
	char *key = NULL;
	char *p = strchr(line, '=');
	if (!p) {
		return;
	}
	*p = '\0';
	key = string_strip(line);
	if (string_null_or_empty(key)) {
		return;
	}
	struct buf value = BUF_INIT;
	buf_add(&value, string_strip(++p));
	if (value.len > LAB_ENV_VAR_MAX_SIZE) {
		goto err;
	}
	buf_expand_shell_variables(&value);
	buf_expand_tilde(&value);
	setenv(key, value.data, 1);
err:
	buf_reset(&value);
}
#undef LAB_ENV_VAR_MAX_SIZE
static bool
LABWC_src_config_session_c_read_environment_file(const char *filename)
{
	char *line = NULL;
	size_t len = 0;
	FILE *stream = fopen(filename, "r");
	if (!stream) {
		return false;
	}
	while (getline(&line, &len, stream) != -1) {
		char *p = strrchr(line, '\n');
		if (p) {
			*p = '\0';
		}
		LABWC_src_config_session_c_process_line(line);
	}
	free(line);
	fclose(stream);
	return true;
}
static char *
LABWC_src_config_session_c_strdup_env_path_validate(const char *prefix, struct dirent *dirent)
{
	assert(prefix);
	char *full_path = strdup_printf("%s/%s", prefix, dirent->d_name);
	if (!full_path) {
		return NULL;
	}
	struct stat statbuf;
	if (stat(full_path, &statbuf) == 0 && S_ISREG(statbuf.st_mode)) {
		return full_path;
	}
	free(full_path);
	return NULL;
}
static int
LABWC_src_config_session_c_env_file_filter(const struct dirent *dirent)
{
	return str_endswith(dirent->d_name, ".env");
}
static bool
LABWC_src_config_session_c_read_environment_dir(const char *path_prefix)
{
	bool success = false;
	char *path = strdup_printf("%s.d", path_prefix);
	struct dirent **dirlist = NULL;
	errno = 0;
	int num_entries = scandir(path, &dirlist, LABWC_src_config_session_c_env_file_filter, alphasort);
	if (num_entries < 0) {
		goto env_dir_cleanup;
	}
	for (int i = 0; i < num_entries; i++) {
		char *env_file_path = LABWC_src_config_session_c_strdup_env_path_validate(path, dirlist[i]);
		free(dirlist[i]);
		if (!env_file_path) {
			continue;
		}
		if (LABWC_src_config_session_c_read_environment_file(env_file_path)) {
			success = true;
		}
		free(env_file_path);
	}
	free(dirlist);
env_dir_cleanup:
	free(path);
	return success;
}
static void
LABWC_src_config_session_c_backend_check_drm(struct wlr_backend *backend, void *is_drm)
{
	if (wlr_backend_is_drm(backend)) {
		*(bool *)is_drm = true;
	}
}
static bool
LABWC_src_config_session_c_should_update_activation(void)
{
	static const char *act_env = "LABWC_UPDATE_ACTIVATION_ENV";
	char *env = getenv(act_env);
	if (env) {
		int enabled = parse_bool(env, -1);
		if (enabled == -1) {
		} else {
			return enabled;
		}
	}
	bool have_drm = false;
	wlr_multi_for_each_backend(server.backend, LABWC_src_config_session_c_backend_check_drm, &have_drm);
	return have_drm;
}
static void
LABWC_src_config_session_c_execute_update(const char *env_keys, const char *env_unset_keys, bool initialize)
{
	char *cmd =
		strdup_printf("dbus-update-activation-environment %s",
			initialize ? env_keys : env_unset_keys);
	spawn_sync_no_shell(cmd);
	free(cmd);
	cmd = strdup_printf("systemctl --user %s %s",
		initialize ? "import-environment" : "unset-environment", env_keys);
	spawn_sync_no_shell(cmd);
	free(cmd);
}
static void
LABWC_src_config_session_c_update_activation_env(bool initialize)
{
	if (!LABWC_src_config_session_c_should_update_activation()) {
		return;
	}
	if (!getenv("DBUS_SESSION_BUS_ADDRESS")) {
		return;
	}
	char *env_keys = str_join(LABWC_src_config_session_c_env_vars, "%s", " ");
	char *env_unset_keys = initialize ? NULL : str_join(LABWC_src_config_session_c_env_vars, "%s=", " ");
	LABWC_src_config_session_c_execute_update(env_keys, env_unset_keys, initialize);
	free(env_keys);
	free(env_unset_keys);
#if HAVE_XWAYLAND
	if (server.xwayland) {
		LABWC_src_config_session_c_execute_update("DISPLAY", "DISPLAY=", initialize);
	}
#endif
}
void
session_environment_init(void)
{
	setenv("XDG_CURRENT_DESKTOP", "labwc:wlroots", 0);
	setenv("_JAVA_AWT_WM_NONREPARENTING", "1", 0);
	struct wl_list paths;
	paths_config_create(&paths, "environment");
	struct path *path;
	wl_list_for_each(path, &paths, link) {
		bool success = LABWC_src_config_session_c_read_environment_file(path->string);
		success |= LABWC_src_config_session_c_read_environment_dir(path->string);
		if (success) {
			break;
		}
	}
	paths_destroy(&paths);
}
static const char * const LABWC_src_config_session_c_autostart_commands[] = {
	"jtlab",
	"gentoo-pipewire-launcher",
	"foot --server",
	NULL,
};
static const char * const LABWC_src_config_session_c_shutdown_commands[] = {
	NULL,
};
static void
LABWC_src_config_session_c_run_commands(const char * const *commands)
{
	for (size_t i = 0; commands[i]; i++) {
		char *cmd = strdup_printf("sh -c '%s >/dev/null 2>&1'", commands[i]);
		spawn_async_no_shell(cmd);
		free(cmd);
	}
}
void
session_autostart_init(void)
{
	LABWC_src_config_session_c_update_activation_env(  true);
	LABWC_src_config_session_c_run_commands(LABWC_src_config_session_c_autostart_commands);
}
void
session_shutdown(void)
{
	LABWC_src_config_session_c_run_commands(LABWC_src_config_session_c_shutdown_commands);
	LABWC_src_config_session_c_update_activation_env(  false);
}
#undef LAB_ENV_VAR_MAX_SIZE
#undef wlr_backend_is_drm
#include <assert.h>
static struct wl_list LABWC_src_decorations_kde_deco_c_decorations;
static struct wlr_server_decoration_manager *LABWC_src_decorations_kde_deco_c_kde_deco_mgr;
struct LABWC_src_decorations_kde_deco_c_kde_deco {
	struct wl_list link;
	struct wlr_server_decoration *wlr_kde_decoration;
	struct view *view;
	struct wl_listener mode;
	struct wl_listener destroy;
};
static void
LABWC_src_decorations_kde_deco_c_handle_destroy(struct wl_listener *listener, void *data)
{
	struct LABWC_src_decorations_kde_deco_c_kde_deco *kde_deco = wl_container_of(listener, kde_deco, destroy);
	wl_list_remove(&kde_deco->destroy.link);
	wl_list_remove(&kde_deco->mode.link);
	wl_list_remove(&kde_deco->link);
	free(kde_deco);
}
static void
LABWC_src_decorations_kde_deco_c_handle_mode(struct wl_listener *listener, void *data)
{
	struct LABWC_src_decorations_kde_deco_c_kde_deco *kde_deco = wl_container_of(listener, kde_deco, mode);
	if (!kde_deco->view) {
		return;
	}
	enum wlr_server_decoration_manager_mode client_mode =
		kde_deco->wlr_kde_decoration->mode;
	switch (client_mode) {
	case WLR_SERVER_DECORATION_MANAGER_MODE_SERVER:
		kde_deco->view->ssd_preference = LAB_SSD_PREF_SERVER;
		break;
	case WLR_SERVER_DECORATION_MANAGER_MODE_NONE:
	case WLR_SERVER_DECORATION_MANAGER_MODE_CLIENT:
		kde_deco->view->ssd_preference = LAB_SSD_PREF_CLIENT;
		break;
	default:
	}
	if (kde_deco->view->ssd_preference == LAB_SSD_PREF_SERVER) {
		view_set_ssd_mode(kde_deco->view, LAB_SSD_MODE_FULL);
	} else {
		view_set_ssd_mode(kde_deco->view, LAB_SSD_MODE_NONE);
	}
}
static void
LABWC_src_decorations_kde_deco_c_handle_new_server_decoration(struct wl_listener *listener, void *data)
{
	struct wlr_server_decoration *wlr_deco = data;
	struct LABWC_src_decorations_kde_deco_c_kde_deco *kde_deco = znew(*kde_deco);
	kde_deco->wlr_kde_decoration = wlr_deco;
	if (wlr_deco->surface) {
		struct wlr_xdg_surface *xdg_surface =
			wlr_xdg_surface_try_from_wlr_surface(wlr_deco->surface);
		if (xdg_surface && xdg_surface->data) {
			kde_deco->view = (struct view *)xdg_surface->data;
			LABWC_src_decorations_kde_deco_c_handle_mode(&kde_deco->mode, wlr_deco);
		}
	}
	wl_signal_add(&wlr_deco->events.destroy, &kde_deco->destroy);
	kde_deco->destroy.notify = LABWC_src_decorations_kde_deco_c_handle_destroy;
	wl_signal_add(&wlr_deco->events.mode, &kde_deco->mode);
	kde_deco->mode.notify = LABWC_src_decorations_kde_deco_c_handle_mode;
	wl_list_append(&LABWC_src_decorations_kde_deco_c_decorations, &kde_deco->link);
}
void
kde_server_decoration_set_view(struct view *view, struct wlr_surface *surface)
{
	struct LABWC_src_decorations_kde_deco_c_kde_deco *kde_deco;
	wl_list_for_each(kde_deco, &LABWC_src_decorations_kde_deco_c_decorations, link) {
		if (kde_deco->wlr_kde_decoration->surface == surface) {
			if (!kde_deco->view) {
				kde_deco->view = view;
				LABWC_src_decorations_kde_deco_c_handle_mode(&kde_deco->mode, kde_deco->wlr_kde_decoration);
			}
			return;
		}
	}
}
void
kde_server_decoration_update_default(void)
{
	assert(LABWC_src_decorations_kde_deco_c_kde_deco_mgr);
	wlr_server_decoration_manager_set_default_mode(LABWC_src_decorations_kde_deco_c_kde_deco_mgr,
		rc.xdg_shell_server_side_deco
		? WLR_SERVER_DECORATION_MANAGER_MODE_SERVER
		: WLR_SERVER_DECORATION_MANAGER_MODE_CLIENT);
}
void
kde_server_decoration_init(void)
{
	assert(!LABWC_src_decorations_kde_deco_c_kde_deco_mgr);
	LABWC_src_decorations_kde_deco_c_kde_deco_mgr = wlr_server_decoration_manager_create(server.wl_display);
	if (!LABWC_src_decorations_kde_deco_c_kde_deco_mgr) {
		exit(EXIT_FAILURE);
	}
	wl_list_init(&LABWC_src_decorations_kde_deco_c_decorations);
	kde_server_decoration_update_default();
	wl_signal_add(&LABWC_src_decorations_kde_deco_c_kde_deco_mgr->events.new_decoration, &server.kde_server_decoration);
	server.kde_server_decoration.notify = LABWC_src_decorations_kde_deco_c_handle_new_server_decoration;
}
void
kde_server_decoration_finish(void)
{
	wl_list_remove(&server.kde_server_decoration.link);
}
struct LABWC_src_decorations_xdg_deco_c_xdg_deco {
	struct wlr_xdg_toplevel_decoration_v1 *wlr_xdg_decoration;
	enum wlr_xdg_toplevel_decoration_v1_mode client_mode;
	struct view *view;
	struct wl_listener destroy;
	struct wl_listener request_mode;
	struct wl_listener surface_commit;
};
static void
LABWC_src_decorations_xdg_deco_c_xdg_deco_destroy(struct wl_listener *listener, void *data)
{
	struct LABWC_src_decorations_xdg_deco_c_xdg_deco *xdg_deco = wl_container_of(listener, xdg_deco, destroy);
	wl_list_remove(&xdg_deco->destroy.link);
	wl_list_remove(&xdg_deco->request_mode.link);
	if (xdg_deco->surface_commit.notify) {
		wl_list_remove(&xdg_deco->surface_commit.link);
		xdg_deco->surface_commit.notify = NULL;
	}
	free(xdg_deco);
}
static void
LABWC_src_decorations_xdg_deco_c_handle_surface_commit(struct wl_listener *listener, void *data)
{
	struct LABWC_src_decorations_xdg_deco_c_xdg_deco *xdg_deco = wl_container_of(listener, xdg_deco, surface_commit);
	struct wlr_xdg_toplevel_decoration_v1 *deco = xdg_deco->wlr_xdg_decoration;
	if (deco->toplevel->base->initial_commit) {
		wlr_xdg_toplevel_decoration_v1_set_mode(deco, xdg_deco->client_mode);
		wl_list_remove(&xdg_deco->surface_commit.link);
		xdg_deco->surface_commit.notify = NULL;
	}
}
static void
LABWC_src_decorations_xdg_deco_c_xdg_deco_request_mode(struct wl_listener *listener, void *data)
{
	struct LABWC_src_decorations_xdg_deco_c_xdg_deco *xdg_deco = wl_container_of(listener, xdg_deco, request_mode);
	enum wlr_xdg_toplevel_decoration_v1_mode client_mode =
		xdg_deco->wlr_xdg_decoration->requested_mode;
	switch (client_mode) {
	case WLR_XDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE:
		xdg_deco->view->ssd_preference = LAB_SSD_PREF_SERVER;
		break;
	case WLR_XDG_TOPLEVEL_DECORATION_V1_MODE_CLIENT_SIDE:
		xdg_deco->view->ssd_preference = LAB_SSD_PREF_CLIENT;
		break;
	case WLR_XDG_TOPLEVEL_DECORATION_V1_MODE_NONE:
		xdg_deco->view->ssd_preference = LAB_SSD_PREF_UNSPEC;
		client_mode = rc.xdg_shell_server_side_deco
			? WLR_XDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE
			: WLR_XDG_TOPLEVEL_DECORATION_V1_MODE_CLIENT_SIDE;
		break;
	default:
	}
	xdg_deco->client_mode = client_mode;
	if (xdg_deco->wlr_xdg_decoration->toplevel->base->initialized) {
		wlr_xdg_toplevel_decoration_v1_set_mode(xdg_deco->wlr_xdg_decoration,
			client_mode);
	} else if (!xdg_deco->surface_commit.notify) {
		xdg_deco->surface_commit.notify = LABWC_src_decorations_xdg_deco_c_handle_surface_commit;
		wl_signal_add(
			&xdg_deco->wlr_xdg_decoration->toplevel->base->surface->events.commit,
			&xdg_deco->surface_commit);
	}
	if (client_mode == WLR_XDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE) {
		view_set_ssd_mode(xdg_deco->view, LAB_SSD_MODE_FULL);
	} else {
		view_set_ssd_mode(xdg_deco->view, LAB_SSD_MODE_NONE);
	}
}
static void
LABWC_src_decorations_xdg_deco_c_xdg_toplevel_decoration(struct wl_listener *listener, void *data)
{
	struct wlr_xdg_toplevel_decoration_v1 *wlr_xdg_decoration = data;
	struct wlr_xdg_surface *xdg_surface = wlr_xdg_decoration->toplevel->base;
	if (!xdg_surface || !xdg_surface->data) {
		return;
	}
	struct LABWC_src_decorations_xdg_deco_c_xdg_deco *xdg_deco = znew(*xdg_deco);
	xdg_deco->wlr_xdg_decoration = wlr_xdg_decoration;
	xdg_deco->view = (struct view *)xdg_surface->data;
	wl_signal_add(&wlr_xdg_decoration->events.destroy, &xdg_deco->destroy);
	xdg_deco->destroy.notify = LABWC_src_decorations_xdg_deco_c_xdg_deco_destroy;
	wl_signal_add(&wlr_xdg_decoration->events.request_mode,
		&xdg_deco->request_mode);
	xdg_deco->request_mode.notify = LABWC_src_decorations_xdg_deco_c_xdg_deco_request_mode;
	LABWC_src_decorations_xdg_deco_c_xdg_deco_request_mode(&xdg_deco->request_mode, wlr_xdg_decoration);
}
void
xdg_server_decoration_init(void)
{
	struct wlr_xdg_decoration_manager_v1 *xdg_deco_mgr = NULL;
	xdg_deco_mgr = wlr_xdg_decoration_manager_v1_create(server.wl_display, 1);
	if (!xdg_deco_mgr) {
		exit(EXIT_FAILURE);
	}
	wl_signal_add(&xdg_deco_mgr->events.new_toplevel_decoration,
		&server.xdg_toplevel_decoration);
	server.xdg_toplevel_decoration.notify = LABWC_src_decorations_xdg_deco_c_xdg_toplevel_decoration;
}
void
xdg_server_decoration_finish(void)
{
	wl_list_remove(&server.xdg_toplevel_decoration.link);
}
