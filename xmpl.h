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
#include <xcb/xcb_icccm.h>

void xmpl_daemonize(char *out, char *err);

void xmpl_conn_init(xcb_connection_t **conn);
void xmpl_conn_kill(xcb_connection_t **conn);

void xmpl_screen_init(xcb_connection_t *conn, xcb_screen_t **screen);
void xmpl_color_init(xcb_connection_t *conn, xcb_screen_t *screen, xcb_colormap_t *colormap, xcb_visualid_t *visual);

void xmpl_window_border(xcb_connection_t *conn, xcb_window_t win, uint32_t size, uint32_t color);
void xmpl_window_background(xcb_connection_t *conn, xcb_window_t win, uint32_t color);

void xmpl_window_stack(xcb_connection_t *conn, xcb_window_t win, uint32_t stack);
void xmpl_window_ignore(xcb_connection_t *conn, xcb_window_t win, int override);
void xmpl_window_move(xcb_connection_t *conn, xcb_window_t win, int x, int y);
void xmpl_window_resize(xcb_connection_t *conn, xcb_window_t win, int width, int height);
void xmpl_window_warp(xcb_connection_t *conn, xcb_window_t win, int x, int y, int width, int height);

int xmpl_window_exists(xcb_connection_t *conn, xcb_window_t win);
int xmpl_window_mapped(xcb_connection_t *conn, xcb_window_t win);
int xmpl_window_ignored(xcb_connection_t *conn, xcb_window_t win);
int xmpl_window_children(xcb_connection_t *conn, xcb_window_t win, xcb_window_t **list);

xcb_window_t xmpl_window_create(xcb_connection_t *conn, xcb_window_t win, int x, int y, int width, int height);
xcb_window_t xmpl_window_parent(xcb_connection_t *conn, xcb_window_t win);

xcb_get_geometry_reply_t *xmpl_window_geometry(xcb_connection_t *conn, xcb_window_t win);
xcb_get_window_attributes_reply_t *xmpl_window_attributes(xcb_connection_t *conn, xcb_window_t win);

char *xmpl_window_get_atom(xcb_connection_t *conn, xcb_window_t win, char *atom_name);
char *xmpl_window_get_property(xcb_connection_t *conn, xcb_window_t win, xcb_atom_t prop);
void xmpl_window_set_atom(xcb_connection_t *conn, xcb_window_t win, char *atom_name, char *value);
void xmpl_window_set_property(xcb_connection_t *conn, xcb_window_t win, xcb_atom_t prop, char *value);

char *xmpl_window_name(xcb_connection_t *conn, xcb_window_t win);
char *xmpl_window_class(xcb_connection_t *conn, xcb_window_t win);
char *xmpl_window_command(xcb_connection_t *conn, xcb_window_t win);

xcb_window_t xmpl_window_current(xcb_connection_t *conn);

xcb_atom_t xmpl_atom(xcb_connection_t *conn, char *atom_name);

void xmpl_event_register(xcb_connection_t *conn, xcb_window_t win, uint32_t mask);
bool xmpl_event_notify_valid(xcb_connection_t *conn, xcb_generic_event_t* event);
bool xmpl_event_configure_valid(xcb_connection_t *conn, xcb_generic_event_t *event);
void xmpl_event_loop(xcb_connection_t *conn, char *event_dir);
bool xmpl_event_trigger(xcb_connection_t *conn, xcb_window_t win, char *event_name, char *event_dir);
char **xmpl_event_environment(xcb_window_t win);

pid_t xmpl_event_spawn(xcb_window_t win, char *cmd_path, bool spawn);

#endif
