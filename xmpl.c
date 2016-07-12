#include "xmpl.h"

void
xmpl_daemonize(char *out, char *err)
{
	int fout, ferr;

	fout = open(out, O_WRONLY);
	ferr = open(err, O_WRONLY);

	fflush(stdout);
	fflush(stderr);

	dup2(fout, 1);
	dup2(ferr, 2);

	close(fout);
	close(ferr);

	if (fork()) {
		exit(0);
	}
}

void
xmpl_conn_init(xcb_connection_t **conn)
{
	*conn = xcb_connect(NULL, NULL);

	if (xcb_connection_has_error(*conn)) {
		errx(1, "unable connect to the X server");
	}
}

void
xmpl_conn_kill(xcb_connection_t **conn)
{
	if (*conn) {
		xcb_flush(*conn);
		xcb_disconnect(*conn);
	}
}

void
xmpl_screen_init(xcb_connection_t *conn, xcb_screen_t **screen)
{
	*screen = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;

	if (*screen == NULL) {
		errx(1, "unable to retrieve screen informations");
	}
}

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

xcb_get_window_attributes_reply_t *
xmpl_window_get_attributes(xcb_connection_t *conn, xcb_window_t win)
{
	xcb_get_window_attributes_cookie_t cookie = xcb_get_window_attributes(conn, win);
	xcb_get_window_attributes_reply_t  *reply = xcb_get_window_attributes_reply(conn, cookie, NULL);

	return reply;
}

void *
xmpl_window_get_atom(xcb_connection_t *conn, xcb_window_t win, char *atom_name, xcb_atom_t type)
{
	xcb_atom_t atom;

	atom = xmpl_atom(conn, atom_name);

	return xmpl_window_get_property(conn, win, atom, type);
}

void
xmpl_window_set_atom(xcb_connection_t *conn, xcb_window_t win, char *atom_name, xcb_atom_t type, void *value)
{
	xcb_atom_t atom;

	atom = xmpl_atom(conn, atom_name);

	xmpl_window_set_property(conn, win, atom, type, value);
}

void *
xmpl_window_get_property(xcb_connection_t *conn, xcb_window_t win, xcb_atom_t prop, xcb_atom_t type)
{
	xcb_get_property_cookie_t cookie;
	xcb_get_property_reply_t *reply;
	void *value;

	cookie = xcb_get_property(conn, 0, win, prop, type, 0L, UINT_MAX);
	reply = xcb_get_property_reply(conn, cookie, NULL);
	value = xcb_get_property_value(reply);

	//free(reply);

	return value;
}

void
xmpl_window_set_property(xcb_connection_t *conn, xcb_window_t win, xcb_atom_t prop, xcb_atom_t type, void *value)
{
	xcb_change_property(conn, XCB_PROP_MODE_REPLACE, win, prop, type, 8, strlen(value), value);
}

char *
xmpl_window_get_name(xcb_connection_t *conn, xcb_window_t win)
{
	return xmpl_window_get_property(conn, win, XCB_ATOM_WM_NAME, XCB_ATOM_STRING);
}

char *
xmpl_window_get_class(xcb_connection_t *conn, xcb_window_t win)
{
	return xmpl_window_get_property(conn, win, XCB_ATOM_WM_CLASS, XCB_ATOM_STRING);
}

char *
xmpl_window_get_command(xcb_connection_t *conn, xcb_window_t win)
{
	return xmpl_window_get_property(conn, win, XCB_ATOM_WM_COMMAND, XCB_ATOM_STRING);
}

char *
xmpl_window_get_type(xcb_connection_t *conn, xcb_window_t win)
{
	xcb_atom_t *atom;

	atom = (xcb_atom_t *) xmpl_window_get_property(conn, win, xmpl_atom(conn, "_NET_WM_WINDOW_TYPE"), XCB_ATOM_ATOM);

	return xmpl_atom_name(conn, *atom);
}

void
xmpl_window_set_ignore(xcb_connection_t *conn, xcb_window_t win, int override)
{
	xcb_change_window_attributes(conn, win, XCB_CW_OVERRIDE_REDIRECT, (uint32_t[]){ override });
	xcb_flush(conn);
}

