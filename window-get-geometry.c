/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	xcb_window_t win;
	xcb_get_geometry_reply_t *geom;

	if (argc < 2) {
		xcbtools_usage_window(argv[0], "");
	}

	xcbtools_conn_init(&conn);

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);
		geom = xcbtools_window_geometry(conn, win);

		printf("%d %d %d %d\n", geom->x, geom->y, geom->width, geom->height);
	}

	xcbtools_conn_kill(&conn);

	return 0;
}
