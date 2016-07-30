#include "xmpl.h"

/**
 * change logging
 */
void
xmpl_logging(char *out, char *err)
{
	int fout, ferr;

	if (out) {
		fout = open(out, O_WRONLY);

		fflush(stdout);
		dup2(fout, 1);
		close(fout);
	}

	if (err) {
		ferr = open(err, O_WRONLY);

		fflush(stderr);
		dup2(ferr, 2);
		close(ferr);
	}
}


void
xmpl_free(void *var)
{
	if (var) {
		free(var);
	}
}

/**
 * Intialize the Connection
 */
void
xmpl_conn_init(xcb_connection_t **conn)
{
	*conn = xcb_connect(NULL, NULL);

	if (xcb_connection_has_error(*conn)) {
		errx(1, "unable connect to the X server");
	}
}

/**
 * Kill the Connection
 */
void
xmpl_conn_kill(xcb_connection_t **conn)
{
	if (*conn) {
		xmpl_conn_sync(*conn);
		xcb_disconnect(*conn);
	}
}

/**
 * Sync the Connection
 */
void
xmpl_conn_sync(xcb_connection_t *conn)
{
	xcb_flush(conn);

	xmpl_free(xcb_get_input_focus_reply(conn, xcb_get_input_focus(conn), NULL));
}

/**
 * Initialize the Screen
 */
void
xmpl_screen_init(xcb_connection_t *conn, xcb_screen_t **screen)
{
	*screen = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;

	if (*screen == NULL) {
		errx(1, "unable to retrieve screen informations");
	}
}

/**
 * Initialize the Color
 */
void
xmpl_color_init(xcb_connection_t *conn, xcb_screen_t *screen, xcb_colormap_t *colormap, xcb_visualid_t *visual)
{
	*colormap = xcb_generate_id(conn);
	*visual = screen->root_visual;
	xcb_depth_iterator_t depth = xcb_screen_allowed_depths_iterator(screen);

	while (depth.rem) {
		xcb_visualtype_t *type = xcb_depth_visuals(depth.data);

		if (depth.data->depth == 32) {
			*visual = type->visual_id;

			break;
		}

		xcb_depth_next(&depth);
	}

	xcb_create_colormap(conn, XCB_COLORMAP_ALLOC_NONE, *colormap, screen->root, *visual);
}

/**
 * Get Attibrutes for Window
 */
xcb_get_window_attributes_reply_t *
xmpl_window_get_attributes(xcb_connection_t *conn, xcb_window_t win)
{
	xcb_get_window_attributes_cookie_t cookie = xcb_get_window_attributes(conn, win);
	xcb_get_window_attributes_reply_t  *reply = xcb_get_window_attributes_reply(conn, cookie, NULL);

	return reply;
}

/**
 * Get Atom for Window
 */
void *
xmpl_window_get_atom(xcb_connection_t *conn, xcb_window_t win, char *atom_name, xcb_atom_t type)
{
	xcb_atom_t atom;

	atom = xmpl_atom(conn, atom_name);

	return xmpl_window_get_property(conn, win, atom, type);
}

/**
 * Set Atom for Window
 */
void
xmpl_window_set_atom(xcb_connection_t *conn, xcb_window_t win, char *atom_name, xcb_atom_t type, void *value)
{
	xcb_atom_t atom;

	atom = xmpl_atom(conn, atom_name);

	xmpl_window_set_property(conn, win, atom, type, value);
}

/**
 * Get Property for Window
 */
void *
xmpl_window_get_property(xcb_connection_t *conn, xcb_window_t win, xcb_atom_t prop, xcb_atom_t type)
{
	xcb_get_property_cookie_t cookie;
	xcb_get_property_reply_t *reply;
	void *value = NULL;

	cookie = xcb_get_property(conn, 0, win, prop, type, 0L, UINT_MAX);

	reply = xcb_get_property_reply(conn, cookie, NULL);

	if (reply) {
		value = xcb_get_property_value(reply);
	} else if (type == XCB_ATOM_STRING) {
		value = "";
	}

	xmpl_free(reply);

	return value;
}

/**
 * Set Property for Window
 */
