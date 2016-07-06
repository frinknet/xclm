/* SEE LICENSE */

#include "xcmd.h"

xcmd_simple {
	xcmd_win_loop {
		if (xcbtools_window_mapped(xcmd_conn, xcmd_win)) {
			xcb_unmap_window(xcmd_conn, xcmd_win);
		} else {
			xcb_map_window(xcmd_conn, xcmd_win);
		}
	}

	xcmd_exit(0);
}
