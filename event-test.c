#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	printf("Triggering %s...\n", argv[0]);
	printf("PATH=%s\n", getenv("PATH"));
	printf("EVENTS=%s\n", getenv("EVENTS"));
	printf("COMMANDS=%s\n", getenv("COMMANDS"));
	printf("WINDOW=%s\n\n", getenv("WINDOW"));
}
