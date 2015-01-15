/* See LICENSE file for copyright and license details. */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>

#include "xcbh.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	xcbh_conn_init(&conn);

	printf("0x%08x\n", xcbh_win_current());

	xcbh_conn_kill(&conn);

	return 0;
}