void
xmpl_window_set_property(xcb_connection_t *conn, xcb_window_t win, xcb_atom_t prop, xcb_atom_t type, void *value)
{
	xcb_change_property(conn, XCB_PROP_MODE_REPLACE, win, prop, type, 8, strlen(value), value);
}

/**
 * Get Name for Window
 */
char *
xmpl_window_name(xcb_connection_t *conn, xcb_window_t win)
{
	return xmpl_window_get_property(conn, win, XCB_ATOM_WM_NAME, XCB_ATOM_STRING);
}

/**
 * Get Class for Window
 */
char *
xmpl_window_class(xcb_connection_t *conn, xcb_window_t win)
{
	return xmpl_window_get_property(conn, win, XCB_ATOM_WM_CLASS, XCB_ATOM_STRING);
}

/**
 * Get Command for Window
 */
char *
xmpl_window_command(xcb_connection_t *conn, xcb_window_t win)
{
	return xmpl_window_get_property(conn, win, XCB_ATOM_WM_COMMAND, XCB_ATOM_STRING);
}

/**
 * Get Type for Window
 */
char *
xmpl_window_type(xcb_connection_t *conn, xcb_window_t win)
{
	xcb_atom_t *atom;

	atom = (xcb_atom_t *) xmpl_window_get_property(conn, win, xmpl_atom(conn, "_NET_WM_WINDOW_TYPE"), XCB_ATOM_ATOM);

	if (!atom) {
		return "";
	}

	return xmpl_atom_name(conn, *atom);
}

/**
 * Set Ignore for Window
 */
void
xmpl_window_set_ignore(xcb_connection_t *conn, xcb_window_t win, int override)
{
	xcb_change_window_attributes(conn, win, XCB_CW_OVERRIDE_REDIRECT, (uint32_t[]){ override });
	xmpl_conn_sync(conn);
}

/**
 * Set Background for Window
 */
void
xmpl_window_background(xcb_connection_t *conn, xcb_window_t win, uint32_t color)
{
	xcb_change_window_attributes(conn, win, XCB_CW_BACK_PIXEL, (uint32_t[]){ color });

	xcb_get_geometry_reply_t *geom = xmpl_window_geometry(conn, win);
	xcb_clear_area(conn, 1, win, 0, 0, geom->width, geom->height);

	xmpl_free(geom);
	xmpl_conn_sync(conn);
}

/**
 * Set Border for Window
 */
void
xmpl_window_border(xcb_connection_t *conn, xcb_window_t win, uint32_t size, uint32_t color)
{
	xcb_change_window_attributes(conn, win, XCB_CW_BORDER_PIXEL, (uint32_t[]){ color });
	xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_BORDER_WIDTH, (uint32_t[]){ size });

	xmpl_conn_sync(conn);
}

/**
 * Set Stack for Window
 */
void
xmpl_window_set_stack(xcb_connection_t *conn, xcb_window_t win, uint32_t stack)
{
	xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_STACK_MODE, (uint32_t[]){ stack });
	xmpl_conn_sync(conn);
}

/**
 * Set Size for Window
 */
void
xmpl_window_resize(xcb_connection_t *conn, xcb_window_t win, int w, int h)
{
	xcb_configure_window(
		conn, win,
		XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
		(uint32_t[]){ w, h }
	);

	xmpl_conn_sync(conn);
}

/**
 * Set Position for Window
 */
void
xmpl_window_move(xcb_connection_t *conn, xcb_window_t win, int x, int y)
{
	xcb_configure_window(
		conn, win,
		XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
		(uint32_t[]){ x, y }
	);

	xmpl_conn_sync(conn);
}

/**
 * Set Geometry for Window
 */
void
xmpl_window_warp(xcb_connection_t *conn, xcb_window_t win, int x, int y, int w, int h)
{
	xcb_configure_window(
		conn, win,
			XCB_CONFIG_WINDOW_X |
			XCB_CONFIG_WINDOW_Y |
			XCB_CONFIG_WINDOW_WIDTH |
			XCB_CONFIG_WINDOW_HEIGHT,
		(uint32_t[]){ x, y, w, h }
	);

	xmpl_conn_sync(conn);
}

/**
 * Get Geometry for Window
 */
