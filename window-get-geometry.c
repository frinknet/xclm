/* SEE LICENSE */

#include "xcmd.h"

xcmd_simple {
	xcb_get_geometry_reply_t *geom;

	xcmd_win_loop {
		geom = xcbtools_window_geometry(xcmd_conn, xcmd_win);

		printf("%d %d %d %d\n", geom->x, geom->y, geom->width, geom->height);
	}

	xcmd_exit(0);
}
