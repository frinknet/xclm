/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "xcbh.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	int x, y;
	xcb_window_t win;
	xcb_get_geometry_reply_t *geom;

	if (argc < 4) {
		xcbh_win_usage(argv[0], "<x> <y> ");
	}

	xcbh_conn_init(&conn);

	x = atoi(*(++argv));
	y = atoi(*(++argv));

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);
		geom = xcbh_win_geometry(conn, win);

		xcbh_win_move(conn, win, geom->x + x, geom->y + y);
	}

	xcbh_conn_kill(&conn);

	return 0;
}
