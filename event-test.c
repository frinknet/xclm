/* See LICENSE */

#include "xcmd.h"
#include <xcb/xcb_aux.h>

xcmd_windows (3, "event-dir") {
	char *event_dir = xcmd_next;
	xcb_generic_event_t *xevent;

	const uint32_t mask = XCB_EVENT_MASK_NO_EVENT
		//
		| XCB_EVENT_MASK_STRUCTURE_NOTIFY
		| XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
		//| XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
		//| XCB_EVENT_MASK_VISIBILITY_CHANGE
		//| XCB_EVENT_MASK_ENTER_WINDOW
		//| XCB_EVENT_MASK_LEAVE_WINDOW
		//| XCB_EVENT_MASK_PROPERTY_CHANGE
		//| XCB_EVENT_MASK_FOCUS_CHANGE
		//| XCB_EVENT_MASK_EXPOSURE
		//| XCB_EVENT_MASK_COLOR_MAP_CHANGE
		//
		//| XCB_EVENT_MASK_POINTER_MOTION
		//| XCB_EVENT_MASK_POINTER_MOTION_HINT
		| XCB_EVENT_MASK_BUTTON_PRESS
		//| XCB_EVENT_MASK_BUTTON_RELEASE
		//| XCB_EVENT_MASK_BUTTON_1_MOTION
		//| XCB_EVENT_MASK_BUTTON_2_MOTION
		//| XCB_EVENT_MASK_BUTTON_3_MOTION
		//| XCB_EVENT_MASK_BUTTON_4_MOTION
		//| XCB_EVENT_MASK_BUTTON_5_MOTION
		//| XCB_EVENT_MASK_BUTTON_MOTION
		//| XCB_EVENT_MASK_OWNER_GRAB_BUTTON
		;

	xcmd_win_loop {
		xmpl_event_register(xcmd_conn, xcmd_win, mask);

		if (xcb_poll_for_event(xcmd_conn) != NULL) {
			fprintf(stderr, "another window manager is running for 0x%08x\n", xcmd_win);

			xcmd_return(1);
		}

		int done = 0;
		xcb_reparent_notify_event_t *xreparent;
		xcb_button_press_event_t *xbutton;

		while (!done && (xevent = xcb_wait_for_event(xcmd_conn))) {
			switch (xevent->response_type) {
				case XCB_REPARENT_NOTIFY:
					xreparent = (xcb_reparent_notify_event_t *) xevent;

					xmpl_event_trigger(xcmd_conn, xcmd_win, xreparent->window, "reparent-notify", event_dir, NULL);

					//switch (xreparent->parent) {
					//}

					break;
				case XCB_BUTTON_PRESS:
					xbutton = (xcb_button_press_event_t *) xevent;

					switch (xbutton->detail) {
						case 4:
							xmpl_event_trigger(xcmd_conn, xcmd_win, xbutton->event, "mouse-scroll-up", event_dir, NULL);

							break;
						case 5:
							xmpl_event_trigger(xcmd_conn, xcmd_win, xbutton->event, "mouse-scroll-down", event_dir, NULL);

							break;
						case 6:
							xmpl_event_trigger(xcmd_conn, xcmd_win, xbutton->event, "mouse-scroll-left", event_dir, NULL);

							break;
						case 7:
							xmpl_event_trigger(xcmd_conn, xcmd_win, xbutton->event, "mouse-scroll-right", event_dir, NULL);

							break;
					}
			}

			xmpl_free(xevent);
		}
	}

	xcmd_exit(0);
}
