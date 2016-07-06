/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (3, "class") {
	char *class = xcmd_next;

	xcmd_win_loop {
		if (strcmp(class, xcbtools_window_class(xcmd_conn, xcmd_win))) {
			xcmd_return(1);
		}
	}

	xcmd_exit(0);
}
