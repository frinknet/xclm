/* SEE LICENSE */

#include "xcmd.h"

xcmd_call (2, "win") {
	xcb_window_t win = strtoul(xcmd_next, NULL, 16);

	xmpl_pointer_center(xcmd_conn, win);

	xcmd_exit(0);
}
