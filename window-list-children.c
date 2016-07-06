/* SEE LICENSE */

#include "xcmd.h"

xcmd_simple {
	xcb_window_t *win;

	xcmd_win_loop {
		xcbtools_window_children(xcmd_conn, xcmd_win, &win);

		while (*win++) {
			printf("0x%08x ", *win);
		}

		printf("\n");
	}

	xcmd_exit(0);
}