void
xmpl_window_set_background(xcb_connection_t *conn, xcb_window_t win, uint32_t color)
{
	xcb_change_window_attributes(conn, win, XCB_CW_BACK_PIXEL, (uint32_t[]){ color });

	xcb_get_geometry_reply_t *geom = xmpl_window_get_geometry(conn, win);
	xcb_clear_area(conn, 1, win, 0, 0, geom->width, geom->height);

	xcb_flush(conn);
}

void
xmpl_window_set_border(xcb_connection_t *conn, xcb_window_t win, uint32_t size, uint32_t color)
{
	xcb_change_window_attributes(conn, win, XCB_CW_BORDER_PIXEL, (uint32_t[]){ color });
	xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_BORDER_WIDTH, (uint32_t[]){ size });

	xcb_flush(conn);
}

void
xmpl_window_set_stack(xcb_connection_t *conn, xcb_window_t win, uint32_t stack)
{
	xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_STACK_MODE, (uint32_t[]){ stack });
	xcb_flush(conn);
}

void
xmpl_window_set_size(xcb_connection_t *conn, xcb_window_t win, int width, int height)
{
	xcb_configure_window(
		conn, win,
		XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
		(uint32_t[]){ width, height }
	);

	xcb_flush(conn);
}

void
xmpl_window_set_position(xcb_connection_t *conn, xcb_window_t win, int x, int y)
{
	xcb_configure_window(
		conn, win,
		XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
		(uint32_t[]){ x, y }
	);

	xcb_flush(conn);
}

void
xmpl_window_set_geometry(xcb_connection_t *conn, xcb_window_t win, int x, int y, int width, int height)
{
	xcb_configure_window(
		conn, win,
			XCB_CONFIG_WINDOW_X |
			XCB_CONFIG_WINDOW_Y |
			XCB_CONFIG_WINDOW_WIDTH |
			XCB_CONFIG_WINDOW_HEIGHT,
		(uint32_t[]){ x, y, width, height }
	);

	xcb_flush(conn);
}

xcb_get_geometry_reply_t *
xmpl_window_get_geometry(xcb_connection_t *conn, xcb_window_t win)
{
	xcb_get_geometry_cookie_t cookie;
	xcb_get_geometry_reply_t *reply;

	cookie = xcb_get_geometry(conn, win);
	reply = xcb_get_geometry_reply(conn, cookie, NULL);

	if (!reply) {
		errx(1, "center_pointer: missing geometry!");
	}

	return reply;
}

int
xmpl_window_is_valid(xcb_connection_t *conn, xcb_window_t win)
{
	xcb_get_window_attributes_reply_t  *attr;

	attr = xmpl_window_get_attributes(conn, win);

	if (attr == NULL) {
		return 0;
	}

	free(attr);

	return 1;
}

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

	free(attr);

	return mapped == XCB_MAP_STATE_VIEWABLE;
}

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

	free(attr);

	return override;
}

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

	free(reply);

	return childnum;
}

xcb_window_t
xmpl_window_get_parent(xcb_connection_t *conn, xcb_window_t win)
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

	free(reply);

	return parent;
}

xcb_window_t
xmpl_window_create(
	xcb_connection_t *conn,
	xcb_window_t parent,
	int x,
	int y,
	int width,
	int height
)
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
		x, y, width, height, 0,
		XCB_WINDOW_CLASS_INPUT_OUTPUT,
		visual,
			XCB_CW_BORDER_PIXEL |
			XCB_CW_COLORMAP,
		(unsigned int[]) {
			0,
			colormap
		});

	xcb_flush(conn);

	return win;
}

xcb_window_t
xmpl_window_get_current(xcb_connection_t *conn)
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

	free(reply);

	return win;
}

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
	  return XCB_NONE;
	}

	atom = reply->atom;

	free(reply);

	return atom;
}

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

	free(reply);

	return atom_name;
}


