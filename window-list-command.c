#include "xcmd.h"

xcmd_windows (3, "command") {
	xcb_window_t *win;
	char *command = xcmd_next;

	xcmd_win_loop {
		xmpl_window_list_children(xcmd_conn, xcmd_win, &win);

		while (*win++) {
			if (!strcmp(command, xmpl_window_get_command(xcmd_conn, xcmd_win))) {
				printf("0x%08x ", *win);
			}
		}

		printf("\n");
	}

	xcmd_exit(0);
}
