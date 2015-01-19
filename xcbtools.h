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
#include <xcb/xcb.h>
#include <xcb/xcb_icccm.h>

#ifndef XCBTOOLS_H__
#define XCBTOOLS_H__

#define XCBTOOLS_SUITE "XCB Tools"
#define XCBTOOLS_VERSION "1.2"
#define XCBTOOLS_COPYRIGHT "2014-2015 FRINKnet and Friends"
#define XCBTOOLS_LICENSE "MIT LICENSE: http://lemurs.mit-license.org"

void xcbtools_conn_init(xcb_connection_t **);
void xcbtools_conn_kill(xcb_connection_t **);

void xcbtools_screen_init(xcb_connection_t *, xcb_screen_t **);

void xcbtools_win_stack(xcb_connection_t *, xcb_window_t, uint32_t);
void xcbtools_win_ignore(xcb_connection_t *, xcb_window_t, int);
void xcbtools_win_move(xcb_connection_t *, xcb_window_t, int, int);
void xcbtools_win_resize(xcb_connection_t *, xcb_window_t, int, int);
void xcbtools_win_warp(xcb_connection_t *, xcb_window_t, int, int, int, int);

int xcbtools_win_exists(xcb_connection_t *, xcb_window_t);
int xcbtools_win_mapped(xcb_connection_t *, xcb_window_t);
int xcbtools_win_ignored(xcb_connection_t *, xcb_window_t);
int xcbtools_win_children(xcb_connection_t *, xcb_window_t, xcb_window_t **);

xcb_window_t xcbtools_win_parent(xcb_connection_t *, xcb_window_t);
xcb_get_geometry_reply_t *xcbtools_win_geometry(xcb_connection_t *, xcb_window_t);
xcb_get_window_attributes_reply_t *xcbtools_win_attributes(xcb_connection_t *, xcb_window_t);

char *xcbtools_win_property(xcb_connection_t *, xcb_window_t, xcb_atom_t);
char *xcbtools_win_name(xcb_connection_t *, xcb_window_t);
char *xcbtools_win_class(xcb_connection_t *, xcb_window_t);
char *xcbtools_win_command(xcb_connection_t *, xcb_window_t);

xcb_window_t xcbtools_win_current();

void xcbtools_event_register(xcb_connection_t *, xcb_window_t, uint32_t);
bool xcbtools_event_notify_valid(xcb_generic_event_t*);
void xcbtools_event_loop(xcb_connection_t *, char *);
bool xcbtools_event_trigger(xcb_connection_t *, xcb_window_t, char *, char *);
pid_t xcbtools_event_spawn(xcb_window_t, char *, char *, char **);

void xcbtools_win_usage(char *, char *);
void xcbtools_usage(char *, char *);
void xcbtools_usage_header(char *);
#endif
