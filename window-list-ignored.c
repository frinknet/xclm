/* SEE LICENSE */

#include "xcmd.h"

xcmd_simple {
	xcb_window_t win, *children;

	xcmd_win_loop {
		xmpl_window_list_children(xcmd_conn, xcmd_win, &children);

		while ((win = *children++)) {
			if (xmpl_window_is_ignored(xcmd_conn, win)) {
				printf("0x%08x", win);
			}
		}

		printf("\n");
	}

	xcmd_exit(0);
}
