/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (3, "color") {
	int color = strtoul(xcmd_next, NULL, 16);

	xcmd_win_exec(xmpl_window_background(xcmd_conn, xcmd_win, color));
}
