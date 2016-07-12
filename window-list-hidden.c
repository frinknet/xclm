#include "xcmd.h"

xcmd_simple {
	xcb_window_t *win;

	xcmd_win_loop {
		xmpl_window_list_children(xcmd_conn, xcmd_win, &win);

		while (*win++) {
			if (!xmpl_window_is_mapped(xcmd_conn, *win) && !xmpl_window_is_ignored(xcmd_conn, *win)) {
				printf("0x%08x ", *win);
			}
		}

		printf("%s", "\n");
	}

	xcmd_exit(0);
}
