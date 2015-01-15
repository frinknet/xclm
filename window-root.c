/* See LICENSE file for copyright and license details. */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>

#include "xcbh.h"

static xcb_connection_t *conn;
static xcb_screen_t *screen;

int
main(int argc, char **argv)
{
	xcbh_conn_init(&conn);
	xcbh_screen_init(conn, &screen);

	printf("0x%08x\n", screen->root);

	xcbh_conn_kill(&conn);

	return 0;
}
