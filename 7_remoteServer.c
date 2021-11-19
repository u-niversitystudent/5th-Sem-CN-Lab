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
#include <unistd.h>

#define MAX 1000

int main() {
	int serverDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    int size;
    char buffer[MAX], message[10][MAX], msg[MAX]=" ";
	char out[]=">a.txt";
	
    struct sockaddr_in clientAddress, serverAddress;

    socklen_t clientLength = sizeof(clientAddress);

    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(9976);

	int length;
    bind(serverDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    while (1) {
        bzero(buffer, sizeof(buffer));
		bzero(msg,1000);
        recvfrom(serverDescriptor, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddress, &clientLength);
		system(buffer);
        printf("Command Executed ... %s ", buffer);
        length=strlen(buffer) - 1;
		printf ("%d", length);
		buffer[length]='>';
		buffer[length+1]='a';
		buffer[length+2]='\0';
		system(buffer);
		FILE *file = fopen ("a", "r");
		for (int i=0; i<10; i++) {
			fgets(message[i], 50, file);
			strcat(msg, message[i]);
			bzero(message[i],50);
		}
		sendto(serverDescriptor, msg, sizeof(msg), 0, (struct sockaddr *)&clientAddress, clientLength);
    }
    close(serverDescriptor);
    return 0;
}
