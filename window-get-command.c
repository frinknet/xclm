/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	xcb_window_t win = 0;

	if (argc < 2) {
		xcbtools_usage_window(argv[0], "");
	}

	xcbtools_conn_init(&conn);

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		printf("%s\n", xcbtools_window_command(conn, win));
	}

	xcbtools_conn_kill(&conn);

	return 0;
}
