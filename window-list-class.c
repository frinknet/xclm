#include "xcmd.h"

xcmd_windows (3, "class") {
	xcb_window_t *win;
	char *cls = xcmd_next;

	xcmd_win_loop {
		xmpl_window_list_children(xcmd_conn, xcmd_win, &win);

		while (*win++) {
			if (!strcmp(cls, xmpl_window_get_class(xcmd_conn, *win))) {
				printf("0x%08x ", *win);
			}
		}

		printf("\n");
	}

	xcmd_exit(0);
}
