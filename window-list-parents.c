/* SEE LICENSE */

#include "xcmd.h"

xcmd_simple {
	xcb_screen_t *screen;

	xmpl_screen_init(xcmd_conn, &screen);

	xcmd_win_loop {
		while (xcmd_win != screen->root) {
			xcmd_win = xmpl_window_get_parent(xcmd_conn, xcmd_win);

			printf("0x%08x ", xcmd_win);
		}

		printf("\n");
	}

	xcmd_exit(0);
}
