/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	int x, y;
	xcb_window_t win;

	if (argc < 4) {
		xcbtools_usage_window(argv[0], "x y");
	}

	xcbtools_conn_init(&conn);

	x = atoi(*++argv);
	y = atoi(*++argv);

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		xcbtools_window_move(conn, win, x, y);
	}

	xcbtools_conn_kill(&conn);

	return 0;
}
