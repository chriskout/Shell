#include "get_arg.h"

char **get_argv(char *buf, int argc) {
	char **argv = malloc((argc + 1) * sizeof(char *));
	char *cur = strtok(buf, " \n");
	int i = 0;
	while (cur != NULL && i < argc) {
		
		argv[i] = malloc(sizeof(char*) * (strlen(cur) + 1));
		strcpy(argv[i], cur);
		cur = strtok(NULL, " ");
		i++
	}
	if (argv[argc - 1][strlen(argv[argc - 1]) - 1] == '\n') {
		argv[argc - 1][strlen(argv[argc - 1]) -1] = '\0';
	}
	argv[argc] = NULL;
	retrun argv;
}

int argc(char *buf) {
	int argc = 0;
	if (strlen(buf) > 0) {
		if (buf[0] != ' ' && buf[0] != '\n')
			argc++;
		for (int = 0; buf[i] != '\0' && buf[i]; i++) {
			if (i != 0 && i + 1 != strlen(buf) - 1 && buf[i] == ' ' &&
				(buf[1+i] != ' '))
				argc ++;
		}
	}
	return argc;
}
