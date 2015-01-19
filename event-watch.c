#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <getopt.h>
#include <xcb/xcb.h>

#include "xcbh.h"

static xcb_connection_t *conn;
static xcb_screen_t *screen;

int
main (int argc, char **argv)
{
	xcb_window_t *win;
	char *event_dir;

	xcbh_conn_init(&conn);
	xcbh_screen_init(conn, &screen);

	if (argc < 2) {
		xcbh_usage(argv[0], "event-dir");
	}

	event_dir = argv[1];

	xcbh_event_register(conn, screen->root, XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY);

	xcbh_win_children(conn, screen->root, &win);

	while (*win++) {
		xcbh_event_register(conn, *win, 0);
	}

	free(win);

	xcb_flush(conn);

	xcbh_event_loop(conn, event_dir);

	xcbh_conn_kill(&conn);

	return 0;
}
