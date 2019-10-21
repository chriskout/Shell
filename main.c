//main for my c shell
//created by christopher outhwaite
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "get_path.h"
#include "get_arg.h"

#define BUFFSIZE 128

char buf[BUFFSIZE];
char ** args;
int num_args;
pid_t pid;
char *prompt;
path_t * path;
char *prompt_base;


int sigignore(int sig);

void handle_child(int sig) {
	int saved_errno = errno;
	errno = saved_errno;
}
void handle_sigint(int sig) {
	sigignore(SIGTERM);
	sigignore(SIGTSTP);
}

void termsig(int sig) {}

int main(void) {
	signal(SIGINT, termsig);
	signal(SIGTERM, handle_sigint);
	signal(SIGSTEP, handle_sigint);
	int nc = REDIR_OW;
	
	path = get_path;
	prompt = getcwd(NULL, 0);
	prompt_base = getcwd(NULL, 0);

	int called_yet = 1;
	printf("%s> ", prompt);
	

	while (fgets(buf, BUFFSIZE, stdin) != NULL) {
		while (waitpid((pid_t)(-1), 0, WNOHANG) > 0) {
		}
	num_args = get_argc(buf);
	if (numargs == 0) {
		printf("%s> ", prompt);
		break;
	}
	
	args = get_argv(buf, num_args);
	if (strcmp (args[0], "which") == 0) {
		if (numargs == 0) {
			printf("Need more arguments");
		}
		else {
		
		}
		
		print("");
	}
	else if (strcmp (args[0], "where") == 0) {
		print("");
	}
	else if(strcmp (args[0], "pwd")) == 0 { //prints working directory
		ptr = getcwd(NULL. 0);
		printf("CWD = %s\n", ptr);
		free(ptr);
	}
	
	else if(strcmp(args[0], "prompt") == 0) {
		if (num_args == 1) {
			printf("to do, make this work");
		}
		else if(num_args == 2) {
			//append arg[1] onto the front of prompt;
			char * to_add = argv[2];
			int len = strlen(prompt_base);
			strncat(to_add, prompt_base, len);
			prompt = to_add;
		}
		else {
			printf("wrong amount of args");
		}
	else if(strcmp(args[0], "cd") == 0) {
		if (num_args == 1) {
			char *home = getenv("HOME");
			chdir(home);
		}
		else if (num_args > 2) { 
			printf("too many args);
		}
		else {
			chdir(args[1]);
		}


	}
	else if(strcmp(arg[0], "pid") == 0) {
		int pid_ans = get_pid();
		printf("PID = %d\n"", pid_ans);
	}
	
	else if (strcmp(args[0], "kill") == 0) {
		if (num_args < 2) {
			printf("need more aguments");
		else if (num_args == 2) {
			kill(atoi(args[1]), SIGTERM);
		}
		else if (num_args == 3) {
			char * signal = malloc(sizeof(char) * strlen(args[1])));
			strcpy(signal, &args[1][1]);
			kill(atoi(args[2]), atoi(signal));
			free(signal);
		}
		
	}
	else if (strcmp(args[0], "exit") == 0) {
		free(prompt);
		free_path(path);
		for int i = 0; i < num_args; i++) {
			free(args[i]);
		}
		free(args);
		clearerr(stdin); 
		exit(0);
	}
	else if (strlen(args[0] != 0) { //else we will try to execute a external command
		pid = fork();
		if (pid < 0) {
			printf("error\n");
		exit(1);
		}
		else if (pid == 0) {
			if (execvp(args[0], args) < 0) {
				execv(args[0], args);
			}
		}
	}
}
