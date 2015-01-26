/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	int x, y;
	xcb_window_t win;
	xcb_window_t parent;
	xcb_void_cookie_t cookie;
	xcb_generic_error_t *error;

	if (argc < 5) {
		xcbtools_usage_window(argv[0], "parent x y");
	}

	xcbtools_conn_init(&conn);

	parent = strtoul(*++argv, NULL, 16);
	x = atoi(*++argv);
	y = atoi(*++argv);

	if (!xcbtools_window_exists(conn, parent)) {
		printf("0x%08x parent window does not exist\n", parent);

		return 1;
	}

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);
		cookie = xcb_reparent_window_checked(conn, win, parent, x, y);
		error = xcb_request_check(conn, cookie);

		if (error != NULL) {
			printf("0x%08x failed to reparent %d\n", win, error->error_code);

			return 1;
		}
	}

	xcbtools_conn_kill(&conn);

	return 0;
}
