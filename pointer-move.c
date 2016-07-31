/* SEE LICENSE */

#include "xcmd.h"

xcmd_call (4, "x y win") {
	int x = atoi(xcmd_next);
	int y = atoi(xcmd_next);
	xcb_window_t win = strtoul(xcmd_next, NULL, 16);

	xmpl_pointer_move(xcmd_conn, win, x, y);

	xcmd_exit(0);
}
