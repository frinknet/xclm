/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (4, "width height") {
	int width = atoi(xcmd_next);
	int height = atoi(xcmd_next);

	xcmd_win_exec(xmpl_window_resize(xcmd_conn, xcmd_win, width, height));
}
