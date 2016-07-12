/* SEE LICENSE */

#include "xcmd.h"

xcmd_simple {
	xcmd_win_exec(printf("0x%08x\n", xmpl_window_get_parent(xcmd_conn, xcmd_win)));
}
