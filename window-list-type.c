#include "xcmd.h"

xcmd_windows (3, "class") {
	xcb_window_t win, *children;
	char *type = xcmd_next;

	xcmd_win_loop {
		xmpl_window_list_children(xcmd_conn, xcmd_win, &children);

		while ((win = *children++)) {
			if (!strcmp(type, xmpl_window_get_type(xcmd_conn, win))) {
				printf("0x%08x ", win);
			}
		}

		printf("\n");
	}

	xcmd_exit(0);
}
