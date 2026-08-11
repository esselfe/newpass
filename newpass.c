#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <getopt.h>

const char *newpass_version_string = "0.0.1";

static const struct option long_options[] = {
	{"help", no_argument, NULL, 'h'},
	{"version", no_argument, NULL, 'V'},
	{"length", required_argument, NULL, 'l'},
	{"verbose", no_argument, NULL, 'v'},
	{NULL, 0, NULL, 0}
};
static const char *short_options = "hVl:v";

//#define DEFAULT_LENGTH 40
#define DEFAULT_LENGTH 4

unsigned char verbose;
unsigned long length;
unsigned long random_block_size = 4096;

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
		case 'l': // --length
			if (optarg != NULL && strlen(optarg) > 0)
				length = atoi(optarg);

			if (length < 1) {
				length = DEFAULT_LENGTH;
				fprintf(stderr, "newpass warning: the specified length is smaller than 1,\n"
					"setting to the default length of %lu\n", length);
			}

			break;
		case 'v': // --verbose
			verbose = 1;
			break;
		}
	}

	if (verbose)
		printf("newpass %s started\n", newpass_version_string);

	if (!length)
		length = DEFAULT_LENGTH;
	
	FILE *fr_random = fopen("/dev/urandom", "rb");
	if (fr_random == NULL) {
		fprintf(stderr, "newpass error: Cannot open /dev/urandom as random source: %s\n",
			strerror(errno));
		exit(ECANCELED);
	}

	// Just read one block of random data for now
	char *random_data = malloc(random_block_size);
	if (random_data == NULL) {
		fprintf(stderr, "newpass error: malloc(%lu) returned NULL, exiting.\n",
			random_block_size);
		fclose(fr_random);
		return ENOMEM;
	}
	random_data[random_block_size - 1] = '\0';
	size_t bytes_read = fread(random_data, 1, random_block_size - 1, fr_random);
	if (bytes_read == 0) {
		if (ferror(fr_random)) {
			fprintf(stderr, "newpass error: fread() failed reading from random source: %s\n",
				strerror(errno));
		} else {
			fprintf(stderr, "newpass error: fread() returned 0 on random source file.\n");
		}
		free(random_data);
		fclose(fr_random);
		return 1;
	}

	srand((unsigned int)time(NULL) % 10000);

	unsigned int cnt = 0;
	while (cnt < length) {
		if (rand() % 2)
			printf("%01X", (unsigned char)random_data[cnt] & 0x0F);
		else
			printf("%01x", (unsigned char)random_data[cnt] & 0x0F);

		++cnt;
	}

	printf("\n");

	return 0;
}

