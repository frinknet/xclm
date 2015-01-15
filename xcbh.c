#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
xcbh_win_redraw(xcb_connection_t *conn, xcb_window_t win, int x, int y, int width, int height)
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
xcbh_win_usage(char *name, char *params)
{
	fprintf(stderr, "usage: %s %s<wid> [wid..]\n", name, params);
	exit(1);
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
