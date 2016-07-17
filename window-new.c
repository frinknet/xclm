/* SEE LICENSE */

#include "xcmd.h"

xcmd_call (7, "parent x y width height class") {
	xcb_window_t parent = strtoul(xcmd_next, NULL, 16);
	int x = atoi(xcmd_next);
	int y = atoi(xcmd_next);
	int width = atoi(xcmd_next);
	int height = atoi(xcmd_next);
	char *cls = xcmd_next;
	char *event_dir = getenv("EVENTS");
	char *event_path = (char *) malloc(1 + strlen(cls) + strlen(event_dir? event_dir : "~/.events"));

	xcb_window_t win = xmpl_window_create(xcmd_conn, parent, x, y, width, height, cls);

	printf("0x%08x\n", win);

	if (xmpl_fork("/dev/null", "/dev/null")) {
		return 0;
	} else {
		xmpl_event_loop(xcmd_conn, win, event_path);
	}

	xcmd_exit(0);
}
