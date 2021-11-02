// Q. Write a program for “Connectionless Iterative Service” in which the server finds the factorial of a number sent by the client and sends it back.

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
    int len, n, l;
	unsigned long int f;
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
		f = 1;
		memset (buffer, 0, 100);
		n = recvfrom(sockfd, (char *)buffer, 100, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
		buffer[n] = '\0';
		l = atoi (buffer);
		while (l > 0) {
			f = f * l;
			l = l - 1;
		}
		sprintf (buffer, "%ld", f);
		printf ("\nfactorial value sent back");
		sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
	}
	close (sockfd);
    return 0;
}
