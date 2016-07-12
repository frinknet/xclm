/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (3, "type") {
	char *type = xcmd_next;

	xcmd_win_loop {
		if (strcmp(type, xmpl_window_get_type(xcmd_conn, xcmd_win))) {
			xcmd_return(1);
		}
	}

	xcmd_exit(0);
}
