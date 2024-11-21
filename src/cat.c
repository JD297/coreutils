#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>

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

	struct stat sb;

	if (stat(argv[1], &sb) == -1) {
		err(EXIT_FAILURE, "stat");
	}

	int fd;

	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		err(EXIT_FAILURE, "%s", argv[1]);
	}

	char *buf = (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	if (buf == MAP_FAILED) {
		err(EXIT_FAILURE, "mmap");
	}

	if (write(STDOUT_FILENO, buf, sb.st_size) == -1) {
		err(EXIT_FAILURE, "write");
	}

	if (munmap(buf, sb.st_size) == -1) {
		err(EXIT_FAILURE, "munmap");
	}

	if (close(fd) == -1) {
		err(EXIT_FAILURE, "%s", argv[1]);
	}

	return 0;
}
