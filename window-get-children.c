/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "xcbh.h"

static xcb_connection_t *conn;

void print_children(xcb_window_t);

void
print_children(xcb_window_t root)
{
	xcb_window_t *win;

	xcbh_win_children(conn, root, &win);

	while (*win++) {
		printf("0x%08x ", *win);
	}

	printf("%s\n", "");
}

int
main(int argc, char **argv)
{
	xcb_window_t win;

	if (argc < 2) {
		xcbh_win_usage(argv[0], "");
	}

	xcbh_conn_init(&conn);

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		print_children(win);
	}

	xcbh_conn_kill(&conn);

	return 0;
}
