/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (6, "x y width height") {
	int x, y, width, height;
	xcb_window_t win;

	x = atoi(*++argv);
	y = atoi(*++argv);
	width = atoi(*++argv);
	height = atoi(*++argv);

	xcmd_loop {
		win = strtoul(xcmd_prop, NULL, 16);

		xmpl_window_warp(xcmd_conn, win, x, y, width, height);
	}

	xcmd_exit(0);
}
