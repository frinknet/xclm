/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;
static xcb_screen_t *screen;

int
main(int argc, char **argv)
{
	int x, y, width, height, border;
	xcb_window_t win;

	if (argc < 5) {
		xcbtools_usage(argv[0], "x y width height border");
	}

	xcbtools_conn_init(&conn);
	xcbtools_screen_init(conn, &screen);

	win = xcb_generate_id(conn);
	x = atoi(*++argv);
	y = atoi(*++argv);
	width = atoi(*++argv);
	height = atoi(*++argv);
	border = atoi(*++argv);

	xcb_create_window(
		conn, XCB_COPY_FROM_PARENT,
		win, screen->root,
		x, y, width, height, border,
		XCB_WINDOW_CLASS_INPUT_OUTPUT,
		screen->root_visual, 0, 0);

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
