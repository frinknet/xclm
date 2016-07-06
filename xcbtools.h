#define _XOPEN_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <libgen.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <xcb/xcb.h>
#include <xcb/xcb_icccm.h>

#ifndef XCBTOOLS_H__
#define XCBTOOLS_H__

#define XCBTOOLS_SUITE "XCMD"
#define XCBTOOLS_VERSION "1.4"
#define XCBTOOLS_COPYRIGHT "2014-2015 FRINKnet and Lemurs"
#define XCBTOOLS_LICENSE "MIT LICENSE: http://lemurs.mit-license.org"

void xcbtools_daemonize(char *out, char *err);

void xcbtools_conn_init(xcb_connection_t **conn);
void xcbtools_conn_kill(xcb_connection_t **conn);

void xcbtools_screen_init(xcb_connection_t *conn, xcb_screen_t **screen);
void xcbtools_color_init(xcb_connection_t *conn, xcb_screen_t *screen, xcb_colormap_t *colormap, xcb_visualid_t *visual);

void xcbtools_window_border(xcb_connection_t *conn, xcb_window_t win, uint32_t size, uint32_t color);
void xcbtools_window_background(xcb_connection_t *conn, xcb_window_t win, uint32_t color);

void xcbtools_window_stack(xcb_connection_t *conn, xcb_window_t win, uint32_t stack);
void xcbtools_window_ignore(xcb_connection_t *conn, xcb_window_t win, int override);
void xcbtools_window_move(xcb_connection_t *conn, xcb_window_t win, int x, int y);
void xcbtools_window_resize(xcb_connection_t *conn, xcb_window_t win, int width, int height);
void xcbtools_window_warp(xcb_connection_t *conn, xcb_window_t win, int x, int y, int width, int height);

int xcbtools_window_exists(xcb_connection_t *conn, xcb_window_t win);
int xcbtools_window_mapped(xcb_connection_t *conn, xcb_window_t win);
int xcbtools_window_ignored(xcb_connection_t *conn, xcb_window_t win);
int xcbtools_window_children(xcb_connection_t *conn, xcb_window_t win, xcb_window_t **list);

xcb_window_t xcbtools_window_create(xcb_connection_t *conn, xcb_window_t win, int x, int y, int width, int height);
xcb_window_t xcbtools_window_parent(xcb_connection_t *conn, xcb_window_t win);

xcb_get_geometry_reply_t *xcbtools_window_geometry(xcb_connection_t *conn, xcb_window_t win);
xcb_get_window_attributes_reply_t *xcbtools_window_attributes(xcb_connection_t *conn, xcb_window_t win);

char *xcbtools_window_property(xcb_connection_t *conn, xcb_window_t win, xcb_atom_t prop);
char *xcbtools_window_atom(xcb_connection_t *conn, xcb_window_t win, char *atom_name);

char *xcbtools_window_name(xcb_connection_t *conn, xcb_window_t win);
char *xcbtools_window_class(xcb_connection_t *conn, xcb_window_t win);
char *xcbtools_window_command(xcb_connection_t *conn, xcb_window_t win);

xcb_window_t xcbtools_window_current(xcb_connection_t *conn);

xcb_atom_t xcbtools_atom(xcb_connection_t *conn, char *atom_name);

void xcbtools_event_register(xcb_connection_t *conn, xcb_window_t win, uint32_t mask);
bool xcbtools_event_notify_valid(xcb_connection_t *conn, xcb_generic_event_t* event);
bool xcbtools_event_configure_valid(xcb_connection_t *conn, xcb_generic_event_t *event);
void xcbtools_event_loop(xcb_connection_t *conn, char *event_dir);
bool xcbtools_event_trigger(xcb_connection_t *conn, xcb_window_t win, char *event_name, char *event_dir);
char **xcbtools_event_environment(xcb_window_t win);

pid_t xcbtools_event_spawn(xcb_window_t win, char *cmd_path, bool spawn);

void xcbtools_usage_window(char *name, char *params);
void xcbtools_usage(char *name, char *params);
void xcbtools_usage_header(char *name);
#endif
