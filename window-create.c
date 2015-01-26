/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;
static xcb_screen_t *screen;
static uint32_t mask = XCB_EVENT_MASK_NO_EVENT
					//| XCB_EVENT_MASK_KEY_PRESS
					//| XCB_EVENT_MASK_KEY_RELEASE
					//| XCB_EVENT_MASK_BUTTON_PRESS
					//| XCB_EVENT_MASK_BUTTON_RELEASE
					| XCB_EVENT_MASK_ENTER_WINDOW
					| XCB_EVENT_MASK_LEAVE_WINDOW
					//| XCB_EVENT_MASK_POINTER_MOTION
					//| XCB_EVENT_MASK_POINTER_MOTION_HINT
					//| XCB_EVENT_MASK_BUTTON_1_MOTION
					//| XCB_EVENT_MASK_BUTTON_2_MOTION
					//| XCB_EVENT_MASK_BUTTON_3_MOTION
					//| XCB_EVENT_MASK_BUTTON_4_MOTION
					//| XCB_EVENT_MASK_BUTTON_5_MOTION
					//| XCB_EVENT_MASK_BUTTON_MOTION
					//| XCB_EVENT_MASK_KEYMAP_STATE
					| XCB_EVENT_MASK_EXPOSURE
					| XCB_EVENT_MASK_VISIBILITY_CHANGE
					| XCB_EVENT_MASK_STRUCTURE_NOTIFY
					| XCB_EVENT_MASK_RESIZE_REDIRECT
					| XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
					| XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
					| XCB_EVENT_MASK_FOCUS_CHANGE
					| XCB_EVENT_MASK_PROPERTY_CHANGE
					| XCB_EVENT_MASK_COLOR_MAP_CHANGE
					| XCB_EVENT_MASK_OWNER_GRAB_BUTTON;

int
main(int argc, char **argv)
{
	int x, y, width, height;
	xcb_window_t win, parent;
	char *event_dir;
	char *events;
	char *winclass;

	if (argc < 7) {
		xcbtools_usage(argv[0], "parent x y width height class");
	}

	xcbtools_conn_init(&conn);
	xcbtools_screen_init(conn, &screen);

	parent = strtoul(*++argv, NULL, 16);
	x = atoi(*++argv);
	y = atoi(*++argv);
	width = atoi(*++argv);
	height = atoi(*++argv);
	winclass = *++argv;

	win = xcbtools_window_create(conn, parent, x, y, width, height);

	events = getenv("EVENTS");
	event_dir = malloc(1 + strlen(events? events : "~/.events") + strlen(winclass));

	sprintf(event_dir, "%s/%s", events? events : "~/.events", winclass);
	printf("0x%08x\n", win);

	xcbtools_daemonize("/dev/null", "/dev/null");

	while (true) {
		sleep(10);
	}

	//xcbtools_event_register(conn, win, mask);
	//xcbtools_event_loop(conn, event_dir);

	xcbtools_conn_kill(&conn);

	return 0;
}
