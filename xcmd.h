// X Command Management Directives to simplify xmpl commands

#include "xmpl.h"
#include "xclm.h"

#define xcmd_call(expects, description) static xcb_connection_t *xcmd_conn;int main(int argc, char **argv){if(argc < expects){xclm_usage(argv[0], description);}xmpl_conn_init(&xcmd_conn);
#define xcmd_windows(expects, description) static xcb_connection_t *xcmd_conn; int main(int argc, char **argv){if(argc < expects){xclm_usage_window(argv[0], description);}xmpl_conn_init(&xcmd_conn);
#define xcmd_simple xcmd_windows(2, "")
#define xcmd_return(r) xmpl_conn_kill(&xcmd_conn);return r
#define xcmd_exit(r) xcmd_return(r);}
#define xcmd_args argc
#define xcmd_prop *argv
#define xcmd_next *++argv
#define xcmd_loop while(*++argv)
#define xcmd_win_loop xcb_window_t xcmd_win;while(*++argv && (xcmd_win = strtoul(xcmd_prop, NULL, 16)))
#define xcmd_win_exec(call) xcmd_win_loop call; xcmd_exit(0)

/** Call without window IDs

xcmd_call(2, "");
xcmd_exit(0);

*/

/** Call with Window IDs
xcmd_windows (2, "") {
	xcb_window_t win;

	xcmd_loop {
		win = strtoul(xcmd_prop, NULL, 16);
	
		printf("%s\n", xmpl_window_name(xcmd_conn, win));
	}

	xcmd_exit(0);
}
*/

/** Call with Window IDs
xcmd_simple {
	xcmd_win_exec(printf("%s\n", xmpl_window_name(xcmd_conn, xcmd_win)));
}
*/

