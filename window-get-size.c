/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "xcbtools.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	xcb_window_t win;
	xcb_get_geometry_reply_t *geom;

	if (argc < 2) {
		xcbtools_win_usage(argv[0], "");
	}

	xcbtools_conn_init(&conn);

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);
		geom = xcbtools_win_geometry(conn, win);

		printf("%d %d\n", geom->width, geom->height);
	}
	xcbtools_conn_kill(&conn);

	return 0;
}
