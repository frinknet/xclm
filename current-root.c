/* SEE LICENSE */

#include "xcmd.h"

xcmd_call (1, "") {
	xcb_screen_t *screen;

	xcbtools_screen_init(xcmd_conn, &screen);

	printf("0x%08x\n", screen->root);

	xcmd_exit(0);
}
