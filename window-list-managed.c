/* SEE LICENSE */

#include "xcmd.h"

xcmd_simple {
	xcb_window_t *win;

	xcmd_win_loop {
		xmpl_window_children(xcmd_conn, xcmd_win, &win);

		while (*win++) {
			if (!xmpl_window_ignored(xcmd_conn, *win)) {
				printf("0x%08x ", *win);
			}
		}

		printf("%s", "\n");
	}

	xcmd_exit(0);
}
