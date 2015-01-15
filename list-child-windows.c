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
	xcb_window_t root;
	xcb_window_t *child;

	if (argc < 2) {
		xcbh_win_usage(argv[0], "");
	}

	xcbh_conn_init(&conn);

	while (*++argv) {
		root = strtoul(*argv, NULL, 16);

		xcbh_win_children(conn, root, &child);

		while (*child++) {
			printf("00x%08x\n", child);
		}
	}

	xcbh_conn_kill(&conn);

	return 0;
}
