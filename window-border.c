/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	xcb_window_t win = 0;
	int size;
	int color;

	if (argc < 4) {
		xcbtools_usage_window(argv[0], "size color");
	}

	size = strtoul(*++argv, NULL, 10);
	color = strtoul(*++argv, NULL, 16);

	xcbtools_conn_init(&conn);

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		xcbtools_window_border(conn, win, size, color);
	}

	xcbtools_conn_kill(&conn);

	return 0;
}
