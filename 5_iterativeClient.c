// Q.5. Write a program for “Connectionless Iterative Service” in which the server finds the factorial of a number sent by the client and sends it back.

// CLIENT

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
		printf("> ");
		fgets (buffer, 100, stdin);
		sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
		n = recvfrom(sockfd, (char *)buffer, 100, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
		buffer[n] = '\0';
		printf("Factorail> %s\n", buffer);
	}
    close (sockfd);
    return 0;
}
