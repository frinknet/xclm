/* SEE LICENSE */

#include "xcmd.h"

xcmd_call (1, "") {
	printf("0x%08x\n", xmpl_window_get_current(xcmd_conn));

	xcmd_exit(0);
}
