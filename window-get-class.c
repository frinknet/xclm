/* SEE LICENSE */

#include "xcmd.h"

xcmd_simple {
	xcmd_win_exec(printf("%s\n", xcbtools_window_class(xcmd_conn, xcmd_win)));
}
