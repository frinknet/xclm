/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (2, "event-name") {
	char *event_dir = getenv("EVENTS");
	char *event_name = xcmd_next;
	xcb_screen_t *screen;

	xmpl_screen_init(xcmd_conn, &screen);


	if (xcmd_args < 3) {
		xmpl_event_trigger(xcmd_conn, screen->root, 0, event_name, event_dir? event_dir : "~/.events");

		xcmd_return(0);
	}

	xcmd_win_exec(xmpl_event_trigger(xcmd_conn, screen->root, xcmd_win, event_name, event_dir? event_dir : "~/.events"));
}
