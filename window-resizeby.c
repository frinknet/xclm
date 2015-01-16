/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "xcbh.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	int width, height;
	xcb_window_t win;
	xcb_get_geometry_reply_t *geom;

	if (argc < 4) {
		xcbh_win_usage(argv[0], "<width> <height> ");
	}

	xcbh_conn_init(&conn);

	width = atoi(*(++argv));
	height = atoi(*(++argv));

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);
		geom = xcbh_win_geometry(conn, win);

		xcbh_win_redraw(conn, win, geom->x - (width / 2), geom->y - (height / 2), geom->width + width, geom->height + height);
	}

	xcbh_conn_kill(&conn);

	return 0;
}
