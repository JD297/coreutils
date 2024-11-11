#include <err.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL) {
		err(EXIT_FAILURE, "getcwd");
	}

	printf("%s\n", cwd);

	return 0;
}
