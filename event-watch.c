/* SEE LICENSE */

#include "xcmd.h"

xcmd_windows (3, "event-dir") {
	char *event_dir = xcmd_next;
	uint32_t mask = XMPL_EVENT_MASK_NULL
		| XMPL_EVENT_MASK_STRUCTURE
		| XMPL_EVENT_MASK_REDIRECT
		| XMPL_EVENT_MASK_FOCUS
		//| XMPL_EVENT_MASK_MOTION
		//| XMPL_EVENT_MASK_BUTTONS
		//| XMPL_EVENT_MASK_KEYS
		| XMPL_EVENT_MASK_EXPOSURE
		//| XMPL_EVENT_MASK_PROPERTIES
		;

	xcmd_win_exec(xmpl_event_watch(xcmd_conn, xcmd_win, event_dir, mask));
}
