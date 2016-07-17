/* SEE LICENSE */

#include "xcmd.h"

xcmd_call (7, "parent x y width height class") {
	xcb_window_t parent = strtoul(*++argv, NULL, 16);
	int x = atoi(xcmd_next);
	int y = atoi(xcmd_next);
	int width = atoi(xcmd_next);
	int height = atoi(xcmd_next);
	char *class = xcmd_next;

	xcb_window_t win = xmpl_window_create(xcmd_conn, parent, x, y, width, height, class);

	printf("0x%08x\n", win);

	if (xmpl_daemonize("/dev/null", "/dev/null")) {
		exit(0);
	}

	// start listening for redraw
	while (true) {
		//wait for event
		sleep(0.1);
		//force redraw
	}

	xcmd_exit(0);
}
