#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <xcb/xcb.h>
#include <xcb/xcb_icccm.h>

#include "xcbh.h"

void
xcbh_conn_init(xcb_connection_t **conn)
{
	*conn = xcb_connect(NULL, NULL);

	if (xcb_connection_has_error(*conn)) {
		errx(1, "unable connect to the X server");
	}
}

void
xcbh_conn_kill(xcb_connection_t **conn)
{
	if (*conn) {
		xcb_flush(*conn);
		xcb_disconnect(*conn);
	}
}

void
xcbh_screen_init(xcb_connection_t *conn, xcb_screen_t **screen)
{
	*screen = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;

	if (*screen == NULL) {
		errx(1, "unable to retrieve screen informations");
	}
}


xcb_get_window_attributes_reply_t *
xcbh_win_attributes(xcb_connection_t *conn, xcb_window_t win)
{
	xcb_get_window_attributes_cookie_t cookie;
	xcb_get_window_attributes_reply_t  *reply;

	cookie = xcb_get_window_attributes(conn, win);
	reply = xcb_get_window_attributes_reply(conn, cookie, NULL);

	return reply;
}

char *
xcbh_win_property(xcb_connection_t *conn, xcb_window_t win, xcb_atom_t prop)
{
	xcb_get_property_cookie_t cookie;
	xcb_get_property_reply_t *reply;
	char *value;

	cookie = xcb_get_property(conn, 0, win,
			prop, XCB_ATOM_STRING, 0L, 32L);
	reply = xcb_get_property_reply(conn, cookie, NULL);
	value = xcb_get_property_value(reply);

	free(reply);

	return value;
}


int
xcbh_win_exists(xcb_connection_t *conn, xcb_window_t win)
{
	xcb_get_window_attributes_reply_t  *attr;

	attr = xcbh_win_attributes(conn, win);

	if (attr == NULL) {
		return 0;
	}

	free(attr);

	return 1;
}

int
xcbh_win_mapped(xcb_connection_t *conn, xcb_window_t win)
{
	int mapped;
	xcb_get_window_attributes_reply_t  *attr;

	attr = xcbh_win_attributes(conn, win);

	if (attr == NULL) {
		return 0;
	}

	mapped = attr->map_state;

	free(attr);

	return mapped == XCB_MAP_STATE_VIEWABLE;
}

int
xcbh_win_ignored(xcb_connection_t *conn, xcb_window_t win)
{
	int override;
	xcb_get_window_attributes_reply_t  *attr;

	attr = xcbh_win_attributes(conn, win);

	if (attr == NULL) {
		return 0;
	}

	override = attr->override_redirect;

	free(attr);

	return override;
}

void
xcbh_win_ignore(xcb_connection_t *conn, xcb_window_t win, int override)
{
	uint32_t mask = XCB_CW_OVERRIDE_REDIRECT;
	uint32_t value[] = { override };

	xcb_change_window_attributes(conn, win, mask, value);
}

void
xcbh_win_stack(xcb_connection_t *conn, xcb_window_t win, uint32_t stack)
{
	uint32_t mask = XCB_CONFIG_WINDOW_STACK_MODE;
	uint32_t value[] = { stack };

	xcb_configure_window(conn, win, mask, value);
}

void
xcbh_win_resize(xcb_connection_t *conn, xcb_window_t win, int width, int height)
{
	uint32_t values[2];
	uint32_t mask = XCB_CONFIG_WINDOW_WIDTH
					| XCB_CONFIG_WINDOW_HEIGHT;

	values[0] = width;
	values[1] = height;

	xcb_configure_window(conn, win, mask, values);
}

void
xcbh_win_move(xcb_connection_t *conn, xcb_window_t win, int x, int y)
{
	uint32_t values[2];
	uint32_t mask = XCB_CONFIG_WINDOW_X
					| XCB_CONFIG_WINDOW_Y;

	values[0] = x;
	values[1] = y;

	xcb_configure_window(conn, win, mask, values);
}

void
xcbh_win_warp(xcb_connection_t *conn, xcb_window_t win, int x, int y, int width, int height)
{
	uint32_t values[4];
	uint32_t mask = XCB_CONFIG_WINDOW_X
					| XCB_CONFIG_WINDOW_Y
					| XCB_CONFIG_WINDOW_WIDTH
					| XCB_CONFIG_WINDOW_HEIGHT;

	values[0] = x;
	values[1] = y;
	values[2] = width;
	values[3] = height;

	xcb_configure_window(conn, win, mask, values);
}

