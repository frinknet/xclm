/* SEE LICENSE */

#include "xcmd.h"

xcmd_simple {
	xcmd_win_exec(printf("%s\n", xmpl_window_command(xcmd_conn, xcmd_win)));
}