xcb_get_geometry_reply_t *
xmpl_window_geometry(xcb_connection_t *conn, xcb_window_t win)
{
	xcb_get_geometry_cookie_t cookie;
	xcb_get_geometry_reply_t *reply;

	cookie = xcb_get_geometry(conn, win);
	reply = xcb_get_geometry_reply(conn, cookie, NULL);

	if (!reply) {
		errx(1, "unable to retrieve geometry!");
	}

	return reply;
}

/**
 * Check that Window is Valid
 */
int
xmpl_window_is_valid(xcb_connection_t *conn, xcb_window_t win)
{
	xcb_get_window_attributes_reply_t  *attr;

	attr = xmpl_window_get_attributes(conn, win);

	if (attr == NULL) {
		return 0;
	}

	xmpl_free(attr);

	return 1;
}

/**
 * Check that Window is Bordered
 */
int
xmpl_window_is_bordered(xcb_connection_t *conn, xcb_window_t win)
{
	int border;
	xcb_get_geometry_reply_t *geom = xmpl_window_geometry(conn, win);

	if (geom == NULL) {
		return 0;
	}

	border = geom->border_width;

	xmpl_free(geom);

	return border > 0;
}

/**
 * Check that Window is Mapped
 */
int
xmpl_window_is_mapped(xcb_connection_t *conn, xcb_window_t win)
{
	int mapped;
	xcb_get_window_attributes_reply_t  *attr;

	attr = xmpl_window_get_attributes(conn, win);

	if (attr == NULL) {
		return 0;
	}

	mapped = attr->map_state;

	xmpl_free(attr);

	return mapped == XCB_MAP_STATE_VIEWABLE;
}

/**
 * Check that Window is Ignored
 */
int
xmpl_window_is_ignored(xcb_connection_t *conn, xcb_window_t win)
{
	int override;
	xcb_get_window_attributes_reply_t *attr;

	attr = xmpl_window_get_attributes(conn, win);

	if (attr == NULL) {
		return 0;
	}

	override = attr->override_redirect;

	xmpl_free(attr);

	return override;
}

/**
 * List Window Children
 */
int
xmpl_window_list_children(xcb_connection_t *conn, xcb_window_t win, xcb_window_t **list)
{
	uint32_t childnum = 0;
	xcb_query_tree_cookie_t cookie;
	xcb_query_tree_reply_t *reply;

	cookie = xcb_query_tree(conn, win);
	reply = xcb_query_tree_reply(conn, cookie, NULL);

	if (reply == NULL) {
		errx(1, "0x%08x: no such window", win);
	}

	*list = malloc(sizeof(xcb_window_t) * reply->children_len);

	memcpy(*list, xcb_query_tree_children(reply),
			sizeof(xcb_window_t) * reply->children_len);

	childnum = reply->children_len;

	xmpl_free(reply);

	return childnum;
}

/**
 * Get Window Parent
 */
xcb_window_t
xmpl_window_parent(xcb_connection_t *conn, xcb_window_t win)
{
	xcb_window_t parent;
	xcb_query_tree_cookie_t cookie;
	xcb_query_tree_reply_t *reply;

	cookie = xcb_query_tree(conn, win);
	reply = xcb_query_tree_reply(conn, cookie, NULL);

	if (reply == NULL) {
		errx(1, "0x%08x: no such window", win);
	}

	parent = reply->parent;

	xmpl_free(reply);

	return parent;
}

/**
 * Create a new Window
 */
xcb_window_t
xmpl_window_create(xcb_connection_t *conn, xcb_window_t parent, int x, int y, int w, int h, char *cls)
{
	xcb_window_t win = xcb_generate_id(conn);
	xcb_screen_t *screen;
	xcb_colormap_t colormap;
	xcb_visualid_t visual;

	xmpl_screen_init(conn, &screen);
	xmpl_color_init(conn, screen, &colormap, &visual);

	xcb_create_window(
		conn,
		(visual == screen->root_visual)? XCB_COPY_FROM_PARENT : 32,
		win,
		parent,
		x, y, w, h, 0,
		XCB_WINDOW_CLASS_INPUT_OUTPUT,
		visual,
			XCB_CW_BORDER_PIXEL |
			XCB_CW_COLORMAP,
		(unsigned int[]) {
			0,
			colormap
		}
	);

	xmpl_window_set_property(conn, win, XCB_ATOM_WM_CLASS, XCB_ATOM_STRING, cls);

	return win;
}

