/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

int
main(int argc, char **argv)
{
	printf("Triggering %s...\n", argv[0]);
	printf("PATH=%s\n", getenv("PATH"));
	printf("EVENTS=%s\n", getenv("EVENTS"));
	printf("COMMANDS=%s\n", getenv("COMMANDS"));
	printf("WINDOW=%s\n\n", getenv("WINDOW"));
}
