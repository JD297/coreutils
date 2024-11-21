#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>

void print_usage()
{
	printf("Usage: %s [FILE]...\n", TARGET);
	printf("Concatenate and print FILE(s)\n");
	printf("\n");
	printf("JD297 %s source code <https://github.com/jd297/coreutils>\n", TARGET);
}

#define HANDLE_ERROR(object) fprintf(stderr, "%s: %s: %s\n", TARGET, object, strerror(errno)); \
                             continue

int main(int argc, char** argv)
{
	if (argc < 2) {
		print_usage();

		exit(EXIT_FAILURE);
	}

	for (int i = 1; i < argc; i++) {
		struct stat sb;

		if (stat(argv[i], &sb) == -1) {
			HANDLE_ERROR(argv[i]);
		}

		int fd;

		if ((fd = open(argv[i], O_RDONLY)) == -1) {
			HANDLE_ERROR(argv[i]);
		}

		char *buf = (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

		if (buf == MAP_FAILED) {
			HANDLE_ERROR("mmap");
		}

		if (write(STDOUT_FILENO, buf, sb.st_size) == -1) {
			HANDLE_ERROR("write");
		}

		if (munmap(buf, sb.st_size) == -1) {
			HANDLE_ERROR("munmap");
		}

		if (close(fd) == -1) {
			HANDLE_ERROR(argv[i]);
		}
	}

	return 0;
}