/**
 * Get Current Window
 */
xcb_window_t
xmpl_window_current(xcb_connection_t *conn)
{
	xcb_window_t win = 0;

	xcb_get_input_focus_cookie_t cookie;
	xcb_get_input_focus_reply_t *reply;

	cookie = xcb_get_input_focus(conn);
	reply = xcb_get_input_focus_reply(conn, cookie, NULL);

	if (reply == NULL) {
		errx(1, "xcb_get_input_focus");
	}

	win = reply->focus;

	xmpl_free(reply);

	return win;
}

/**
 * Get Atom based on Atom Name
 */
xcb_atom_t
xmpl_atom(xcb_connection_t *conn, char *atom_name)
{
	xcb_intern_atom_cookie_t cookie;
	xcb_intern_atom_reply_t *reply;
	xcb_atom_t atom;

	if (atom_name == NULL) {
	  return XCB_NONE;
	}

	cookie = xcb_intern_atom(conn, 0, strlen(atom_name), atom_name);
	reply = xcb_intern_atom_reply(conn, cookie, NULL);

	if (!reply) {
		fprintf(stderr, "Could not create atom for %s\n", atom_name);
		
		exit(1);
	}

	atom = reply->atom;

	xmpl_free(reply);

	return atom;
}

/**
 * Get Atom Name based on Atom
 */
char *
xmpl_atom_name(xcb_connection_t *conn, xcb_atom_t atom)
{
	xcb_get_atom_name_cookie_t cookie;
	xcb_get_atom_name_reply_t *reply;
	char *atom_name;

	if (atom == XCB_NONE) {
	  return "";
	}

	cookie = xcb_get_atom_name(conn, atom);
	reply = xcb_get_atom_name_reply(conn, cookie, NULL);

	if (!reply) {
	  return "";
	}

	atom_name = xcb_get_atom_name_name(reply);

	xmpl_free(reply);

	return atom_name;
}

/**
 * Test Whether A Configure Event Should Happen
 */
static void
xmpl_event_configure_request(xcb_connection_t *conn, xcb_configure_request_event_t *event)
{
	u_int16_t mask = 0;
	u_int32_t vals[7];
	short i = 0;

	if (event->value_mask & XCB_CONFIG_WINDOW_X) {
		mask |= XCB_CONFIG_WINDOW_X;
		vals[i++] = event->x;
	}

	if (event->value_mask & XCB_CONFIG_WINDOW_Y) {
		mask |= XCB_CONFIG_WINDOW_Y;
		vals[i++] = event->y;
	}

	if (event->value_mask & XCB_CONFIG_WINDOW_WIDTH) {
		mask |= XCB_CONFIG_WINDOW_WIDTH;
		vals[i++] = event->width;
	}

	if (event->value_mask & XCB_CONFIG_WINDOW_HEIGHT) {
		mask |= XCB_CONFIG_WINDOW_HEIGHT;
		vals[i++] = event->height;
	}

	if (event->value_mask & XCB_CONFIG_WINDOW_BORDER_WIDTH) {
		mask |= XCB_CONFIG_WINDOW_BORDER_WIDTH;
		vals[i++] = event->border_width;
	}

	if (event->value_mask & XCB_CONFIG_WINDOW_SIBLING) {
		mask |= XCB_CONFIG_WINDOW_SIBLING;
		vals[i++] = event->sibling;
	}

	if (event->value_mask & XCB_CONFIG_WINDOW_STACK_MODE) {
		mask |= XCB_CONFIG_WINDOW_STACK_MODE;
		vals[i++] = event->stack_mode;
	}

	xcb_configure_window(conn, event->window, mask,vals);
	xcb_flush(conn);
}

/*
 * Test Whether A Notify Event Should Happen
 */
static bool
xmpl_event_notify_valid(xcb_connection_t *conn, xcb_generic_event_t *event)
{
	xcb_enter_notify_event_t *notify;

	notify = (xcb_enter_notify_event_t*)event;

	switch (notify->mode) {
	case XCB_NOTIFY_MODE_NORMAL:
	case XCB_NOTIFY_MODE_UNGRAB:
		if (notify->detail != XCB_NOTIFY_DETAIL_INFERIOR) {
			return true;
		}
	}

	return false;
}

/**
 * Event Environment
 */
