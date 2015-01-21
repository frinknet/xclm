/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	xcb_window_t win = 0;
	int color;

	if (argc < 3) {
		xcbtools_usage_window(argv[0], "color");
	}

	color = strtoul(*++argv, NULL, 16);

	xcbtools_conn_init(&conn);

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		xcbtools_window_background(conn, win, color);
	}

	xcbtools_conn_kill(&conn);

	return 0;
}
