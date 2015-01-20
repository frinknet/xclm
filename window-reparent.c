/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	int x, y;
	xcb_window_t win;
	xcb_window_t parent;

	if (argc < 5) {
		xcbtools_usage_window(argv[0], "parent x y");
	}

	xcbtools_conn_init(&conn);

	parent = strtoul(*++argv, NULL, 16);
	x = atoi(*++argv);
	y = atoi(*++argv);

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		xcb_reparent_window(conn, win, parent, x, y);
	}

	xcbtools_conn_kill(&conn);

	return 0;
}
