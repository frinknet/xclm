/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <string.h>
#include <err.h>

#include "xcbh.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	xcb_window_t win;
	char *command = "";

	if (argc < 3) {
		xcbh_win_usage(argv[0], "command");
	}

	xcbh_conn_init(&conn);

	;

	command = malloc(strlen(*argv++));

	memcpy(command, *argv, strlen(*argv));

	while (*++argv) {
		win = strtoul(*argv, NULL, 16);

		if (strcmp(command, xcbh_win_command(conn, win))) {
			xcbh_conn_kill(&conn);

			return 1;
		}
	}

	xcbh_conn_kill(&conn);

	return 0;
}
