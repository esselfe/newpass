#include <stdio.h>
#include <getopt.h>

const char *newpass_version_string = "0.0.1";

static const struct option long_options[] = {
	{"help", no_argument, NULL, 'h'},
	{"version", no_argument, NULL, 'V'},
	{"verbose", no_argument, NULL, 'v'},
	{NULL, 0, NULL, 0}
};
static const char *short_options = "hVv";

unsigned char verbose;

void ShowHelp(void) {
	printf("newpass usage: newpass { -h/--help | -V/--version | -v/--verbose }\n");
}

void ShowVersion(void) {
	printf("newpass %s\n", newpass_version_string);
}

int main(int argc, char **argv) {
	int c;
	while (1) {
		c = getopt_long(argc, argv, short_options, long_options, NULL);
		if (c < 0)
			break;

		switch (c) {
		case 'h': // --help
			ShowHelp();
			exit(0);
			break;
		case 'V': // --versioin
			ShowVersion();
			exit(0);
			break;
		case 'v': // --verbose
			verbose = 1;
			break;
		}
	}

	if (verbose)
		printf("newpass %s started\n", newpass_version_string);

	return 0;
}

