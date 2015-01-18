#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <getopt.h>
#include <xcb/xcb.h>

#include "xcbh.h"

static xcb_connection_t *conn;
static xcb_screen_t *screen;
static char *event_dir;

int
main (int argc, char **argv)
{
	char *event_dir;
	xcb_window_t *win;

	xcbh_conn_init(&conn);
	xcbh_screen_init(conn, &screen);

	if (argc < 2) {
		xcbh_usage(argv[0], "event-dir");
	}

	xcbh_win_register_events(screen->root, XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY);

	xcbh_get_windows(conn, screen->root, &win);

	while (*win++) {
		xcbh_win_register_events(*win, mask);
	}

	free(win);

	xcb_flush(conn);

	events_loop();

	xcbh_conn_kill(&conn);

	return 0;
}
