/* SEE LICENSE */

#include "xcmd.h"

xcmd_simple {
	xcmd_win_exec(xcbtools_window_stack(xcmd_conn, xcmd_win, XCB_STACK_MODE_ABOVE));
}
