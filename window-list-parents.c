/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;
static xcb_screen_t *screen;

void print_parents(xcb_window_t);

void
print_parents(xcb_window_t win)
{
	while (win != screen->root) {
		win = xcbtools_window_parent(conn, win);

		printf("0x%08x ", win);
	}

	printf("%s", "\n");
}

int
main(int argc, char **argv)
{
	xcb_window_t win = 0;

	if (argc < 2) {
		xcbtools_usage_window(argv[0], "");
	}

	xcbtools_conn_init(&conn);
	xcbtools_screen_init(conn, &screen);

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		print_parents(win);
	}

	xcbtools_conn_kill(&conn);

	return 0;
}