void
xmpl_event_register(xcb_connection_t *conn, xcb_window_t win, uint32_t type)
{
	uint32_t values[] = { type };
	uint32_t mask = XCB_CW_EVENT_MASK;

	xcb_change_window_attributes(conn, win, mask, values);
	xcb_flush(conn);
}

bool
xmpl_event_configure_valid(xcb_connection_t *conn, xcb_generic_event_t *event)
{
	xcb_configure_notify_event_t *config;
	xcb_screen_t *screen;

	xmpl_screen_init(conn, &screen);

	config = (xcb_configure_notify_event_t*)event;

	return (config->window == screen->root);
}

bool
xmpl_event_notify_valid(xcb_connection_t *conn, xcb_generic_event_t *event)
{
	xcb_enter_notify_event_t *notify;

	notify = (xcb_enter_notify_event_t*)event;

	switch (notify->mode) {
	case XCB_NOTIFY_MODE_NORMAL:
	case XCB_NOTIFY_MODE_UNGRAB:
		if (notify->detail == 0) {
			return true;
		}
	}

	return false;
}

void
xmpl_event_loop(xcb_connection_t *conn, char *event_dir)
{
	bool running = true;
	char *event_name = (char *) malloc(32);
	xcb_generic_event_t *event;


	while (running) {
		event = xcb_wait_for_event(conn);

		switch (event->response_type & ~0x80) {
			case 0:
				break;
			case XCB_KEY_PRESS:
				xmpl_event_trigger(conn, ((xcb_key_press_event_t*)event)->event, "key-down", event_dir);

				break;
			case XCB_KEY_RELEASE:
				xmpl_event_trigger(conn, ((xcb_key_release_event_t*)event)->event, "key-up", event_dir);

				break;
			case XCB_BUTTON_PRESS:
				xmpl_event_trigger(conn, ((xcb_button_press_event_t*)event)->event, "mouse-down", event_dir);

				break;
			case XCB_BUTTON_RELEASE:
				xmpl_event_trigger(conn, ((xcb_button_release_event_t*)event)->event, "mouse-up", event_dir);

				break;
			case XCB_MOTION_NOTIFY:
				if (xmpl_event_notify_valid(conn, event)) {
					xmpl_event_trigger(conn, ((xcb_motion_notify_event_t*)event)->event, "mouse-move", event_dir);
				}

				break;
			case XCB_ENTER_NOTIFY:
				if (xmpl_event_notify_valid(conn, event)) {
					xmpl_event_trigger(conn, ((xcb_enter_notify_event_t*)event)->event, "mouse-in", event_dir);
				}

				break;
			case XCB_LEAVE_NOTIFY:
				if (xmpl_event_notify_valid(conn, event)) {
					xmpl_event_trigger(conn, ((xcb_enter_notify_event_t*)event)->event, "mouse-out", event_dir);
				}

				break;
			case XCB_FOCUS_IN:
				xmpl_event_trigger(conn, ((xcb_focus_in_event_t*)event)->event, "window-focus", event_dir);

				break;
			case XCB_FOCUS_OUT:
				xmpl_event_trigger(conn, ((xcb_focus_in_event_t*)event)->event, "window-blur", event_dir);

				break;
			case XCB_KEYMAP_NOTIFY:
				xmpl_event_trigger(conn, ((xcb_button_release_event_t*)event)->event, "keymap-notify", event_dir);

				break;
			case XCB_EXPOSE:
				xmpl_event_trigger(conn, ((xcb_expose_event_t*)event)->window, "window-expose", event_dir);

				break;
			case XCB_GRAPHICS_EXPOSURE:
				xmpl_event_trigger(conn, 0, "graphics-exposure", event_dir);

				break;
			case XCB_NO_EXPOSURE:
				xmpl_event_trigger(conn, 0, "no-exposure", event_dir);

				break;
			case XCB_VISIBILITY_NOTIFY:
				xmpl_event_trigger(conn, ((xcb_visibility_notify_event_t*)event)->window, "window-visibility", event_dir);

				break;
			case XCB_CREATE_NOTIFY:
				xmpl_event_trigger(conn, ((xcb_create_notify_event_t*)event)->window, "window-create", event_dir);

				xmpl_event_register(conn, ((xcb_create_notify_event_t*)event)->window, 0);

				break;
			case XCB_DESTROY_NOTIFY:
				xmpl_event_trigger(conn, ((xcb_create_notify_event_t*)event)->window, "window-destroy", event_dir);

				break;
			case XCB_UNMAP_NOTIFY:
				if (xmpl_event_configure_valid(conn, event)) {
					xmpl_event_trigger(conn, ((xcb_unmap_notify_event_t*)event)->window, "window-hide", event_dir);
				}

				break;
			case XCB_MAP_NOTIFY:
				if (xmpl_event_configure_valid(conn, event)) {
					xmpl_event_trigger(conn, ((xcb_map_notify_event_t*)event)->window, "window-show", event_dir);
				}

				break;
			case XCB_MAP_REQUEST:
				xmpl_event_trigger(conn, ((xcb_map_request_event_t*)event)->window, "map-request", event_dir);

				break;
			case XCB_REPARENT_NOTIFY:
				xmpl_event_trigger(conn, ((xcb_reparent_notify_event_t*)event)->window, "window-reparent", event_dir);

				break;
			case XCB_CONFIGURE_NOTIFY:
				if (xmpl_event_configure_valid(conn, event)) {
					xmpl_event_trigger(conn, ((xcb_configure_notify_event_t*)event)->window, "config-finish", event_dir);
				}

				break;
			case XCB_CONFIGURE_REQUEST:
				if (xmpl_event_configure_valid(conn, event)) {
					xmpl_event_trigger(conn, ((xcb_configure_request_event_t*)event)->window, "config-begin", event_dir);
				}

				break;
			case XCB_GRAVITY_NOTIFY:
				xmpl_event_trigger(conn, ((xcb_gravity_notify_event_t*)event)->window, "gravity", event_dir);

				break;
			case XCB_RESIZE_REQUEST:
				xmpl_event_trigger(conn, ((xcb_resize_request_event_t*)event)->window, "window-resize", event_dir);

				break;
			case XCB_CIRCULATE_NOTIFY:
				xmpl_event_trigger(conn, ((xcb_circulate_notify_event_t*)event)->window, "restack-finish", event_dir);

				break;
			case XCB_CIRCULATE_REQUEST:
				xmpl_event_trigger(conn, ((xcb_circulate_request_event_t*)event)->window, "restack-start", event_dir);

				break;
			case XCB_PROPERTY_NOTIFY:
				xmpl_event_trigger(conn, ((xcb_property_notify_event_t*)event)->window, "property-change", event_dir);

				break;
			case XCB_SELECTION_CLEAR:
				xmpl_event_trigger(conn, ((xcb_selection_clear_event_t*)event)->owner, "select-clear", event_dir);

				break;
			case XCB_SELECTION_REQUEST:
				xmpl_event_trigger(conn, ((xcb_selection_request_event_t*)event)->owner, "select-start", event_dir);

				break;
			case XCB_SELECTION_NOTIFY:
				xmpl_event_trigger(conn, ((xcb_selection_notify_event_t*)event)->requestor, "select-end", event_dir);

				break;
			case XCB_COLORMAP_NOTIFY:
				xmpl_event_trigger(conn, ((xcb_colormap_notify_event_t*)event)->window, "colormap-change", event_dir);

				break;
			case XCB_CLIENT_MESSAGE:
				xmpl_event_trigger(conn, ((xcb_client_message_event_t*)event)->window, "client-message", event_dir);

				break;
			case XCB_MAPPING_NOTIFY:
				xmpl_event_trigger(conn, 0, "keymap-change", event_dir);

				break;
			default:
				sprintf(event_name, "unknow-event-%d", event->response_type);

				xmpl_event_trigger(conn, 0, event_name, event_dir);
		}
	}
}

