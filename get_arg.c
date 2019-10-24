#include "get_arg.h"
//input: buf, the given buffer
//	argc, the amount of arguments. Found by calling get_argc earlier
//output: a list of arguments
char **get_argv(char *buf, int argc) {
	char **argv = malloc((argc + 1) * sizeof(char *));
	char *cur = strtok(buf, " \n");
	int x = 0;
	while (cur != NULL && x < argc) {
		
		argv[x] = malloc(sizeof(char*) * (strlen(cur) + 1));
		strcpy(argv[x], cur);
		cur = strtok(NULL, " ");
		x++;
	}
	if (argv[argc - 1][strlen(argv[argc - 1]) - 1] == '\n') {
		argv[argc - 1][strlen(argv[argc - 1]) -1] = '\0';
	}
	argv[argc] = NULL;
	return argv;
}
//input:  buf, the buffer. Expect a bunch of space seperated strings
//output: the number of commands in the buffer
int get_argc(char *buf) {
	int argc = 0;
	if (strlen(buf) != 0) {
		if (buf[0] != ' ' && buf[0] != '\n')
			argc++;
		for (int i = 0; buf[i] != '\0' && buf[i]; i++) {
			if (i != 0 && i + 1 != strlen(buf) - 1 && buf[i] == ' ' &&
				(buf[1+i] != ' '))
				argc ++;
		}
	}
	return argc;
}