static char **
xmpl_event_environment(xcb_window_t root, xcb_window_t win, char *env)
{
	char *x_display = getenv("DISPLAY");
	char *events_dir = getenv("EVENTS");
	char *path_dir = getenv("PATH");
	char *cmd_dir = getenv("COMMANDS");
	char *settings[8];
	int i = 0;

	settings[i] = (char *) malloc(15);

	sprintf(settings[i++], "ROOT=0x%08x", root);

	settings[i] = (char *) malloc(17);

	if (win) {
		sprintf(settings[i++], "WINDOW=0x%08x", win);
	} else {
		sprintf(settings[i++], "WINDOW=");
	}

	if (!events_dir) {
		events_dir = (char *) malloc(10);
		events_dir = "~/.events";
	}

	settings[i] = (char *) malloc(8 + strlen(events_dir));

	sprintf(settings[i++], "EVENTS=%s", events_dir);

	if (!cmd_dir) {
		cmd_dir = (char *) malloc(2);
		cmd_dir = ".";
	}

	settings[i] = (char *) malloc(10 + strlen(cmd_dir));

	sprintf(settings[i++], "COMMANDS=%s", cmd_dir);

	if (!path_dir) {
		path_dir = (char *) malloc(2);
		path_dir = ".";
	}

	settings[i] = (char *) malloc(6 + strlen(cmd_dir) + strlen(path_dir));

	sprintf(settings[i++], "PATH=%s:%s", path_dir, cmd_dir);

	settings[i] = (char *) malloc(8 + strlen(x_display));

	sprintf(settings[i++], "DISPLAY=%s", x_display);

	settings[i] = env;
	settings[++i] = NULL;

	char **ret = malloc(sizeof(settings));

	memcpy(ret, settings, sizeof(settings));

	return ret;
}

/**
 * Register Events
 */
void
xmpl_event_register(xcb_connection_t *conn, xcb_window_t win, uint32_t mask)
{
	xcb_window_t *children;
	xcb_grab_pointer_cookie_t pointer_cookie;
	xcb_void_cookie_t button_cookie;
	xcb_void_cookie_t key_cookie;
	xcb_void_cookie_t cookie;
	xcb_generic_error_t *error;

	if (!xmpl_window_is_valid(conn, win)) {
		fprintf(stderr, "Invalid window 0x%08x\n", win);

		return;
	}

	fprintf(stderr, "Registering structure 0x%08x\n", win);

	cookie = xcb_change_window_attributes_checked(conn, win, XCB_CW_EVENT_MASK, (uint32_t[]){ mask
			& ~XMPL_EVENT_MASK_POINTER
			& ~XMPL_EVENT_MASK_BUTTONS
			& ~XMPL_EVENT_MASK_KEYS
		});
	error = xcb_request_check(conn, cookie);

	if (error == NULL && mask & XMPL_EVENT_MASK_POINTER) {
		fprintf(stderr, "Registering pointer 0x%08x\n", win);
		pointer_cookie = xcb_grab_pointer(conn, false, win, mask & XMPL_EVENT_MASK_POINTER,
			XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC, XCB_NONE, XCB_NONE, XCB_TIME_CURRENT_TIME);

		xcb_grab_pointer_reply(conn, pointer_cookie, &error);
	}

	if (error == NULL && mask & XMPL_EVENT_MASK_BUTTONS) {
		fprintf(stderr, "Registering buttons 0x%08x\n", win);
		button_cookie = xcb_grab_button(conn, false, win, mask & XMPL_EVENT_MASK_BUTTONS,
			XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC, XCB_NONE, XCB_NONE,
			XCB_BUTTON_INDEX_ANY, XCB_MOD_MASK_ANY);

		error = xcb_request_check(conn, button_cookie);
	}

	if (error == NULL && mask & XMPL_EVENT_MASK_KEYS) {
		fprintf(stderr, "Registering keys 0x%08x\n", win);
		key_cookie = xcb_grab_key(conn, false, win, mask & XMPL_EVENT_MASK_KEYS,
			XCB_GRAB_ANY, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);

		error = xcb_request_check(conn, key_cookie);
	}

	if (error != NULL) {
		fprintf(stderr, "Unable to register events for 0x%08x\n", win);

		return;
	}

	for (xmpl_window_list_children(conn, win, &children); *children; ++children) {
		xmpl_event_register(conn, *children, mask);
	}

	xmpl_conn_sync(conn);
}

