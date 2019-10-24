/**************************************************************************************
main for my c shell:                                                                   |
Why did you put almost all of your functions in main you ask?                          |
Because I am dumb. 								       |
I hate it as much as you do, but as soon as I start moving them the ssh                |
is going to crash, and I cant figure out how to copy and paste, only cut and paste.    |
I am also running out of time... I tried to commment them as if they where functions                                                       |
created by chrsotpher outhwaite                                                        |
***************************************************************************************/
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "get_path.h"
#include "get_arg.h"
#include <dirent.h>

#define BUFFSIZE 128

char buf[BUFFSIZE];
char **args;
int num_args;
pid_t pid;
char *prompt;
struct pathelement  *path;
char *prompt_base;
char *ptr;
extern char **environ;
char * prev;
char * prefix;
//char cmd[64];

//signal handling stuff
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

//frees the path
void free_path(struct pathelement * path) {
	
	struct pathelement *p;
		while (path) {
		//printf("Path element: %s\n", path->element);
		p = path; 
		path = path->next;
		//printf("copied element: %s\n", p->element);
		//free(p->next); WHY DOESNT THIS WORK!!!!!!!!!!!!!!!!!!!!!!!!!
		free(p);
		
	}

}


//inpute: a command
//output: none
//prints the appropriate line
void which(char * command) {
	char cmd[64];
	struct pathelement *p;
	struct pathelement *root;
	p = get_path();
	root = p;
	while (p) {
		//strcpy(cmd, p->element); 
		//strncat(cmd, "/", 1);
		//strncat(cmd, command, strlen(command));
		sprintf(cmd, "%s/%s", p->element, command);
		if (access(cmd, X_OK) == 0) {
			printf("[%s]\n", cmd);
			fflush(stdout);
		}
		p = p->next;
	}
	free_path(root);
}

//inpute: command
//output: boolean, can the command be executed on the path. 
//side effects: none
int which2(char * command) {
	char cmd[64];
	struct pathelement *p;
	struct pathelement *root;
	p = get_path();
	root = p;
	while (p) {
		//strcpy(cmd, p->element); 
		//strncat(cmd, "/", 1);
		//strncat(cmd, command, strlen(command));
		sprintf(cmd, "%s/%s", p->element, command);
		if (access(cmd, X_OK) == 0) {
			free_path(root);
			return 1;
		}
		p = p->next;
	}
	free_path(root);
	return 0;
}
//inpute: command
//output: none
//side effects: prints out appropriate line

void where(char * command) {
	char cmd[64];
	struct pathelement *p;
	struct pathelement *root;
	p = get_path();
	root = p;
	while (p) {
		//strcpy(cmd, p->element); 
		sprintf(cmd, "%s/%s", p->element, command);
		//strncat(cmd, command, strlen(command));
		if (access(cmd, F_OK) == 0) {
			printf("[%s]\n", cmd);
			fflush(stdout);
			//break;
		}
		p = p->next;
	}
	free_path(root);
}
//sorry about this

