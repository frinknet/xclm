/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (6, "x y width height") {
	int x = atoi(*++argv);
	int y = atoi(*++argv);
	int width = atoi(*++argv);
	int height = atoi(*++argv);
	xcb_get_geometry_reply_t *geom;

	xcmd_win_loop {
		geom = xmpl_window_geometry(xcmd_conn, xcmd_win);

		xmpl_window_warp(
			xcmd_conn,
			xcmd_win,
			geom->x + x,
			geom->y + y,
			geom->width + width,
			geom->height + height
		);
	}

	xcmd_exit(0);
}
