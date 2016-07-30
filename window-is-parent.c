/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (3, "parent") {
	xcb_window_t parent = strtoul(xcmd_next, NULL, 16);

	xcmd_win_loop {
		if (parent != xmpl_window_parent(xcmd_conn, xcmd_win)) {
			xcmd_return(1);
		}
	}

	xcmd_exit(0);
}
