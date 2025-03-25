#include "./headers/util.h"

// Get the port when starting the program
int main(int argc, char *argv[]) {
	int port;
	if (argc < 2) {
		usage();
		return 0;
	}

	port = atoi(argv[1]);
	int s = setup_listener(port);

	shell(s);
	close(s);
	return 0;
}

void usage() {
	printf("ERROR running RSC!\nRun: rsc <listen port>\n");
}
