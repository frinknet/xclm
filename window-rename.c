/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (3, "name") {
	char *name = xcmd_next;

	xcmd_win_exec(xmpl_window_rename(xcmd_conn, xcmd_win, name));
}
