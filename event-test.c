/* See LICENSE file for copyright and license details. */

#include "xcbtools.h"

int
main(int argc, char **argv)
{
	fprintf(stderr, "Triggering %s...\n", argv[0]);
	fprintf(stdout, "PATH=%s\n", getenv("PATH"));
	fprintf(stdout, "EVENTS=%s\n", getenv("EVENTS"));
	fprintf(stdout, "COMMANDS=%s\n", getenv("COMMANDS"));
	fprintf(stdout, "WINDOW=%s\n\n", getenv("WINDOW"));
}
