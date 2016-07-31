#ifndef XMPL_H__
#define XMPL_H__

#define _XOPEN_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <libgen.h>
#include <limits.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <xcb/xcb.h>
//#include <xcb/xcb_icccm.h>

const static u_int32_t XMPL_EVENT_MASK_NULL =
	XCB_EVENT_MASK_NO_EVENT;

const static u_int32_t XMPL_EVENT_MASK_STRUCTURE =
	XCB_EVENT_MASK_STRUCTURE_NOTIFY
	| XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY;

const static u_int32_t XMPL_EVENT_MASK_REDIRECT =
	XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
	| XCB_EVENT_MASK_RESIZE_REDIRECT;

const static u_int32_t XMPL_EVENT_MASK_PROPERTIES =
	XCB_EVENT_MASK_PROPERTY_CHANGE
	| XCB_EVENT_MASK_COLOR_MAP_CHANGE;

const static u_int32_t XMPL_EVENT_MASK_FOCUS =
	XCB_EVENT_MASK_ENTER_WINDOW
	| XCB_EVENT_MASK_LEAVE_WINDOW
	| XCB_EVENT_MASK_FOCUS_CHANGE;

const static u_int32_t XMPL_EVENT_MASK_EXPOSURE =
	XCB_EVENT_MASK_EXPOSURE
	| XCB_EVENT_MASK_VISIBILITY_CHANGE;

const static u_int32_t XMPL_EVENT_MASK_MOTION =
	XCB_EVENT_MASK_POINTER_MOTION
	| XCB_EVENT_MASK_POINTER_MOTION_HINT
	| XCB_EVENT_MASK_BUTTON_MOTION
	| XCB_EVENT_MASK_BUTTON_1_MOTION
	| XCB_EVENT_MASK_BUTTON_2_MOTION
	| XCB_EVENT_MASK_BUTTON_3_MOTION
	| XCB_EVENT_MASK_BUTTON_4_MOTION
	| XCB_EVENT_MASK_BUTTON_5_MOTION;

const static u_int32_t XMPL_EVENT_MASK_BUTTONS =
	XCB_EVENT_MASK_BUTTON_PRESS
	| XCB_EVENT_MASK_BUTTON_RELEASE
	| XCB_EVENT_MASK_OWNER_GRAB_BUTTON;

const static u_int32_t XMPL_EVENT_MASK_POINTER =
	XCB_EVENT_MASK_POINTER_MOTION
	| XCB_EVENT_MASK_POINTER_MOTION_HINT
	| XCB_EVENT_MASK_BUTTON_MOTION
	| XCB_EVENT_MASK_BUTTON_1_MOTION
	| XCB_EVENT_MASK_BUTTON_2_MOTION
	| XCB_EVENT_MASK_BUTTON_3_MOTION
	| XCB_EVENT_MASK_BUTTON_4_MOTION
	| XCB_EVENT_MASK_BUTTON_5_MOTION
	| XCB_EVENT_MASK_BUTTON_PRESS
	| XCB_EVENT_MASK_BUTTON_RELEASE
	| XCB_EVENT_MASK_OWNER_GRAB_BUTTON;

const static u_int32_t XMPL_EVENT_MASK_KEYS =
	XCB_EVENT_MASK_KEY_PRESS
	| XCB_EVENT_MASK_KEY_RELEASE
	| XCB_EVENT_MASK_KEYMAP_STATE;

void xmpl_free(void *var);

void xmpl_logging(char *out, char *err);

void xmpl_conn_init(xcb_connection_t **conn);
void xmpl_conn_kill(xcb_connection_t **conn);
void xmpl_conn_sync(xcb_connection_t *conn);

void xmpl_screen_init(xcb_connection_t *conn, xcb_screen_t **screen);
void xmpl_color_init(xcb_connection_t *conn, xcb_screen_t *screen, xcb_colormap_t *colormap, xcb_visualid_t *visual);

void xmpl_window_border(xcb_connection_t *conn, xcb_window_t win, uint32_t size, uint32_t color);
void xmpl_window_background(xcb_connection_t *conn, xcb_window_t win, uint32_t color);

void xmpl_window_set_stack(xcb_connection_t *conn, xcb_window_t win, uint32_t stack);
void xmpl_window_set_ignore(xcb_connection_t *conn, xcb_window_t win, int override);
void xmpl_window_move(xcb_connection_t *conn, xcb_window_t win, int x, int y);
void xmpl_window_resize(xcb_connection_t *conn, xcb_window_t win, int w, int h);
void xmpl_window_warp(xcb_connection_t *conn, xcb_window_t win, int x, int y, int w, int h);

