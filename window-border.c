/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (4, "color") {
	int size = strtoul(xcmd_next, NULL, 10);
	int color = strtoul(xcmd_next, NULL, 16);

	xcmd_win_exec(xmpl_window_border(xcmd_conn, xcmd_win, size, color));
}