xcb_get_geometry_reply_t *
xcbh_win_geometry(xcb_connection_t *conn, xcb_window_t win)
{
	xcb_get_geometry_cookie_t cookie;
	xcb_get_geometry_reply_t *reply;

	cookie = xcb_get_geometry(conn, win);
	reply = xcb_get_geometry_reply(conn, cookie, NULL);

	if (!reply) {
		errx(1, "center_pointer: missing geometry!");
	}

	return reply;
}

int
xcbh_win_children(xcb_connection_t *conn, xcb_window_t win, xcb_window_t **list)
{
	uint32_t childnum = 0;
	xcb_query_tree_cookie_t cookie;
	xcb_query_tree_reply_t *reply;

	cookie = xcb_query_tree(conn, win);
	reply = xcb_query_tree_reply(conn, cookie, NULL);

	if (reply == NULL) {
		errx(1, "0x%08x: no such window", win);
	}

	*list = malloc(sizeof(xcb_window_t) * reply->children_len);

	memcpy(*list, xcb_query_tree_children(reply),
			sizeof(xcb_window_t) * reply->children_len);

	childnum = reply->children_len;

	free(reply);

	return childnum;
}

xcb_window_t
xcbh_win_parent(xcb_connection_t *conn, xcb_window_t win)
{
	xcb_window_t parent;
	xcb_query_tree_cookie_t cookie;
	xcb_query_tree_reply_t *reply;

	cookie = xcb_query_tree(conn, win);
	reply = xcb_query_tree_reply(conn, cookie, NULL);

	if (reply == NULL) {
		errx(1, "0x%08x: no such window", win);
	}

	parent = reply->parent;

	free(reply);

	return parent;
}

char *
xcbh_win_name(xcb_connection_t *conn, xcb_window_t win)
{
	return xcbh_win_property(conn, win, XCB_ATOM_WM_NAME);
}

char *
xcbh_win_class(xcb_connection_t *conn, xcb_window_t win)
{
	return xcbh_win_property(conn, win, XCB_ATOM_WM_CLASS);
}

char *
xcbh_win_command(xcb_connection_t *conn, xcb_window_t win)
{
	return xcbh_win_property(conn, win, XCB_ATOM_WM_COMMAND);
}

xcb_window_t
xcbh_win_current(xcb_connection_t *conn)
{
	xcb_window_t win = 0;

	xcb_get_input_focus_cookie_t cookie;
	xcb_get_input_focus_reply_t *reply;

	cookie = xcb_get_input_focus(conn);
	reply = xcb_get_input_focus_reply(conn, cookie, NULL);

	if (reply == NULL) {
		errx(1, "xcb_get_input_focus");
	}

	win = reply->focus;

	free(reply);

	return win;
}

void
xcbh_win_register_events(xcb_window_t win, uint32_t mask)
{
	uint32_t values[] = { mask };

	xcb_change_window_attributes(conn, w, XCB_CW_EVENT_MASK, values);
	xcb_flush(conn);
}

bool
xcbh_event_notify_valid(xcb_generic_event_t *event)
{
	xcb_enter_notify_event_t *notify;
	notify = (xcb_enter_notify_event_t*)event;

	switch (notify->mode) {
	case XCB_NOTIFY_MODE_NORMAL:
	case XCB_NOTIFY_MODE_UNGRAB:
		if (notify->detail == 0) {
			return true;
		}
	}

	return false;
}

