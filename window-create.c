/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (6, "parent x y width height") {
	xcb_window_t parent = strtoul(*++argv, NULL, 16);
	int x = atoi(*++argv);
	int y = atoi(*++argv);
	int width = atoi(*++argv);
	int height = atoi(*++argv);
	xcb_window_t win = xmpl_window_create(xcmd_conn, parent, x, y, width, height);

	printf("0x%08x\n", win);

	xmpl_daemonize("/dev/null", "/dev/null");

	// start listening for redraw
	while (true) {
		//wait for event
		sleep(10);
		//force redraw
	}

	xcmd_exit(0);
}
