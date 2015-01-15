/* See LICENSE file for copyright and license details. */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>

#include "xcbh.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	xcb_window_t win = 0;

	if (argc < 2) {
		xcbh_win_usage(argv[0], "");
	}

	xcbh_conn_init(&conn);

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		if (xcbh_win_mapped(conn, win)) {
			xcb_unmap_window(conn, win);
		} else {
			xcb_map_window(conn, win);
		}
	}

	xcbh_conn_kill(&conn);

	return 0;
}
