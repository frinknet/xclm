/* SEE LICENSE */

#include "xcmd.h"

xcmd_simple {
	xcmd_win_exec(xcb_unmap_window(xcmd_conn, xcmd_win));
}
