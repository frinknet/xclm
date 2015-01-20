/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	xcb_window_t win;
	char *class = "";

	if (argc < 3) {
		xcbtools_usage_window(argv[0], "class");
	}

	xcbtools_conn_init(&conn);

	;

	class = malloc(strlen(*argv++));

	memcpy(class, *argv, strlen(*argv));

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		if (strcmp(class, xcbtools_window_class(conn, win))) {
			xcbtools_conn_kill(&conn);

			return 1;
		}
	}

	xcbtools_conn_kill(&conn);

	return 0;
}
