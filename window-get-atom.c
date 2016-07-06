/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (3, "atom-name") {
	char *atom_name = xcmd_next;

	xcmd_win_loop {
		printf("NUMBER: %i\n", xcbtools_atom(xcmd_conn, atom_name));
		printf("VALUE: %s\n", xcbtools_window_atom(xcmd_conn, xcmd_win, atom_name));
	}

	xcmd_exit(0);
}
