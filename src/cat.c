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

#define HANDLE_ERROR(object, error) fprintf(stderr, "%s: %s: %s\n", TARGET, object, strerror(error)); \
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
			HANDLE_ERROR(argv[i], errno);
		}

		if ((sb.st_mode & S_IFMT) == S_IFDIR) {
			HANDLE_ERROR(argv[i], EISDIR);
		}

		int fd;

		if ((fd = open(argv[i], O_RDONLY)) == -1) {
			HANDLE_ERROR(argv[i], errno);
		}

		char *buf = (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

		if (buf == MAP_FAILED) {
			HANDLE_ERROR("mmap", errno);
		}

		if (write(STDOUT_FILENO, buf, sb.st_size) == -1) {
			HANDLE_ERROR("write", errno);
		}

		if (munmap(buf, sb.st_size) == -1) {
			HANDLE_ERROR("munmap", errno);
		}

		if (close(fd) == -1) {
			HANDLE_ERROR(argv[i], errno);
		}
	}

	return 0;
}
