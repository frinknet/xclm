/* SEE LICENSE */

#include "xcmd.h"

xcmd_simple {
	xcb_screen_t *screen;

	xcbtools_screen_init(xcmd_conn, &screen);

	xcmd_win_loop {
		while (xcmd_win != screen->root) {
			xcmd_win = xcbtools_window_parent(xcmd_conn, xcmd_win);

			printf("0x%08x ", xcmd_win);
		}

		printf("\n");
	}

	xcmd_exit(0);
}