int xmpl_window_is_valid(xcb_connection_t *conn, xcb_window_t win);
int xmpl_window_is_mapped(xcb_connection_t *conn, xcb_window_t win);
int xmpl_window_is_ignored(xcb_connection_t *conn, xcb_window_t win);
int xmpl_window_is_bordered(xcb_connection_t *conn, xcb_window_t win);

int xmpl_window_list_children(xcb_connection_t *conn, xcb_window_t win, xcb_window_t **list);

xcb_window_t xmpl_window_create(xcb_connection_t *conn, xcb_window_t win, int x, int y, int w, int h, char *cls);
xcb_window_t xmpl_window_parent(xcb_connection_t *conn, xcb_window_t win);

xcb_get_geometry_reply_t *xmpl_window_geometry(xcb_connection_t *conn, xcb_window_t win);
xcb_get_window_attributes_reply_t *xmpl_window_get_attributes(xcb_connection_t *conn, xcb_window_t win);

void *xmpl_window_get_atom(xcb_connection_t *conn, xcb_window_t win, char *atom_name, xcb_atom_t type);
void *xmpl_window_get_property(xcb_connection_t *conn, xcb_window_t win, xcb_atom_t prop, xcb_atom_t type);
void xmpl_window_set_atom(xcb_connection_t *conn, xcb_window_t win, char *atom_name, xcb_atom_t type, void *value);
void xmpl_window_set_property(xcb_connection_t *conn, xcb_window_t win, xcb_atom_t prop, xcb_atom_t type, void *value);

char *xmpl_window_name(xcb_connection_t *conn, xcb_window_t win);
char *xmpl_window_class(xcb_connection_t *conn, xcb_window_t win);
char *xmpl_window_command(xcb_connection_t *conn, xcb_window_t win);
char *xmpl_window_type(xcb_connection_t *conn, xcb_window_t win);

void xmpl_window_rename(xcb_connection_t *conn, xcb_window_t win, char *name);

void xmpl_window_stick(xcb_connection_t *conn, xcb_window_t win);
void xmpl_window_unstick(xcb_connection_t *conn, xcb_window_t win);

void xmpl_window_pin_above(xcb_connection_t *conn, xcb_window_t win);
void xmpl_window_pin_below(xcb_connection_t *conn, xcb_window_t win);
void xmpl_window_unpin(xcb_connection_t *conn, xcb_window_t win);

void xmpl_window_shade(xcb_connection_t *conn, xcb_window_t win);
void xmpl_window_unshade(xcb_connection_t *conn, xcb_window_t win);

void xmpl_window_fullscreen(xcb_connection_t *conn, xcb_window_t win);
void xmpl_window_maximize_vertical(xcb_connection_t *conn, xcb_window_t win);
void xmpl_window_maximize_horizontal(xcb_connection_t *conn, xcb_window_t win);
void xmpl_window_maximize(xcb_connection_t *conn, xcb_window_t win);
void xmpl_window_minimize(xcb_connection_t *conn, xcb_window_t win);
void xmpl_window_restore(xcb_connection_t *conn, xcb_window_t win);

void xmpl_window_taskbar_exclude(xcb_connection_t *conn, xcb_window_t win);
void xmpl_window_taskbar_include(xcb_connection_t *conn, xcb_window_t win);
void xmpl_window_pager_exclude(xcb_connection_t *conn, xcb_window_t win);
void xmpl_window_pager_include(xcb_connection_t *conn, xcb_window_t win);

void xmpl_window_alert(xcb_connection_t *conn, xcb_window_t win);
void xmpl_window_focus(xcb_connection_t *conn, xcb_window_t win);
void xmpl_window_kill(xcb_connection_t *conn, xcb_window_t win);

xcb_window_t xmpl_window_current(xcb_connection_t *conn);

void xmpl_event_register(xcb_connection_t *conn, xcb_window_t win, uint32_t mask);
void xmpl_event_watch(xcb_connection_t *conn, xcb_window_t root, char *event_dir, uint32_t mask);
bool xmpl_event_trigger(xcb_connection_t *conn, xcb_window_t root, xcb_window_t win, char *event_name, char *event_dir, char *env);
void xmpl_event_spawn(xcb_window_t root, xcb_window_t win, char *cmd_path, int timeout, char *env);

void xmpl_pointer_center(xcb_connection_t *conn, xcb_window_t win);
void xmpl_pointer_warp(xcb_connection_t *conn, xcb_window_t win, int x, int y);

xcb_atom_t xmpl_atom(xcb_connection_t *conn, char *atom_name);
char *xmpl_atom_name(xcb_connection_t *conn, xcb_atom_t atom_name);
#endif
