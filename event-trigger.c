/* SEE LICENSE */

#include "xcmd.h"

xcmd_call (2, "event-name [..wid]") {
	char *event_dir = getenv("EVENTS");
	char *event_name = xcmd_next;
	xcb_screen_t *screen;

	xmpl_screen_init(xcmd_conn, &screen);

	event_dir = event_dir? event_dir : "~/.events";

	printf("event-dir %s\n", event_dir);

	if (xcmd_args < 3) {
		xmpl_event_trigger(xcmd_conn, screen->root, 0, event_name, event_dir);

		xcmd_return(0);
	}

	xcmd_win_exec(xmpl_event_trigger(xcmd_conn, screen->root, xcmd_win, event_name, event_dir));
}