/**
 * Register events and run an Event Loop
 */
void
xmpl_event_watch(xcb_connection_t *conn, xcb_window_t root, char *event_dir, uint32_t mask)
{
	char *event_name = malloc(32);
	char *env = NULL;
	xcb_generic_event_t *event;
	xcb_button_press_event_t *press_event;
	xcb_button_release_event_t *release_event;
	xcb_reparent_notify_event_t *reparent_event;

	xmpl_event_register(conn, root, mask);
	xmpl_event_trigger(conn, root, root, "watch-start", event_dir, env);

	while (conn) {
		event = xcb_wait_for_event(conn);

		switch (event->response_type & ~0x80) {
			case 0:
				break;
			case XCB_CONFIGURE_REQUEST:
				xmpl_event_configure_request(conn, (xcb_configure_request_event_t*) event);

				break;
			case XCB_CIRCULATE_REQUEST:
				xmpl_event_trigger(conn, root, ((xcb_circulate_request_event_t*)event)->window, "restack-start", event_dir, env);

				break;
			case XCB_RESIZE_REQUEST:
				xmpl_event_trigger(conn, root, ((xcb_resize_request_event_t*)event)->window, "WINDOW-RESIZE", event_dir, env);

				break;
			case XCB_SELECTION_REQUEST:
				xmpl_event_trigger(conn, root, ((xcb_selection_request_event_t*)event)->owner, "select-start", event_dir, env);

				break;
			case XCB_MAP_REQUEST:
				xcb_map_window(conn, ((xcb_map_notify_event_t*)event)->window);
				xcb_flush(conn);
				xmpl_event_register(conn, ((xcb_map_notify_event_t*)event)->window, mask);

				break;
			case XCB_KEY_PRESS:
				xmpl_event_trigger(conn, root, ((xcb_key_press_event_t*)event)->event, "key-down", event_dir, env);

				break;
			case XCB_KEY_RELEASE:
				xmpl_event_trigger(conn, root, ((xcb_key_release_event_t*)event)->event, "key-up", event_dir, env);

				break;
			case XCB_BUTTON_PRESS:
				press_event = (xcb_button_press_event_t *) event;

				switch (press_event->detail) {
					case 1:
						sprintf(event_name, "mouse-down-left");

						break;
					case 2:
						sprintf(event_name, "mouse-down-middle");

						break;
					case 3:
						sprintf(event_name, "mouse-down-right");

						break;
					case 4:
						sprintf(event_name, "mouse-scroll-up");

						break;
					case 5:
						sprintf(event_name, "mouse-scroll-down");

						break;
					case 6:
						sprintf(event_name, "mouse-scroll-left");

						break;
					case 7:
						sprintf(event_name, "mouse-scroll-right");

						break;
					default:
						sprintf(event_name, "mouse-down-button-%d", press_event->detail);
				}

				xmpl_event_trigger(conn, root, press_event->event, event_name, event_dir, env);

				break;
			case XCB_BUTTON_RELEASE:
				release_event = (xcb_button_release_event_t *) event;

				switch (release_event->detail) {
					case 1:
						sprintf(event_name, "mouse-up-left");

						break;
					case 2:
						sprintf(event_name, "mouse-up-middle");

						break;
					case 3:
						sprintf(event_name, "mouse-up-right");

						break;
					case 4:
					case 5:
					case 6:
					case 7:
						//avoid release for scroll buttons

						continue;
					default:
						sprintf(event_name, "mouse-up-button-%d", release_event->detail);
				}

				xmpl_event_trigger(conn, root, release_event->event, event_name, event_dir, env);
				xmpl_free(release_event);

				break;
			case XCB_MOTION_NOTIFY:
				xmpl_event_trigger(conn, root, ((xcb_motion_notify_event_t*)event)->event, "mouse-move", event_dir, env);

				break;
			case XCB_ENTER_NOTIFY:
				if (xmpl_event_notify_valid(conn, event)) {
					xmpl_event_trigger(conn, root, ((xcb_enter_notify_event_t*)event)->event, "window-enter", event_dir, env);
				}

				break;
			case XCB_LEAVE_NOTIFY:
				if (xmpl_event_notify_valid(conn, event)) {
					xmpl_event_trigger(conn, root, ((xcb_enter_notify_event_t*)event)->event, "window-leave", event_dir, env);
				}

				break;
			case XCB_FOCUS_IN:
				xmpl_event_trigger(conn, root, ((xcb_focus_in_event_t*)event)->event, "window-focus", event_dir, env);

				break;
			case XCB_FOCUS_OUT:
				xmpl_event_trigger(conn, root, ((xcb_focus_in_event_t*)event)->event, "window-blur", event_dir, env);

				break;
			case XCB_KEYMAP_NOTIFY:
				xmpl_event_trigger(conn, root, ((xcb_button_release_event_t*)event)->event, "keymap-notify", event_dir, env);

				break;
			case XCB_EXPOSE:
				xmpl_event_trigger(conn, root, ((xcb_expose_event_t*)event)->window, "window-expose", event_dir, env);

				break;
			case XCB_GRAPHICS_EXPOSURE:
				xmpl_event_trigger(conn, root, ((xcb_expose_event_t*)event)->window, "window-exposure", event_dir, env);

				break;
			case XCB_NO_EXPOSURE:
				xmpl_event_trigger(conn, root, ((xcb_expose_event_t*)event)->window, "window-no-expose", event_dir, env);

				break;
			case XCB_VISIBILITY_NOTIFY:
				xmpl_event_trigger(conn, root, ((xcb_visibility_notify_event_t*)event)->window, "window-visible", event_dir, env);

				break;
			case XCB_CREATE_NOTIFY:
				xmpl_event_trigger(conn, root, ((xcb_create_notify_event_t*)event)->window, "window-create", event_dir, env);
				xmpl_event_register(conn, ((xcb_create_notify_event_t*)event)->window, mask);

				break;
			case XCB_DESTROY_NOTIFY:
				xmpl_event_trigger(conn, root, ((xcb_create_notify_event_t*)event)->window, "window-destroy", event_dir, env);

				break;
			case XCB_UNMAP_NOTIFY:
				xmpl_event_trigger(conn, root, ((xcb_unmap_notify_event_t*)event)->window, "window-hide", event_dir, env);

				break;
			case XCB_MAP_NOTIFY:
				xmpl_event_trigger(conn, root, ((xcb_map_notify_event_t*)event)->window, "window-show", event_dir, env);

				break;
			case XCB_REPARENT_NOTIFY:
				reparent_event = (xcb_reparent_notify_event_t *) event;

				if (reparent_event->parent == root) {
					xmpl_event_trigger(conn, root, reparent_event->window, "reparent-incoming", event_dir, env);
				} else {
					xmpl_event_trigger(conn, root, reparent_event->window, "reparent-outgoing", event_dir, env);
				}

				xmpl_free(reparent_event);

				break;
			case XCB_CONFIGURE_NOTIFY:
				xmpl_event_trigger(conn, root, ((xcb_configure_notify_event_t*)event)->window, "config-finish", event_dir, env);

				break;
			case XCB_GRAVITY_NOTIFY:
				xmpl_event_trigger(conn, root, ((xcb_gravity_notify_event_t*)event)->window, "gravity", event_dir, env);

				break;
			case XCB_CIRCULATE_NOTIFY:
				xmpl_event_trigger(conn, root, ((xcb_circulate_notify_event_t*)event)->window, "restack-finish", event_dir, env);

				break;
			case XCB_PROPERTY_NOTIFY:
				sprintf(event_name, "property/%s", xmpl_atom_name(conn, ((xcb_property_notify_event_t*)event)->atom));

				xmpl_event_trigger(conn, root, ((xcb_property_notify_event_t*)event)->window, event_name, event_dir, env);

				break;
			case XCB_SELECTION_CLEAR:
				xmpl_event_trigger(conn, root, ((xcb_selection_clear_event_t*)event)->owner, "select-clear", event_dir, env);

				break;
			case XCB_SELECTION_NOTIFY:
				xmpl_event_trigger(conn, root, ((xcb_selection_notify_event_t*)event)->requestor, "select-end", event_dir, env);

				break;
			case XCB_COLORMAP_NOTIFY:
				xmpl_event_trigger(conn, root, ((xcb_colormap_notify_event_t*)event)->window, "colormap-change", event_dir, env);

				break;
			case XCB_CLIENT_MESSAGE:
				xmpl_event_trigger(conn, root, ((xcb_client_message_event_t*)event)->window, "client-message", event_dir, env);

				break;
			case XCB_MAPPING_NOTIFY:
				xmpl_event_trigger(conn, root, 0, "window-remap", event_dir, env);

				break;
			default:
				sprintf(event_name, "unknow-event-%d", event->response_type);

				xmpl_event_trigger(conn, root, 0, event_name, event_dir, env);
		}

		xmpl_free(event);
	}
}

