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
	xcb_window_t win;
	xcb_get_geometry_reply_t *geom;

	if (argc < 1) {
		xcbh_win_usage(argv[0]);
	}

	xcbh_conn_init(&conn);

	while (*argv) {
		win = strtoul(*argv++, NULL, 16);
		geom = xcbh_win_geometry(conn, win);

		printf("%d %d\n", geom->width, geom->height);
	}
	xcbh_conn_kill(&conn);

	return 0;
}
