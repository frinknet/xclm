/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	int x, y, width, height;
	xcb_window_t win;

	if (argc < 6) {
		xcbtools_win_usage(argv[0], "x y width height ");
	}

	xcbtools_conn_init(&conn);

	x = atoi(*(++argv));
	y = atoi(*(++argv));
	width = atoi(*(++argv));
	height = atoi(*(++argv));

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		xcbtools_win_warp(conn, win, x, y, width, height);
	}

	xcbtools_conn_kill(&conn);

	return 0;
}
