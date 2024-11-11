#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>

int main()
{
	struct utsname uts;

	if (uname(&uts) == -1) {
		err(EXIT_FAILURE, "uname");
	}

	printf("%s %s %s %s %s\n", uts.sysname, uts.nodename, uts.release, uts.version, uts.machine);

	return 0;
}
