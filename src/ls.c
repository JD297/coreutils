#include <dirent.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	const char* path = ".";

	DIR *d;
	struct dirent *dir;

	if ((d = opendir(path)) == NULL)
	{
		err(EXIT_FAILURE, "%s", path);
	}

	while ((dir = readdir(d)) != NULL) {
		printf("%s\n", dir->d_name);
	}

	if (closedir(d) == -1) {
		err(EXIT_FAILURE, "%s", path);
	}

	return 0;
}
