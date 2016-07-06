/* SEE LICENSE */

#include "xcmd.h"

xcmd_call (2, "wid") {
	xcmd_win_loop {
		if (!xmpl_window_exists(xcmd_conn, xcmd_win)) {
			xcmd_return(1);
		}

		xcb_set_input_focus(xcmd_conn, XCB_INPUT_FOCUS_POINTER_ROOT, xcmd_win, XCB_CURRENT_TIME);
	}

	xcmd_exit(0);
}