/**
 * Trigger an Event
 */
bool
xmpl_event_trigger(xcb_connection_t *conn, xcb_window_t root, xcb_window_t win, char *event_name, char *event_dir, char *env)
{
	char *event_path = (char *) malloc(2 + strlen(event_name) + strlen(event_dir? event_dir : "~/.events"));
	char *event_cmd;
	DIR *dir;
	struct stat statbuf;
	struct dirent *entry;

	sprintf(event_path, "%s/%s", event_dir? event_dir : "~/.events", event_name);

	if (!win) {
		printf("event-trigger %s\n", event_name);
	} else if (!xmpl_window_is_ignored(conn, win)) {
		printf("event-trigger %s 0x%08x\n", event_name, win);
	} else {
		printf("event-ignored %s 0x%08x\n", event_name, win);

		return false;
	}

	dir = opendir(event_path);

	if (dir == NULL) {
		if (
			stat(event_path, &statbuf) != 0 ||
			statbuf.st_mode <= 0 ||
			!(S_IXUSR & statbuf.st_mode)
		) {
			return false;
		}

		xmpl_event_spawn(root, win, event_path, 15, env);

		return true;
	}

	while ((entry = readdir(dir)) != NULL) {
		if (!strncmp(entry->d_name, ".", 1)) {
			continue;
		}

		event_cmd = (char *) malloc(2 + strlen(event_path) + strlen(entry->d_name));

		sprintf(event_cmd, "%s/%s", event_path, entry->d_name);

		xmpl_event_spawn(root, win, event_cmd, 5, env);
	}

	closedir(dir);

	return true;
}

