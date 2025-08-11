#include <err.h>
#include <sys/reboot.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int opt;

	int howto = RB_HALT;

	while ((opt = getopt (argc, argv, "pr")) != -1) {
		switch (opt) {
		case 'p':
			howto = RB_HALT | RB_POWERDOWN;
			break;
		case 'r':
			howto = RB_AUTOBOOT;
			break;
		default:
			fprintf(stderr, "usage: %s [-p]\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if (reboot(howto) == -1) {
		err(EXIT_FAILURE, "reboot");
	}

	return 0;
}
