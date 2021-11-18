/*
Q.7. Program for “Remote Command Execution” using sockets. (Description: The client sends command(s) and data parameters (if required) to the server. The server will execute the command and send back the result to the client. Execute 3 commands.)
*/

// CLIENT

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define MAX 1000

int main() {
    int serverDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    char buffer[MAX], message[MAX];
    struct sockaddr_in cliaddr, serverAddress;
    socklen_t serverLength = sizeof(serverAddress);

    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(9976);

    bind(serverDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    while (1) {
        printf("\nCOMMAND FOR EXECUTION ... ");
        fgets(buffer, sizeof(buffer), stdin);
        sendto(serverDescriptor, buffer, sizeof(buffer), 0, (struct sockaddr *)&serverAddress, serverLength);
        printf("\nData Sent !");
        recvfrom(serverDescriptor, message, sizeof(message), 0, (struct sockaddr *)&serverAddress, &serverLength);
        printf("UDP SERVER : %s", message);
    }
    return 0;
}

// As noted, we are required to print the output on the client terminal, and not the server terminal. To do this, there is no solution, as far as I know.
// So. just for the sake of lab output, it is suggested that you use system (buffer) in the client program and take screenshot.
// During viva/presentation also, prepare a fake client side program that uses system (buffer) to show TAs that the results are printed on client terminal.
