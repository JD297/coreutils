#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define HANDLE_ERROR(object, error)\
        fprintf(stderr, "%s: %s: %s\n", argv[0], object, strerror(error)); \
        continue

int main(int argc, char** argv)
{
	int ch;
	int uflag = 0;
	int code = EXIT_SUCCESS;

	while ((ch = getopt(argc, argv, "u")) != -1) {
		switch (ch) {
		case 'u':
			uflag = 1;
			break;
		default:
			fprintf(stderr, "usage: %s [-u] [file...]\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if (uflag == 1) {
		if (setvbuf(stdout, NULL, _IONBF, 0)) {
			err(EXIT_FAILURE, "setvbuf");
		}
	}

	if (argc == optind) {
		argc++;
		argv[optind] = "-";
	}

	for (int i = optind; i < argc; i++) {
		int fd;
		struct stat sb;

		if (strcmp("-", argv[i]) == 0) {
			long pz = sysconf(_SC_PAGE_SIZE);

			char *buf;

			if ((buf = malloc(pz)) == NULL) {
				err(EXIT_FAILURE, NULL);
			}

			for (ssize_t nread = 0; (nread = read(STDIN_FILENO, buf, pz)) > 0;) {
				write(STDOUT_FILENO, buf, nread);
			}

			free(buf);

			continue;
		}

		if (stat(argv[i], &sb) == -1) {
			HANDLE_ERROR(argv[i], errno);
			code = EXIT_FAILURE;
		}

		if ((sb.st_mode & S_IFMT) == S_IFDIR) {
			HANDLE_ERROR(argv[i], EISDIR);
			code = EXIT_FAILURE;
		}

		if ((fd = open(argv[i], O_RDONLY)) == -1) {
			HANDLE_ERROR(argv[i], errno);
			code = EXIT_FAILURE;
		}

		char *buf = (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

		if (buf == MAP_FAILED) {
			HANDLE_ERROR("mmap", errno);
			code = EXIT_FAILURE;
		}

		if (write(STDOUT_FILENO, buf, sb.st_size) == -1) {
			HANDLE_ERROR("write", errno);
			code = EXIT_FAILURE;
		}

		if (munmap(buf, sb.st_size) == -1) {
			HANDLE_ERROR("munmap", errno);
			code = EXIT_FAILURE;
		}

		if (close(fd) == -1) {
			HANDLE_ERROR(argv[i], errno);
			code = EXIT_FAILURE;
		}
	}

	return code;
}
