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
	const uint32_t mask = XCB_EVENT_MASK_NO_EVENT
		//
		| XCB_EVENT_MASK_STRUCTURE_NOTIFY
		| XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
		| XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
		| XCB_EVENT_MASK_VISIBILITY_CHANGE
		| XCB_EVENT_MASK_ENTER_WINDOW
		| XCB_EVENT_MASK_LEAVE_WINDOW
		//| XCB_EVENT_MASK_PROPERTY_CHANGE
		| XCB_EVENT_MASK_FOCUS_CHANGE
		| XCB_EVENT_MASK_EXPOSURE
		//| XCB_EVENT_MASK_COLOR_MAP_CHANGE
		//
		| XCB_EVENT_MASK_POINTER_MOTION
		| XCB_EVENT_MASK_POINTER_MOTION_HINT
		| XCB_EVENT_MASK_BUTTON_PRESS
		| XCB_EVENT_MASK_BUTTON_RELEASE
		| XCB_EVENT_MASK_BUTTON_1_MOTION
		| XCB_EVENT_MASK_BUTTON_2_MOTION
		| XCB_EVENT_MASK_BUTTON_3_MOTION
		| XCB_EVENT_MASK_BUTTON_4_MOTION
		| XCB_EVENT_MASK_BUTTON_5_MOTION
		| XCB_EVENT_MASK_BUTTON_MOTION
		| XCB_EVENT_MASK_OWNER_GRAB_BUTTON;

	sprintf(event_path, "%s/%s", event_dir? event_dir : "~/.events", cls);

	xcb_window_t win = xmpl_window_create(xcmd_conn, parent, x, y, w, h, cls);

	printf("0x%08x\n", win);

	if (xmpl_fork("/dev/null", "/dev/null")) {
		 exit(0);
	} else {
		xmpl_event_watch(xcmd_conn, win, event_path, mask);
	}

	xcmd_exit(0);
}
