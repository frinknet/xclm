/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	xcbtools_conn_init(&conn);

	printf("0x%08x\n", xcbtools_win_current(conn));

	xcbtools_conn_kill(&conn);

	return 0;
}
