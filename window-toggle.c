/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	xcb_window_t win = 0;

	if (argc < 2) {
		xcbtools_usage_window(argv[0], "");
	}

	xcbtools_conn_init(&conn);

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		if (xcbtools_window_mapped(conn, win)) {
			xcb_unmap_window(conn, win);
		} else {
			xcb_map_window(conn, win);
		}
	}

	xcbtools_conn_kill(&conn);

	return 0;
}
