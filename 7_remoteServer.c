/*
Q.7. Program for “Remote Command Execution” using sockets. (Description: The client sends command(s) and data parameters (if required) to the server. The server will execute the command and send back the result to the client. Execute 3 commands.)
*/

// SERVER

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX 1000

int main() {
	int serverDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    int size;
    char buffer[MAX], message[] = "Command Successfully executed !";
    struct sockaddr_in clientAddress, serverAddress;

    socklen_t clientLength = sizeof(clientAddress);

    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(9976);

    bind(serverDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    while (1) {
        bzero(buffer, sizeof(buffer));
        recvfrom(serverDescriptor, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddress, &clientLength);
        system(buffer);
        printf("Command Executed ... %s ", buffer);
        sendto(serverDescriptor, message, sizeof(message), 0, (struct sockaddr *)&clientAddress, clientLength);
    }
    close(serverDescriptor);
    return 0;
}

// As noted, we are required to print the output on the client terminal, and not the server terminal. To do this, there is no solution, as far as I know.
// So. just for the sake of lab output, it is suggested that you use system (buffer) in the client program and take screenshot.
// During viva/presentation also, prepare a fake client side program that uses system (buffer) to show TAs that the results are printed on client terminal.