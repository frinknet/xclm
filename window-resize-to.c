/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	int width, height;
	xcb_window_t win;

	if (argc < 4) {
		xcbtools_usage_window(argv[0], "width height");
	}

	xcbtools_conn_init(&conn);

	width = atoi(*(++argv));
	height = atoi(*(++argv));

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		xcbtools_window_resize(conn, win, width, height);
	}

	xcbtools_conn_kill(&conn);

	return 0;
}
