#include <dirent.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int ch;

	while ((ch = getopt(argc, argv, "")) != -1) {
		switch (ch) {
		default:
			fprintf(stderr, "usage: %s\n", argv[0]);
			return EXIT_FAILURE;
		}
	}

	const char *filename = ".";

	DIR *dirp;

	if ((dirp = opendir(filename)) == NULL) {
		err(EXIT_FAILURE, "%s", filename);
	}

	struct dirent *dp;

	while ((dp = readdir(dirp)) != NULL) {
		printf("%s\n", dp->d_name);
	}

	closedir(dirp);

	return EXIT_SUCCESS;
}
