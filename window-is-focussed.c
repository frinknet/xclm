/* SEE LICENSE */

#include "xcmd.h"

xcmd_simple {
	xcmd_win_loop {
		if (xmpl_window_current(xcmd_conn) == xcmd_win) {
			xcmd_return(0);
		}
	}

	xcmd_exit(1);
}
