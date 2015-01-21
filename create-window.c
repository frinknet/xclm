/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;
static xcb_screen_t *screen;
static xcb_colormap_t colormap;
static xcb_visualid_t visual;

int
main(int argc, char **argv)
{
	int x, y, width, height, background, bordersize, bordercolor;
	xcb_window_t win;

	if (argc < 8) {
		xcbtools_usage(argv[0], "x y width height background bordersize bordercolor");
	}

	xcbtools_conn_init(&conn);
	xcbtools_screen_init(conn, &screen);
	xcbtools_color_init(conn, screen, &colormap, &visual);

	win = xcb_generate_id(conn);
	x = atoi(*++argv);
	y = atoi(*++argv);
	width = atoi(*++argv);
	height = atoi(*++argv);
	background = strtoul(*++argv, NULL, 16);
	bordersize = atoi(*++argv);
	bordercolor = strtoul(*++argv, NULL, 16);

	printf("background: %08x\n", background);
	printf("bordercolor: %08x\n", bordercolor);

	xcb_create_window(
		conn,
		(visual == screen->root_visual)? XCB_COPY_FROM_PARENT : 32,
		win,
		screen->root,
		x, y, width, height, bordersize,
		XCB_WINDOW_CLASS_INPUT_OUTPUT,
		visual,
			XCB_CW_BACK_PIXEL |
			XCB_CW_BORDER_PIXEL |
			XCB_CW_OVERRIDE_REDIRECT |
			XCB_CW_EVENT_MASK |
			XCB_CW_COLORMAP,
		(unsigned int[]) {
			background,
			bordercolor,
			1,
				XCB_EVENT_MASK_PROPERTY_CHANGE |
				XCB_EVENT_MASK_EXPOSURE |
				XCB_EVENT_MASK_POINTER_MOTION,
			colormap
		});

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
