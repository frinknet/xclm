/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (6, "x y width height") {
	int x = atoi(*++argv);
	int y = atoi(*++argv);
	int width = atoi(*++argv);
	int height = atoi(*++argv);

	xcmd_win_exec(xmpl_window_warp(xcmd_conn, xcmd_win, x, y, width, height));
}
