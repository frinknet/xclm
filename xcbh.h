#ifndef XCB_HELPERS_H__
#define XCB_HELPERS_H__

void xcbh_conn_init(xcb_connection_t **);
void xcbh_conn_kill(xcb_connection_t **);

void xcbh_screen_init(xcb_connection_t *, xcb_screen_t **);

int xcbh_win_exists(xcb_connection_t *, xcb_window_t);
int xcbh_win_mapped(xcb_connection_t *, xcb_window_t);
int xcbh_win_ignored(xcb_connection_t *, xcb_window_t);

void xcbh_win_ignore(xcb_connection_t *, xcb_window_t, int);
void xcbh_win_move(xcb_connection_t *, xcb_window_t, int, int);
void xcbh_win_resize(xcb_connection_t *, xcb_window_t, int, int);
void xcbh_win_redraw(xcb_connection_t *, xcb_window_t, int, int, int, int);

xcb_get_geometry_reply_t *xcbh_win_geometry(xcb_connection_t *, xcb_window_t);
int xcbh_win_children(xcb_connection_t *, xcb_window_t, xcb_window_t **);

char *xcbh_win_title(xcb_connection_t *, xcb_window_t);

xcb_window_t xcbh_win_current();

void xcbh_win_usage(char *, char *);
#endif
