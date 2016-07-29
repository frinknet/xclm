/* SEE LICENSE */

#include "xcmd.h"

xcmd_call (7, "parent x y width height class") {
	xcb_window_t parent = strtoul(xcmd_next, NULL, 16);
	int x = atoi(xcmd_next);
	int y = atoi(xcmd_next);
	int w = atoi(xcmd_next);
	int h = atoi(xcmd_next);
	char *cls = xcmd_next;
	char *event_dir = getenv("EVENTS");
	char *event_path = (char *) malloc(1 + strlen(cls) + strlen(event_dir? event_dir : "~/.events"));
	const uint32_t mask = XMPL_EVENT_MASK_NULL
		| XMPL_EVENT_MASK_STRUCTURE
		//| XMPL_EVENT_MASK_REDIRECT
		| XMPL_EVENT_MASK_FOCUS
		//| XMPL_EVENT_MASK_MOTION
		| XMPL_EVENT_MASK_BUTTONS
		//| XMPL_EVENT_MASK_KEYS
		//| XMPL_EVENT_MASK_EXPOSURE
		//| XMPL_EVENT_MASK_PROPERTIES
		;

	sprintf(event_path, "%s/%s", event_dir? event_dir : "~/.events", cls);

	xcb_window_t win = xmpl_window_create(xcmd_conn, parent, x, y, w, h, cls);

	printf("0x%08x\n", win);

	xmpl_logging("/dev/null", "/dev/null");

	if (fork() > 0) {
		 exit(0);
	} else {
		xmpl_event_watch(xcmd_conn, win, event_path, mask);
	}

	xcmd_exit(0);
}
