#ifndef XCLM_H__
#define XCLM_H__

#define XCLM_SUITE "XCLM!"
#define XCLM_VERSION "1.4"
#define XCLM_COPYRIGHT "2014-2016 FRINKnet and Lemurs"
#define XCLM_LICENSE "MIT LICENSE: http://lemurs.mit-license.org"

void xclm_usage_window(char *name, char *params);
void xclm_usage(char *name, char *params);
void xclm_usage_header(char *name);

void
xclm_usage(char *name, char *params)
{
	xclm_usage_header(basename(name));
	fprintf(stderr, "Usage:\n\t%s %s\n\n", basename(name), params);
	exit(1);
}

void
xclm_usage_window(char *name, char *params)
{
	char *win_params = (char *) malloc(16 + strlen(params));

	strcpy(win_params, params);

	if (strlen(params)) {
		strcat(win_params, " ");
	}

	strcat(win_params, "wid [..wid]");

	xclm_usage(name, win_params);
}

void
xclm_usage_header(char *name)
{
	fprintf(stderr, "[%s] - %s Version %s\n", name, XCLM_SUITE, XCLM_VERSION);
	fprintf(stderr, "Copyright (c) %s\n", XCLM_COPYRIGHT);
	fprintf(stderr, "%s\n\n", XCLM_LICENSE);
}

#endif
