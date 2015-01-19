/* See LICENSE file for copyright and license details. */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>

#include "xcbh.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	xcb_window_t win = 0;
	char *event_dir = getenv("EVENTS");
	char *event_name;

	if (argc < 2) {
		xcbh_usage(argv[0], "event-name [..wid]");
	}

	event_name = *++argv;

	if (argc < 3) {
		xcbh_event_trigger(conn, 0, event_name, event_dir);

		return 0;
	}

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		xcbh_event_trigger(conn, win, event_name, event_dir);
	}

	xcbh_conn_kill(&conn);

	return 0;
}
