#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	printf("EVENT=%s\n", argv[1]);
	printf("PATH=%s\n", getenv("PATH"));
	printf("EVENTS=%s\n", getenv("EVENTS"));
	printf("COMMANDS=%s\n", getenv("COMMANDS"));
	printf("WINDOW=%s\n\n", getenv("WINDOW"));
}
