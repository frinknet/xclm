/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (4, "x y") {
	int x = atoi(*++argv);
	int y = atoi(*++argv);
	xcb_get_geometry_reply_t *geom;

	xcmd_win_loop {
		geom = xmpl_window_geometry(xcmd_conn, xcmd_win);

		xmpl_window_move(xcmd_conn, xcmd_win, geom->x + x, geom->y + y);
	}

	xcmd_exit(0);
}
