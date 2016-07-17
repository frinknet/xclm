#ifndef XMPL_H__
#define XMPL_H__

#define _XOPEN_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <libgen.h>
#include <limits.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <xcb/xcb.h>
//#include <xcb/xcb_icccm.h>

int xmpl_daemonize(char *out, char *err);

void xmpl_conn_init(xcb_connection_t **conn);
void xmpl_conn_kill(xcb_connection_t **conn);
void xmpl_conn_sync(xcb_connection_t *conn);

void xmpl_screen_init(xcb_connection_t *conn, xcb_screen_t **screen);
void xmpl_color_init(xcb_connection_t *conn, xcb_screen_t *screen, xcb_colormap_t *colormap, xcb_visualid_t *visual);

void xmpl_window_set_border(xcb_connection_t *conn, xcb_window_t win, uint32_t size, uint32_t color);
void xmpl_window_set_background(xcb_connection_t *conn, xcb_window_t win, uint32_t color);

void xmpl_window_set_stack(xcb_connection_t *conn, xcb_window_t win, uint32_t stack);
void xmpl_window_set_ignore(xcb_connection_t *conn, xcb_window_t win, int override);
void xmpl_window_set_position(xcb_connection_t *conn, xcb_window_t win, int x, int y);
void xmpl_window_set_size(xcb_connection_t *conn, xcb_window_t win, int width, int height);
void xmpl_window_set_geometry(xcb_connection_t *conn, xcb_window_t win, int x, int y, int width, int height);

int xmpl_window_is_valid(xcb_connection_t *conn, xcb_window_t win);
int xmpl_window_is_mapped(xcb_connection_t *conn, xcb_window_t win);
int xmpl_window_is_ignored(xcb_connection_t *conn, xcb_window_t win);

int xmpl_window_list_children(xcb_connection_t *conn, xcb_window_t win, xcb_window_t **list);

xcb_window_t xmpl_window_create(xcb_connection_t *conn, xcb_window_t win, int x, int y, int width, int height, char *cls);
xcb_window_t xmpl_window_get_parent(xcb_connection_t *conn, xcb_window_t win);

xcb_get_geometry_reply_t *xmpl_window_get_geometry(xcb_connection_t *conn, xcb_window_t win);
xcb_get_window_attributes_reply_t *xmpl_window_get_attributes(xcb_connection_t *conn, xcb_window_t win);

void *xmpl_window_get_atom(xcb_connection_t *conn, xcb_window_t win, char *atom_name, xcb_atom_t type);
void *xmpl_window_get_property(xcb_connection_t *conn, xcb_window_t win, xcb_atom_t prop, xcb_atom_t type);
void xmpl_window_set_atom(xcb_connection_t *conn, xcb_window_t win, char *atom_name, xcb_atom_t type, void *value);
void xmpl_window_set_property(xcb_connection_t *conn, xcb_window_t win, xcb_atom_t prop, xcb_atom_t type, void *value);

char *xmpl_window_get_name(xcb_connection_t *conn, xcb_window_t win);
char *xmpl_window_get_class(xcb_connection_t *conn, xcb_window_t win);
char *xmpl_window_get_command(xcb_connection_t *conn, xcb_window_t win);
char *xmpl_window_get_type(xcb_connection_t *conn, xcb_window_t win);

xcb_window_t xmpl_window_get_current(xcb_connection_t *conn);

xcb_atom_t xmpl_atom(xcb_connection_t *conn, char *atom_name);
char *xmpl_atom_name(xcb_connection_t *conn, xcb_atom_t atom_name);

void xmpl_event_register(xcb_connection_t *conn, xcb_window_t win, uint32_t mask);
bool xmpl_event_notify_valid(xcb_connection_t *conn, xcb_generic_event_t* event);
bool xmpl_event_configure_valid(xcb_connection_t *conn, xcb_generic_event_t *event);
void xmpl_event_loop(xcb_connection_t *conn, xcb_window_t root, char *event_dir);
bool xmpl_event_trigger(xcb_connection_t *conn, xcb_window_t root, xcb_window_t win, char *event_name, char *event_dir);
pid_t xmpl_event_spawn(xcb_window_t root, xcb_window_t win, char *cmd_path, bool spawn);
char **xmpl_event_env(xcb_window_t root, xcb_window_t win);

#endif
