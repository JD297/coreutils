#include <dirent.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_usage()
{
	printf("Usage: %s [OPTION]... [FILE]\n", TARGET);
	printf("List stats from files\n");
	printf("\n");
	printf("  -l		List files vertically\n");
	printf("\n");
	printf("JD297 coreutils source code <https://github.com/jd297/coreutils>\n");
}

int main(int argc, char** argv)
{
	int lflag = 0;

	int opt;

	while ((opt = getopt(argc, argv, "l")) != -1) {
		switch (opt) {
			case 'l':
				lflag = 1;
				break;
			default:
				print_usage();
				exit(EXIT_FAILURE);
		}
	}

	char* path = NULL;

	for (int index = optind; index < argc; index++) {
		path = argv[index];
		break;
	}

	if (path == NULL) {
		path = ".";
	}

	DIR *d;
	struct dirent *dir;

	if ((d = opendir(path)) == NULL)
	{
		err(EXIT_FAILURE, "%s", path);
	}

	char** list;

	list = (char**)calloc(1, sizeof(char*));

	int list_index = 0;

	if (list == NULL) {
		err(EXIT_FAILURE, "calloc");
	}

	while ((dir = readdir(d)) != NULL) {
		list = realloc(list, (list_index + 1) * sizeof(char*));

		if (list == NULL) {
			err(EXIT_FAILURE, "realloc");
		}

		list[list_index] = dir->d_name;

		list_index++;
	}

	if (closedir(d) == -1) {
		err(EXIT_FAILURE, "%s", path);
	}

	for (int i = 0; i < list_index; i++) {
		printf("%s", list[i]);

		if (lflag || list_index - 1 == i) {
			printf("\n");
		} else {
			printf("  ");
		}
	}

	return 0;
}
