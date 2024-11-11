#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

void print_usage()
{
	printf("Usage: %s [FILE]\n", TARGET);
	printf("Concatenate and print FILE\n");
	printf("\n");
	printf("JD297 %s source code <https://github.com/jd297/coreutils>\n", TARGET);
}

int main(int argc, char** argv)
{
	if (argc != 2) {
		print_usage();

		exit(EXIT_FAILURE);
	}

	const char* pathname = argv[1];

	int fd;

	if ((fd = open(pathname, O_RDONLY)) == -1) {
		err(EXIT_FAILURE, "%s", pathname);
	}

	char buffer[BUFFER_SIZE];

	ssize_t bytes;

	do {
		memset(buffer, 0, sizeof(buffer));

		if ((bytes = read(fd, buffer, sizeof(buffer))) == -1) {
			err(EXIT_FAILURE, "%s", pathname);
		}

		printf("%.*s", (int)bytes, buffer);
	} while (bytes > 0);

	if (close(fd) == -1) {
		err(EXIT_FAILURE, "%s", pathname);
	}

	return 0;
}
