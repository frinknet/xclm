/* SEE LICENSE */

#include "xcmd.h"

xcmd_simple {
	xcmd_win_exec(printf("%s\n", xmpl_window_get_class(xcmd_conn, xcmd_win)));
}
