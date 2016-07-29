/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (3, "atom-name") {
	char *atom_name = xcmd_next;

	setbuf(stdout, NULL);

	xcmd_win_loop {
		printf("%s\n", (char *) xmpl_window_get_atom(xcmd_conn, xcmd_win, atom_name, XCB_ATOM_STRING));
	}

	xcmd_exit(0);
}
