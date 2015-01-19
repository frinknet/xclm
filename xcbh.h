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

#ifndef XCB_HELPERS_H__
#define XCB_HELPERS_H__

#define XCBH_VERSION "1.2"
#define XCBH_COPYRIGHT "2014-2015 FRINKnet and the Expatriated Lemurs of Borneo"
#define XCBH_LICENSE "MIT LICENSE: http://lemurs.mit-license.org"

void xcbh_conn_init(xcb_connection_t **);
void xcbh_conn_kill(xcb_connection_t **);

void xcbh_screen_init(xcb_connection_t *, xcb_screen_t **);

void xcbh_win_stack(xcb_connection_t *, xcb_window_t, uint32_t);
void xcbh_win_ignore(xcb_connection_t *, xcb_window_t, int);
void xcbh_win_move(xcb_connection_t *, xcb_window_t, int, int);
void xcbh_win_resize(xcb_connection_t *, xcb_window_t, int, int);
void xcbh_win_warp(xcb_connection_t *, xcb_window_t, int, int, int, int);

int xcbh_win_exists(xcb_connection_t *, xcb_window_t);
int xcbh_win_mapped(xcb_connection_t *, xcb_window_t);
int xcbh_win_ignored(xcb_connection_t *, xcb_window_t);
int xcbh_win_children(xcb_connection_t *, xcb_window_t, xcb_window_t **);

xcb_window_t xcbh_win_parent(xcb_connection_t *, xcb_window_t);
xcb_get_geometry_reply_t *xcbh_win_geometry(xcb_connection_t *, xcb_window_t);
xcb_get_window_attributes_reply_t *xcbh_win_attributes(xcb_connection_t *, xcb_window_t);

char *xcbh_win_property(xcb_connection_t *, xcb_window_t, xcb_atom_t);
char *xcbh_win_name(xcb_connection_t *, xcb_window_t);
char *xcbh_win_class(xcb_connection_t *, xcb_window_t);
char *xcbh_win_command(xcb_connection_t *, xcb_window_t);

xcb_window_t xcbh_win_current();

void xcbh_event_register(xcb_connection_t *, xcb_window_t, uint32_t);
bool xcbh_event_notify_valid(xcb_generic_event_t*);
void xcbh_event_loop(xcb_connection_t *, char *);
bool xcbh_event_trigger(xcb_connection_t *, xcb_window_t, char *, char *);
pid_t xcbh_event_spawn(xcb_window_t, char *, char *, char **);

void xcbh_win_usage(char *, char *);
void xcbh_usage(char *, char *);
void xcbh_show_header();
#endif
