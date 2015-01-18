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
	int width, height;
	xcb_window_t win;

	if (argc < 4) {
		xcbh_win_usage(argv[0], "width height");
	}

	xcbh_conn_init(&conn);

	width = atoi(*(++argv));
	height = atoi(*(++argv));

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		xcbh_win_resize(conn, win, width, height);
	}

	xcbh_conn_kill(&conn);

	return 0;
}
