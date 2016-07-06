/* SEE LICENSE */

#include "xcmd.h"

xcmd_simple {
	xcmd_win_loop {
		if (!xmpl_window_mapped(xcmd_conn, xcmd_win)) {
			xcmd_return(1);
		}
	}

	xcmd_exit(0);
}
