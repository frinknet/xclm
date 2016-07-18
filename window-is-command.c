/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (3, "command") {
	char *command = xcmd_next;

	xcmd_win_loop {
		if (strcmp(command, xmpl_window_get_command(xcmd_conn, xcmd_win))) {
			xcmd_return(1);
		}
	}

	xcmd_exit(0);
}
