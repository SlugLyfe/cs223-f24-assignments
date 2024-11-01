/*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon 
 * Date: 1/11/24
 * Description: My own personal and customized
 * very simple shell made from scratch! 
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void print_intro() {
	char cwd[64];
	struct passwd *pw = getpwuid(geteuid());
	if (getcwd(cwd, sizeof(cwd)) != NULL){
		printf(ANSI_COLOR_MAGENTA "KabirSuperPinkShell %s %s " ANSI_COLOR_RESET, cwd, pw->pw_name);
	}
	else {
		perror("cwd or pwuid error");
		exit(EXIT_FAILURE);
	}
}

void command_execution(char *command){
	char *args[64];
	char *token = strtok(command, " ");
	int i = 0;
	while (token != NULL){
		args[i++] = token;
		token = strtok(NULL, " ");
	}
	args[i] = NULL;
	pid_t pid = fork();
	if (pid == 0) {
		if (execvp(args[0], args) == -1){
			perror("Failed to execute the command");
			exit(EXIT_FAILURE);
		}
	} else {
		int status;
		wait(&status);
		}
	} 


int main() {
	char *input;
	while(1) {
		print_intro();
		input = readline(NULL);
		if (input == NULL){
			printf("\n");
			continue;
		}
		if (strcmp(input, "exit") == 0){
			free(input);
			break;
		}
		add_history(input);
		command_execution(input);
		free(input);

	}
  return 0;
}