/**
 * Event Spawn
 */
void
xmpl_event_spawn(xcb_window_t root, xcb_window_t win, char *cmd_path, int timeout, char *env)
{
	pid_t pid;
	struct stat statbuf;
	char *cmd[] = {
		cmd_path,
		NULL
	};
	int status;

	printf("event-spawn %s 0x%08x\n", cmd_path, win);

	// fork parent to start async process
	if ((pid = fork())) {
		waitpid(-1, &status, WNOHANG);

		return;
	}

	// fork child to orphan the process
	if ((pid = fork())) {
		exit(0);
	}

	// babysitter will put babies to sleep
	if ((pid = fork())) {
		sleep(timeout);
		kill(pid, SIGKILL);
		exit(0);
	}

	// check executability
	if (stat(cmd_path, &statbuf) != 0 || statbuf.st_mode <= 0 || !(S_IXUSR & statbuf.st_mode)) {
		fprintf(stderr, "not executable: %s\n", cmd_path);
		
		exit(1);
	}

	// run program
	execve(cmd_path, cmd, xmpl_event_environment(root, win, env));

	// report error
	fprintf(stderr, "execution error: %s\n", cmd_path);

	exit(1);
}

void
xmpl_pointer_center(xcb_connection_t *conn, xcb_window_t win)
{
	uint32_t values[1];
	xcb_get_geometry_reply_t *geom;

	geom = xmpl_window_geometry(conn, win);

	xcb_warp_pointer(conn, XCB_NONE, win, 0, 0, 0, 0,
			(geom->width + (geom->border_width * 2)) / 2,
			(geom->height + (geom->border_width * 2)) / 2);

	values[0] = XCB_STACK_MODE_ABOVE;

	xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_STACK_MODE, values);

	xmpl_free(geom);
}

void
xmpl_pointer_warp(xcb_connection_t *conn, xcb_window_t win, int x, int y)
{
	xcb_warp_pointer(conn, XCB_NONE, win, 0, 0, 0, 0, x, y);
}
