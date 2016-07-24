/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (3, "event-dir") {
	//xcb_window_t *child;

	uint32_t mask = XCB_EVENT_MASK_NO_EVENT
		| XCB_EVENT_MASK_STRUCTURE_NOTIFY
		| XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
		| XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
		| XCB_EVENT_MASK_ENTER_WINDOW
		| XCB_EVENT_MASK_LEAVE_WINDOW
		| XCB_EVENT_MASK_POINTER_MOTION
		| XCB_EVENT_MASK_POINTER_MOTION_HINT
		| XCB_EVENT_MASK_BUTTON_PRESS
		| XCB_EVENT_MASK_BUTTON_RELEASE
		| XCB_EVENT_MASK_BUTTON_MOTION
		| XCB_EVENT_MASK_BUTTON_1_MOTION
		| XCB_EVENT_MASK_BUTTON_2_MOTION
		| XCB_EVENT_MASK_BUTTON_3_MOTION
		| XCB_EVENT_MASK_BUTTON_4_MOTION
		| XCB_EVENT_MASK_BUTTON_5_MOTION
		//| XCB_EVENT_MASK_KEY_PRESS
		//| XCB_EVENT_MASK_KEY_RELEASE
		//| XCB_EVENT_MASK_KEYMAP_STATE
		| XCB_EVENT_MASK_EXPOSURE
		| XCB_EVENT_MASK_VISIBILITY_CHANGE
		| XCB_EVENT_MASK_RESIZE_REDIRECT
		| XCB_EVENT_MASK_FOCUS_CHANGE
		//| XCB_EVENT_MASK_PROPERTY_CHANGE
		//| XCB_EVENT_MASK_COLOR_MAP_CHANGE
		| XCB_EVENT_MASK_OWNER_GRAB_BUTTON
		;

	char *event_dir = xcmd_next;

	xcmd_win_loop {
		// xmpl_event_register(xcmd_conn, xcmd_win, mask);
		// xmpl_window_list_children(xcmd_conn, xcmd_win, &child);

		// while (*child) {
		// 	xmpl_event_register(xcmd_conn, *child++, mask);
		// }

		xmpl_event_loop(xcmd_conn, xcmd_win, event_dir, mask);
	}

	xcmd_exit(0);
}
