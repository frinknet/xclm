/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "xcbh.h"

static xcb_connection_t *conn;
static xcb_screen_t *screen;

static void resize(xcb_window_t, int, int);

int
main(int argc, char **argv)
{
	int x, y;
	xcb_window_t win;

	if (argc < 3) {
		xcbh_win_usage(argv[0], "<x> <y> <width> <height> ");
	}

	xcbh_conn_init(&conn);

	x = atoi(*(++argv));
	y = atoi(*(++argv));

	while (*argv) {
		win = strtoul(*argv++, NULL, 16);

		xcbh_win_redraw(conn, win, x, y, width, height);
	}

	xcbh_conn_kill(&conn);

	return 0;
}
