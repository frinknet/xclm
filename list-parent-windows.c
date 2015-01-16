/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <err.h>

#include "xcbh.h"

static xcb_connection_t *conn;
static xcb_screen_t *screen;

void print_parents(xcb_window_t);

void
print_parents(xcb_window_t win)
{
	while (win != screen->root) {
		win = xcbh_win_parent(conn, win);

		printf("0x%08x\n", win);
	}
}

int
main(int argc, char **argv)
{
	xcb_window_t win = 0;

	if (argc < 2) {
		xcbh_win_usage(argv[0], "");
	}

	xcbh_conn_init(&conn);
	xcbh_screen_init(conn, &screen);

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		print_parents(win);
	}

	xcbh_conn_kill(&conn);

	return 0;
}
