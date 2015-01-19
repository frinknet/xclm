/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;

void print_children(xcb_window_t);

void
print_children(xcb_window_t root)
{
	xcb_window_t *win;

	xcbtools_win_children(conn, root, &win);

	while (*win++) {
		printf("0x%08x ", *win);
	}

	printf("%s", "\n");
}

int
main(int argc, char **argv)
{
	xcb_window_t win;

	if (argc < 2) {
		xcbtools_win_usage(argv[0], "");
	}

	xcbtools_conn_init(&conn);

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		print_children(win);
	}

	xcbtools_conn_kill(&conn);

	return 0;
}
