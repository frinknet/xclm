/* SEE LICENSE */

#include "xcmd.h"

xcmd_simple {
	xcb_query_pointer_reply_t *pointer;

	xcmd_win_loop {
		pointer = xmpl_pointer_position(xcmd_conn, xcmd_win);

		printf("%d %d\n", pointer->win_x, pointer->win_y);
	}

	xcmd_exit(0);
}
