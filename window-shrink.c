/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (4, "width height") {
	int width = atoi(xcmd_next);
	int height = atoi(xcmd_next);
	xcb_get_geometry_reply_t *geom;

	xcmd_win_loop {
		geom = xmpl_window_get_geometry(xcmd_conn, xcmd_win);

		xmpl_window_set_size(
			xcmd_conn,
			xcmd_win,
			(geom->width + width) * -1,
			(geom->height + height) * -1
		);
	}
	
	xcmd_exit(0);
}