void
xcbh_event_loop(void)
{
	bool running = true;
	xcb_generic_event_t *event;

	while (running) {
		event = xcb_wait_for_event(conn);

		switch (event->response_type & ~0x80) {
		case XCB_ENTER_NOTIFY:
			if (xcbh_event_notify_valid(event)) {
				event_trigger("window-mouse-enter", ((xcb_enter_notify_event_t*)event)->event);
			}

			break;
		case XCB_LEAVE_NOTIFY:
			if (xcbh_event_notify_valid(event)) {
				xcbh_event_trigger("window-mouse-leave", ((xcb_enter_notify_event_t*)event)->event);
			}

			break;
		case XCB_GRAPHICS_EXPOSURE:
			xcbh_event_trigger("window-graphics-expose", 0);

			break;
		case XCB_NO_EXPOSURE:
			xcbh_event_trigger("window-no-expose", 0);

			break;
		case XCB_VISIBILITY_NOTIFY:
			xcbh_event_trigger("window-visible", 0);

			break;
		case XCB_REPARENT_NOTIFY:
			xcbh_event_trigger("window-reparent", 0);

			break;
		case XCB_CONFIGURE_REQUEST:
			xcbh_event_trigger("configure-request", 0);

			break;
		case XCB_CONFIGURE_NOTIFY:
			xcbh_event_trigger("configure-notify", 0);

			break;
		case XCB_RESIZE_REQUEST:
			xcbh_event_trigger("window-resize", 0);

			break;
		case XCB_GRAVITY_NOTIFY:
			xcbh_event_trigger("gravity-notify", 0);

			break;
		case XCB_CIRCULATE_REQUEST:
			xcbh_event_trigger("circulate-request", 0);

			break;
		case XCB_CIRCULATE_NOTIFY:
			xcbh_event_trigger("circulate-notify", 0);

			break;
		case XCB_PROPERTY_NOTIFY:
			xcbh_event_trigger("property-notify", 0);

			break;
		case XCB_SELECTION_CLEAR:
			xcbh_event_trigger("selection-clear", 0);

			break;
		case XCB_SELECTION_REQUEST:
			xcbh_event_trigger("selection-request", 0);

			break;
		case XCB_SELECTION_NOTIFY:
			xcbh_event_trigger("selection-notify", 0);

			break;
		case XCB_COLORMAP_NOTIFY:
			xcbh_event_trigger("colormap-notify", 0);

			break;
		case XCB_FOCUS_IN:
			xcbh_event_trigger("window-focus", ((xcb_focus_in_event_t*)event)->event);

			break;
		case XCB_FOCUS_OUT:
			xcbh_event_trigger("window-blur", ((xcb_focus_in_event_t*)event)->event);

			break;
		case XCB_CLIENT_MESSAGE:
			xcbh_event_trigger("client-message", 0);

			break;
		case XCB_MAP_REQUEST:
			xcbh_event_trigger("window-map-request", ((xcb_map_notify_event_t*)event)->window);

			break;
		case XCB_MAP_NOTIFY:
			xcbh_event_trigger("window-mapped", ((xcb_map_notify_event_t*)event)->window);

			break;
		case XCB_UNMAP_NOTIFY:
			xcbh_event_trigger("window-unmapped", ((xcb_map_notify_event_t*)event)->window);

			break;
		case XCB_MAPPING_NOTIFY:
			xcbh_event_trigger("window-mapping", (((xcb_map_notify_event_t*)event)->window);

			break;
		case XCB_DESTROY_NOTIFY:
			xcbh_event_trigger("window-destroy", ((xcb_create_notify_event_t*)event)->window);

			break;
		case XCB_CREATE_NOTIFY:
			xcbh_event_trigger("window-create", ((xcb_create_notify_event_t*)event)->window);

			xcbh_win_register_events(win, mask);

			break;
		default:
			sprintf(event_name, "xcb-event-%d", event->response_type);

			xcbh_event_trigger(event_name, 0);
		}
	}
}

void
xcbh_event_trigger(char *event_name, xcb_window_t win)
{
	if (win && !xcbh_win_ignored(conn, win)) {
		printf("event-trigger %s 0x%08x\n", event_name, win);
	} else {
		printf("event-trigger %s\n", event_name);
	}

	fflush(stdout);
}

void
xcbh_win_usage(char *name, char *params)
{
	char *win_params = (char *) malloc(16 + strlen(params));

	strcpy(win_params, params);

	if (strlen(params)) {
		strcat(win_params, " ");
	}

	strcat(win_params, "wid [..wid]");

	xcbh_usage(name, win_params);
}

void
xcbh_usage(char *name, char *params)
{
	xcbh_show_header();
	fprintf(stderr, "Usage:\n\t%s %s\n\n", basename(name), params);
	exit(1);
}

void
xcbh_show_header()
{
	fprintf(stderr, "FRINKnet XCB Tools v%s\n", XCBH_VERSION);
	fprintf(stderr, "Copyright %s\n", XCBH_COPYRIGHT);
	fprintf(stderr, "%s\n\n", XCBH_LICENSE);
}

/*
void
xcbh_pointer_center(xcb_connection_t conn, xcb_window_t win)
{
	uint32_t values[1];
	xcb_get_geometry_reply_t *geom;

	geom = xcbh_win_geometry(conn, win);

	xcb_warp_pointer(conn, XCB_NONE, win, 0, 0, 0, 0,
			(geom->width  + (geom->border_width * 2)) / 2,
			(geom->height + (geom->border_width * 2)) / 2);

	values[0] = XCB_STACK_MODE_ABOVE;

	xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_STACK_MODE, values);

	free(geom);
}
*/
