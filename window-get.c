/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (3, "atom-name") {
	char *atom_name = xcmd_next;

	xcmd_win_exec(printf("%s\n", xmpl_window_get_atom(xcmd_conn, xcmd_win, atom_name)));
}
