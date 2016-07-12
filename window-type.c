/* SEE LICENSE */

#include "xcmd.h"

xcmd_simple {
	char *atom_name = "_NET_WM_WINDOW_TYPE";

	xcmd_win_loop {
		printf("%s\n", xmpl_window_get_atom(xcmd_conn, xcmd_win, atom_name));
	}

	xcmd_exit(0);
}
