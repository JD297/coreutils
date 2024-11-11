#include <err.h>
#include <linux/reboot.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int reboot(int cmd);

void print_usage()
{
	printf("Usage: %s [OPTION]\n", TARGET);
	printf("Sends the given option as argument to the reboot syscall.\n");
	printf("\n");
	printf("  -H		Halt the system\n");
	printf("  -P		Power-off the system\n");
	printf("  -R		Reboot the system\n");
	printf("  -S		Suspends (hibernate) the system\n");
}

int main(int argc, char** argv)
{
	if (argc > 2) {
		print_usage();
		exit(EXIT_FAILURE);
	}

	int opt;

	int cmd = LINUX_REBOOT_CMD_POWER_OFF;

	while ((opt = getopt (argc, argv, "HPRS")) != -1) {
		switch (opt) {
			case 'H':
				cmd = LINUX_REBOOT_CMD_HALT;
				break;
			case 'P':
				cmd = LINUX_REBOOT_CMD_POWER_OFF;
				break;
			case 'R':
				cmd = LINUX_REBOOT_CMD_RESTART;
				break;
			case 'S':
				cmd = LINUX_REBOOT_CMD_SW_SUSPEND;
				break;
			default:
				print_usage();
				exit(EXIT_FAILURE);
		}
	}

	if (reboot(cmd) == -1) {
		err(EXIT_FAILURE, "reboot (0x%08x)", cmd);
	}

	return 0;
}
