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
	int x, y;
	xcb_window_t win;

	if (argc < 4) {
		xcbtools_win_usage(argv[0], "x y");
	}

	xcbtools_conn_init(&conn);

	x = atoi(*(++argv));
	y = atoi(*(++argv));

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		xcbtools_win_move(conn, win, x, y);
	}

	xcbtools_conn_kill(&conn);

	return 0;
}