bool
xmpl_event_trigger(xcb_connection_t *conn, xcb_window_t win, char *event_name, char *event_dir)
{
	char *event_path = (char *) malloc(1 + strlen(event_name) + strlen(event_dir? event_dir : "~/.events"));
	char *event_cmd;
	DIR *dir;
	//struct dirent prev;
	struct dirent *entry;

	sprintf(event_path, "%s/%s", event_dir? event_dir : "~/.events", event_name);

	if (!win) {
		printf("event-trigger %s\n", event_name);
	} else if (!xmpl_window_is_ignored(conn, win)) {
		printf("event-trigger %s 0x%08x\n", event_name, win);
	} else {
		return false;
	}

	fflush(stdout);

	dir = opendir(event_path);

	if (dir == NULL) {
		//fprintf(stderr, "failed dir: %s\n", event_path);

		return false;
	}

	while ((entry = readdir(dir)) != NULL) {
		if (
			(strcmp(entry->d_name, ".") == 0) ||
			(strcmp(entry->d_name, "..") == 0)
		) {
			continue;
		}

		event_cmd = (char *) malloc(2 + strlen(event_path) + strlen(entry->d_name));

		sprintf(event_cmd, "%s/%s", event_path, entry->d_name);

		xmpl_event_spawn(win, event_cmd, true);
	}

	closedir(dir);

	return true;
}

