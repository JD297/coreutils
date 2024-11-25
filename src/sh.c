#include <err.h>
#include <errno.h>
#include <linux/limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

int sh_cd(int argc, char** argv)
{
	if (argc != 2) {
		fprintf(stderr, "%s: Usage: %s [dir]\n", TARGET, argv[0]);
		exit(EXIT_FAILURE);
	}

	int ret;

	if ((ret = chdir(argv[1])) == -1) {
		err(EXIT_FAILURE, "chdir");
	}

	return ret;
}

int main()
{
	char *command;
	size_t len = 0;
	ssize_t nread;

	size_t arguments_size = 4096 * 2; // TODO get pagesize

	char** arguments = (char**)mmap(NULL, arguments_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, NULL, 0);

	if (arguments == MAP_FAILED) {
		err(EXIT_FAILURE, "mmap (arguments)");
	}

	size_t command_size = 4096 * 2; // TODO get pagesize

	if ((command = (char *)mmap(NULL, command_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, NULL, 0)) == MAP_FAILED) {
		err(EXIT_FAILURE, "mmap (command)");
	}

	do {
		command = memset(command, 0, command_size);

		char cwd[PATH_MAX];

		if (getcwd(cwd, sizeof(cwd)) == NULL) {
			err(EXIT_FAILURE, "getcwd");
		}

		printf("%s# ", cwd);

		if ((nread = read(STDIN_FILENO, command, command_size)) == -1) {
			err(EXIT_FAILURE, "read (command)");
		}

		if (nread == 0 || nread == 1) {
			continue;
		}

		if(nread > 0 && command[nread-1] == '\n') {
			command[nread-1] = '\0';
		}

		arguments = memset(arguments, 0, arguments_size);

		int arguments_index = 0;

		char *saveptr;

		arguments[arguments_index] = strtok_r(command, " ", &saveptr);

		#define program arguments[0]

		if (program == NULL) {
			continue;
		}

		while (arguments[arguments_index] != NULL) {
			arguments_index++;

			if (arguments_size < arguments_index * sizeof(char *)) {
				arguments = (char **)mremap(arguments, arguments_size, arguments_size * 2, MREMAP_MAYMOVE | MREMAP_DONTUNMAP);
				arguments_size = arguments_size * 2;

				if (arguments == MAP_FAILED) {
					err(EXIT_FAILURE, "mremap (arguments)");
				}
			}

			arguments[arguments_index] = strtok_r(NULL, " ", &saveptr);
		}

		if (strcmp(program, "exit") == 0) {
			break;
		}

		if (strcmp(program, "cd") == 0) {
			sh_cd(arguments_index, arguments);

			continue;
		}

		pid_t pid;

		if ((pid = fork()) == -1) {
			fprintf(stderr, "%s: fork: ", strerror(errno));
			continue;
		}

		if (pid == 0) {
			if (execvp(program, arguments) == -1) {
				err(EXIT_FAILURE, "%s", program);
			}

			exit(EXIT_SUCCESS);
		}

		wait(NULL);
	} while (1);

	if (munmap(arguments, arguments_size) == -1) {
		err(EXIT_FAILURE, "munmap (arguments)");
	}

	if (munmap(command, command_size) == -1) {
		err(EXIT_FAILURE, "munmap (command)");
	}

	return 0;
}
