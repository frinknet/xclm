/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (4, "atom-name value") {
	char *atom_name = xcmd_next;
	char *value = xcmd_next;

	xcmd_win_exec(xmpl_window_set_atom(xcmd_conn, xcmd_win, atom_name, XCB_ATOM_STRING, value));
}
