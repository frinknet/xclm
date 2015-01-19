/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	xcb_window_t win = 0;

	if (argc < 2) {
		xcbtools_win_usage(argv[0], "");
	}

	xcbtools_conn_init(&conn);

	win = strtoul(argv[1], NULL, 16);

	if (xcbtools_win_exists(conn, win)) {
		xcb_set_input_focus(conn, XCB_INPUT_FOCUS_POINTER_ROOT, win,
				XCB_CURRENT_TIME);
	}

	xcbtools_conn_kill(&conn);

	return 0;
}
