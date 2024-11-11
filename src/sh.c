#include <err.h>
#include <linux/limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int sh_cd(int argc, char** argv)
{
	if (argc != 2) {
		printf("%s: Usage: %s [dir]\n", TARGET, argv[0]);
		return 1;
	}

	int ret;

	if ((ret = chdir(argv[1])) == -1) {
		perror("chdir");
	}

	return ret;
}

int main()
{
	char* command;
	size_t len = 0;
	ssize_t nread;

	do {
		command = NULL;

		char cwd[PATH_MAX];

		if (getcwd(cwd, sizeof(cwd)) == NULL) {
			err(EXIT_FAILURE, "getcwd");
		}

		printf("%s# ", cwd);

		if ((nread = getline(&command, &len, stdin)) == -1) {
			free(command);
			err(EXIT_FAILURE, "%s", "getline");
		}

		if (nread == 0 || nread == 1) {
			continue;
		}

		if(nread > 0 && command[nread-1] == '\n') {
			command[nread-1] = '\0';
		}

		char** arguments;

		arguments = (char**)calloc(1, sizeof(char*));

		if (arguments == NULL) {
			err(EXIT_FAILURE, "calloc");
		}

		int arguments_index = 0;

		arguments[arguments_index] = strtok(command, " ");

		#define program arguments[0]

		if (program == NULL) {
			free(arguments);
			continue;
		}

		while (arguments[arguments_index] != NULL) {
			arguments_index++;

			arguments = realloc(arguments, (arguments_index + 1) * sizeof(char*));

			if (arguments == NULL) {
				err(EXIT_FAILURE, "realloc");
			}

			arguments[arguments_index] = strtok(NULL, " ");
		}

		if (strcmp(program, "exit") == 0) {
			free(arguments);
			break;
		}

		if (strcmp(program, "cd") == 0) {
			sh_cd(arguments_index, arguments);

			free(arguments);
			continue;
		}

		pid_t pid;

		if ((pid = fork()) == -1) {
			perror("fork");
			continue;
		}

		if (pid == 0) {
			if (execvp(program, arguments) == -1) {
				err(EXIT_FAILURE, "%s", program);
			}

			exit(EXIT_SUCCESS);
		}

		wait(NULL);

		free(arguments);
	} while (1);

	free(command);

	return 0;
}
