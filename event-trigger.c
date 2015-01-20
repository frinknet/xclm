/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	xcb_window_t win = 0;
	char *event_dir = getenv("EVENTS");
	char *event_name;
	bool spawn = false;

	if (argc < 2) {
		xcbtools_usage(argv[0], "event-name [..wid]");
	}

	event_name = *++argv;

	xcbtools_conn_init(&conn);

	if (argc < 3) {
		xcbtools_event_trigger(conn, 0, event_name, event_dir? event_dir : "~/.events", spawn);

		return 0;
	}

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		xcbtools_event_trigger(conn, win, event_name, event_dir? event_dir : "~/.events", spawn);
	}

	xcbtools_conn_kill(&conn);

	return 0;
}