char **
xmpl_event_environment(xcb_window_t win)
{
	char *events_dir = getenv("EVENTS");
	char *path_dir = getenv("PATH");
	char *cmd_dir = getenv("COMMANDS");
	char *env[5];


	env[0] = (char *) malloc(32);

	if (win) {
		sprintf(env[0], "WINDOW=0x%08x", win);
	} else {
		sprintf(env[0], "WINDOW=");
	}

	if (!events_dir) {
		events_dir = (char *) malloc(10);
		events_dir = "~/.events";
	}

	env[1] = (char *) malloc(8 + strlen(events_dir));

	sprintf(env[1], "EVENTS=%s", events_dir);

	if (!cmd_dir) {
		cmd_dir = (char *) malloc(2);
		cmd_dir = ".";
	}

	env[2] = (char *) malloc(10 + strlen(cmd_dir));

	sprintf(env[2], "COMMANDS=%s", cmd_dir);

	if (!path_dir) {
		path_dir = (char *) malloc(2);
		path_dir = ".";
	}

	env[3] = (char *) malloc(6 + strlen(cmd_dir) + strlen(path_dir));

	sprintf(env[3], "PATH=%s:%s", path_dir, cmd_dir);

	env[4] = NULL;

	char **ret = malloc(sizeof(env));

	memcpy(ret, env, sizeof(env));

	return ret;
}

pid_t
xmpl_event_spawn(xcb_window_t win, char *cmd_path, bool spawn)
{
	pid_t pid;
	struct stat statbuf;
	char *cmd[] = {
		cmd_path,
		NULL
	};
	int fd;


	switch (pid = spawn? fork() : 0) {
		case -1:
			fprintf(stderr, "failed to fork properly\n");
			exit(1);

			break;
		case 0:
			fd = open("/dev/null", O_WRONLY);

			dup2(fd, 1);
			close(fd);

			break;
		default:
			return pid;
	}

	if (
		stat(cmd_path, &statbuf) != 0 ||
		statbuf.st_mode <= 0 ||
		!(S_IXUSR & statbuf.st_mode)
	) {
		fprintf(stderr, "failed stat: %s\n", cmd_path);

		return 0;
	}

	execve(cmd_path, cmd, xmpl_event_environment(win));
	fprintf(stderr, "failed exec: %s\n", cmd_path);

	exit(1);
}

/*
void
xmpl_mouse_center_window(xcb_connection_t conn, xcb_window_t win)
{
	uint32_t values[1];
	xcb_get_geometry_reply_t *geom;

	geom = xmpl_window_get_geometry(conn, win);

	xcb_warp_pointer(conn, XCB_NONE, win, 0, 0, 0, 0,
			(geom->width  + (geom->border_width * 2)) / 2,
			(geom->height + (geom->border_width * 2)) / 2);

	values[0] = XCB_STACK_MODE_ABOVE;

	xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_STACK_MODE, values);

	free(geom);
}
*/