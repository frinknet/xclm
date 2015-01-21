/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	int x, y, width, height;
	xcb_window_t win;

	if (argc < 6) {
		xcbtools_usage(argv[0], "x y width height");
	}

	xcbtools_conn_init(&conn);

	x = atoi(*++argv);
	y = atoi(*++argv);
	width = atoi(*++argv);
	height = atoi(*++argv);


	win = xcbtools_window_create(conn, x, y, width, height);
	xcbtools_window_ignore(conn, win, true);

	xcb_map_window(conn, win);

	xcb_flush(conn);

	printf("0x%08x\n", win);

	if (fork()) {
		return 0;
	}

	while(true) {
		sleep(10);
	}

	xcbtools_conn_kill(&conn);
}
