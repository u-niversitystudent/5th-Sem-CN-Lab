/*
Q.4. Write an “Echo Client” and “Echo Server” using UDP to estimate the round trip Time from client to the server. The server should be such that it can accept Multiple connections at any given time. (Description: Multiple clients connected at the same time to one server for connection establishment, server has to listen to all the clients, use I/O monitoring calls if required.)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
  
int main () {
    int sockfd;
    char buffer[100];
    struct sockaddr_in servaddr, cliaddr;
    int len, n;
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
		exit (0);
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    servaddr.sin_family    = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8080);
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
		exit (0);
    len = sizeof(cliaddr);
	printf ("connection established");
	while (true) {
		memset (buffer, 0, 100);
		n = recvfrom(sockfd, (char *)buffer, 100, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
		buffer[n] = '\0';
		sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
		printf ("\nreceived message echoed back to its respective client");
	}
	close (sockfd);
    return 0;
}