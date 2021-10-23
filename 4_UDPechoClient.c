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
#include <time.h>
  
int main () {
    int sockfd;
    char buffer[100];
	clock_t t;
	int len, n;
    struct sockaddr_in servaddr;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
		exit (0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(8080);
	printf ("connection established\n");
	while (true) {
		memset (buffer, 0, 100);
		fgets (buffer, 100, stdin);
		t = clock ();
		sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
		n = recvfrom(sockfd, (char *)buffer, 100, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
		buffer[n-1] = '\0';
		t = clock () - t;
		printf("%s takes %f time to echo back\n", buffer, (((double) (t)) / CLOCKS_PER_SEC));
	}
    close (sockfd);
    return 0;
}