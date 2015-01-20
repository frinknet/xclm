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

#define XCBTOOLS_SUITE "XCB Tools"
#define XCBTOOLS_VERSION "1.2"
#define XCBTOOLS_COPYRIGHT "2014-2015 FRINKnet and Lemurs"
#define XCBTOOLS_LICENSE "MIT LICENSE: http://lemurs.mit-license.org"

void xcbtools_conn_init(xcb_connection_t **);
void xcbtools_conn_kill(xcb_connection_t **);

void xcbtools_screen_init(xcb_connection_t *, xcb_screen_t **);

void xcbtools_window_border(xcb_connection_t *, xcb_window_t, int, int);
void xcbtools_window_stack(xcb_connection_t *, xcb_window_t, uint32_t);
void xcbtools_window_ignore(xcb_connection_t *, xcb_window_t, int);
void xcbtools_window_move(xcb_connection_t *, xcb_window_t, int, int);
void xcbtools_window_resize(xcb_connection_t *, xcb_window_t, int, int);
void xcbtools_window_warp(xcb_connection_t *, xcb_window_t, int, int, int, int);

int xcbtools_window_exists(xcb_connection_t *, xcb_window_t);
int xcbtools_window_mapped(xcb_connection_t *, xcb_window_t);
int xcbtools_window_ignored(xcb_connection_t *, xcb_window_t);
int xcbtools_window_children(xcb_connection_t *, xcb_window_t, xcb_window_t **);

xcb_window_t xcbtools_window_parent(xcb_connection_t *, xcb_window_t);
xcb_get_geometry_reply_t *xcbtools_window_geometry(xcb_connection_t *, xcb_window_t);
xcb_get_window_attributes_reply_t *xcbtools_window_attributes(xcb_connection_t *, xcb_window_t);

char *xcbtools_window_property(xcb_connection_t *, xcb_window_t, xcb_atom_t);
char *xcbtools_window_name(xcb_connection_t *, xcb_window_t);
char *xcbtools_window_class(xcb_connection_t *, xcb_window_t);
char *xcbtools_window_command(xcb_connection_t *, xcb_window_t);


xcb_window_t xcbtools_window_current();

void xcbtools_event_register(xcb_connection_t *, xcb_window_t, uint32_t);
bool xcbtools_event_notify_valid(xcb_connection_t *, xcb_generic_event_t*);
bool xcbtools_event_configure_valid(xcb_connection_t *, xcb_generic_event_t *);
void xcbtools_event_loop(xcb_connection_t *, char *);
bool xcbtools_event_trigger(xcb_connection_t *, xcb_window_t, char *, char *);
char **xcbtools_event_environment(xcb_window_t);
pid_t xcbtools_event_spawn(xcb_window_t, char *, bool);

void xcbtools_usage_window(char *, char *);
void xcbtools_usage(char *, char *);
void xcbtools_usage_header(char *);
#endif
