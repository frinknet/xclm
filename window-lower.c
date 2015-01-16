/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "xcbh.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	xcb_window_t win;

	if (argc < 4) {
		xcbh_win_usage(argv[0], "");
	}

	xcbh_conn_init(&conn);

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		xcbh_win_move(conn, win, XCB_STACK_MODE_BELOW);
	}

	xcbh_conn_kill(&conn);

	return 0;
}
