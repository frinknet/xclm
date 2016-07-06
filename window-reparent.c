/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (5, "parent x y") {
	xcb_window_t parent = strtoul(xcmd_next, NULL, 16);
	int x = atoi(xcmd_next);
	int y = atoi(xcmd_next);
	xcb_void_cookie_t cookie;
	xcb_generic_error_t *error;

	if (!xcbtools_window_exists(xcmd_conn, parent)) {
		printf("0x%08x parent window does not exist\n", parent);

		xcmd_return(1);
	}

	xcmd_win_loop {
		cookie = xcb_reparent_window_checked(xcmd_conn, xcmd_win, parent, x, y);
		error = xcb_request_check(xcmd_conn, cookie);

		if (error != NULL) {
			printf("0x%08x failed to reparent %d\n", xcmd_win, error->error_code);

			xcmd_return(1);
		}
	}

	xcmd_exit(0);
}
