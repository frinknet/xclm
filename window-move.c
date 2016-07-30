/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (4, "x y") {
	int x = atoi(xcmd_next);
	int y = atoi(xcmd_next);

	xcmd_win_exec(xmpl_window_move(xcmd_conn, xcmd_win, x, y));
}
