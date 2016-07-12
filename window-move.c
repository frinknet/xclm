/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (4, "x y") {
	int x = atoi(*++argv);
	int y = atoi(*++argv);

	xcmd_win_exec(xmpl_window_set_position(xcmd_conn, xcmd_win, x, y));
}
