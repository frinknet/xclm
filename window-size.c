/* SEE LICENSE */

#include "xcmd.h"

xcmd_simple {
	xcb_get_geometry_reply_t *geom;

	xcmd_win_loop {
		geom = xmpl_window_geometry(xcmd_conn, xcmd_win);

		printf("%d %d\n", geom->width, geom->height);
	}

	xcmd_exit(0);
}
