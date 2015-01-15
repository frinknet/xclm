#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>

#include "util.h"

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

int
xcbh_win_exists(xcb_connection_t *conn, xcb_window_t win)
{
	xcb_get_window_attributes_cookie_t cookie;
	xcb_get_window_attributes_reply_t  *reply;

	cookie = xcb_get_window_attributes(conn, win);
	reply = xcb_get_window_attributes_reply(conn, cookie, NULL);

	if (reply == NULL) {
		return 0;
	}

	free(reply);

	return 1;
}

int
xcbh_win_mapped(xcb_connection_t *conn, xcb_window_t win)
{
	int mapped;
	xcb_get_window_attributes_cookie_t cookie;
	xcb_get_window_attributes_reply_t  *reply;

	cookie = xcb_get_window_attributes(conn, win);
	reply = xcb_get_window_attributes_reply(conn, cookie, NULL);

	if (reply == NULL) {
		return 0;
	}

	mapped = reply->map_state;

	free(reply);

	return mapped == XCB_MAP_STATE_VIEWABLE;
}

int
xcbh_win_ignored(xcb_connection_t *conn, xcb_window_t win)
{
	int override;
	xcb_get_window_attributes_cookie_t cookie;
	xcb_get_window_attributes_reply_t  *reply;

	cookie = xcb_get_window_attributes(conn, win);
	reply = xcb_get_window_attributes_reply(conn, cookie, NULL);

	if (reply == NULL) {
		return 0;
	}

	override = reply->override_redirect;

	free(reply);

	return override;
}

void
xcbh_win_ignore(xcb_connection_t *conn, xcb_window_t win, int override)
{
	uint32_t mask = XCB_CW_OVERRIDE_REDIRECT;
	uint32_t value[] = { override };

	xcb_change_window_attributes(conn, win, mask, value);
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

	childnum = r->children_len;

	free(reply);

	return childnum;
}

xcb_window_t
xcbh_win_current(void)
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
xcbh_win_usage(char *name)
{
	fprintf(stderr, "usage: %s <wid> [wid..]\n", name);
	exit(1);
}