int main(void) {
	signal(SIGINT, termsig);
	signal(SIGTERM, handle_sigint);
	signal(SIGSTOP, handle_sigint);
	//int nc = REDIR_OW;
	
	path = get_path();
	

	prev = getcwd(NULL, 0);
	prompt_base = getcwd(NULL, 0);
	prompt = getcwd(NULL, 0);
	//prompt_base = getcwd(NULL, 0);
	printf("Starting shell\n%s> ", prompt);
	fflush(stdout);
	start:
	while (fgets(buf, BUFFSIZE, stdin) != NULL) {

	num_args = get_argc(buf);
	if (num_args == 0) {

		if (prefix) printf("%s%s> ", prefix, prompt); 
		else printf("%s> ", prompt);
		break;
	}
	
	args = get_argv(buf, num_args);
	//calls which, hey im actually using helper methods
	if (strcmp (args[0], "which") == 0) {
		if (num_args == 2) {
			which(args[1]);
		}
		else {
			printf("incorrect number of args");
		}
		
		printf(" ");
	}
	//calls where...
	else if (strcmp (args[0], "where") == 0) {
		if (num_args == 2) {
			where(args[1]);
		}
		else {
			printf("incorrect number of args");
		}
	}
	//calls pwd, this one is simple and doesnt need much explaining.
	//make sure to free the pointer.
	else if(strcmp (args[0], "pwd") == 0) { //prints working directory
		ptr = getcwd(NULL, 0);
		printf("CWD = %s\n", ptr);
		free(ptr);
	}
	//hardest thing to implement by far, at least 5 hours imo...
	else if(strcmp(args[0], "prompt") == 0) {
		//inpute: none
		//output: none
		//side effects: asks the user for a prompt. takes said prompt and puts it into memory
		//then puts the prompt base at the end of the prompt. prefix is also populated.
		free(prefix);
		prefix = malloc(32);
		if (num_args == 1) {
			//char * prompt_pre = malloc(32); //sizeof not needed characters are 1
			printf("Enter a Prompt: ");
			fgets(prefix, 32, stdin);
			//int len = strlen(prompt_base);
			prefix[strlen(prefix) - 1] = '\0';
			//strcpy(prefix, prompt_pre);
			//strcat(prefix, prompt_base);
			//printf("\n The prompt now is %s\n", prompt_pre);
			//free(prompt);
			//strcpy(prompt, prefix);
			//prompt = prefix;
			//free(prompt_pre);
		}
		//input: a string, that will be new prompt prefix.
		//output:  none
		//side effects: takes the second arg, and concacts it in front of the current prompt.
		else if(num_args == 2) {
			//append arg[1] onto the front of prompt;
			strcpy(prefix, args[1]);
			
		}
		else {
			free(prefix);
			printf(" Wrong amount of args\n");
		}
	}
	else if(strcmp(args[0], "cd") == 0) {
		char * tmp = getcwd(NULL, 0);
		//relatively simple, just moves to home dir
		if (num_args == 1) {
			char *home = getenv("HOME");
			chdir(home);
			prev = tmp;
		}
		else if (num_args > 2) {
			printf("Wrong Amount of Args\n");
		}
		//input: a string that is a directory or -
		//output: none
		//side effects: changes chdir
		else {
			if (strcmp(args[1], "-")) {
				chdir(prev);
				prev = tmp;
			}
			else {
				chdir(args[1]);
			}
		}
	}
	//simple... gets the pid
	else if(strcmp(args[0], "pid") == 0) {
		int pid_ans = getpid();
		printf("PID = %d\n", pid_ans);
	}
	//inputes: a process id to kill, or a process id to send a signal to, and the signal type
	//output: none (deja vu)
	//side effects: kills the process
	else if (strcmp(args[0], "kill") == 0) {
		if (num_args < 2) {
			printf(" Need more aguments\n");
		}
		else if (num_args == 2) {
			kill(atoi(args[1]), SIGTERM);
		}
		else if (num_args == 3) {
			char * signal = malloc(sizeof(char) * strlen(args[1]));
			strcpy(signal, &args[1][1]);
			int the_signal = atoi(signal); //getting concerend with variable names.
			kill(atoi(args[2]), the_signal);
			free(signal);
		}
		
	}
	
	else if (strcmp(args[0], "list") == 0) {
		struct dirent * dire;
		//input: no arguments, opens current dir
		if (num_args == 1) {
			DIR *dr = opendir(".");
			while((dire = readdir(dr)) != NULL) {
				printf("%s\n", dire->d_name);
			}
		}
		//input: a directroy. attempts to open that directroy.
		else if (num_args == 2) { 
			DIR *dr = opendir(args[1]);
			if (dr == NULL) {
				printf("could not open directory, probably your fault\n");
			}
			else {
				while((dire = readdir(dr)) != NULL) {
					printf("%s\n", dire->d_name);
				}
			}
		}
	}
	//gets an environment variable, if one is given as input
	//no input, prints all env variables.
	else if (strcmp(args[0], "printenv") == 0) {
		if (num_args > 2) {
			//printf("stderr not working");
			fprintf(stderr, "The instrauctions say to print the tcsh error, but I tried on this on tcsh and no error was printed. So this is your error message I guess.\n");
		}
		else if (num_args == 2) {
			char* env = getenv(args[1]);
			printf("%s", env);
			free(env);
		}
		else if (num_args == 1) {
			int i = 1;
			char *s = *environ;
			
			for (; s; i++) {
				printf("%s\n", s);
				s = *(environ+i);
			}
		}
	}
	//no inpute: prints all env variables
	//1 inpiute: creates new variable with no value
	//2 inputes: creates or overwrites variable with (second arg) with a value (third arg)
	else if (strcmp(args[0], "setenv") == 0) {
		if (num_args == 1) {
			int i = 1;
			char *s = *environ;
			
			for (; s; i++) {
				printf("%s\n", s);
				s = *(environ+i);
			}
		}
		else if (num_args == 2) {
			setenv(args[1], " ", 0);
		}
		else if (num_args == 3) {
			setenv(args[1], args[2], 1);
			if (strcmp(args[1], "PATH") == 0) {
				//code for freeing old path and updating new one
				free_path(path);
				path = get_path();
			}
			else if (strcmp(args[1], "HOME") == 0) {
				//code for putting HOME directory as new home dir
			}
		}
	}
	//exits and hopefully frees everything that needs to be freed
	else if (strcmp(args[0], "exit") == 0) {
		free(prompt);
		free(prefix);
		//printf("error at freeing path?");
		//fflush(stdout);
		free_path(path);
		//printf("guess the error is somewhere else");
		//fflush(stdout);
		for (int i = 0; i < num_args; i++) {
			free(args[i]);
		}
		free(args);
		clearerr(stdin);
		exit(0);
	}
	else if (args[0][0] == '/' || args[0][0] == '.') { //else try's to execute an external command
		if (access(args[0], X_OK) == 0) {
			pid = fork();

			if (pid < 0) {
			printf("error\n");
			exit(1);
			}
			else if (pid == 0) {
				execv(args[0], args);
				printf("Error with running %s\n", args[0]);
				exit(0);
			}
		wait(NULL); //can wait here, we know child process will exit
		}
		else {
			printf("command not found\n");
		}
	}
	else { //executes an external command. uses execvp for ease of use. this is also why which2() exists
		int found = which2(args[0]);
		if (found) {
			pid = fork();
			if (pid < 0) {
				printf("error\n");
				exit(1);
			}
			else if (pid == 0) {
				execvp(args[0], args);
				printf("Error with running %s\n", args[0]);
				exit(0);
			}
			wait(NULL); //can wait here, we know child process will exit
		}
		else {
			printf("command not found\n");
		}
	}
	

		for (int i = 0; i < num_args; i++) {
			free(args[i]);
		}
	

		free(args);
//I cant figure why this has to be in the while loop but it does...
	free(prompt);
	prompt = getcwd(NULL, 0);
	if (prefix) {
		char * tmp = malloc(strlen(prefix));
		strcpy(tmp, prefix);
		strcat(prefix, prompt);
		printf("%s> ", prefix);
		free(prefix);
		prefix = tmp;
		
	}
	else {
		printf("%s> ", prompt);
	}
	} //end of while loop

	clearerr(stdin);
	goto start;
	exit(0);
}
