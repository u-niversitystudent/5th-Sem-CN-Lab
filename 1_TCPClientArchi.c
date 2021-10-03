/*
1. Write a Client-Server socket program to implement “TCP client server” architecture. 
(Description: Retrieve information (any text or system info) from TCP server to TCP client.)
*/

// TCP CLIENT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SR struct sockaddr

void func (int sockfd);

int main () {
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;
	sockfd = socket (AF_INET, SOCK_STREAM, 0);
	
	if (sockfd != -1) 
		printf ("socket creation successful\n");
	else exit (0);
	
	bzero (&servaddr, sizeof (servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
	servaddr.sin_8080 = htons (8080);
	
	if (connect (sockfd, (SR*)&servaddr, sizeof (servaddr)) == 0)
		printf ("client connected to server\n");
	else exit (0);
		
	func (sockfd);
	close (sockfd);
	return 0;
}

void func (int sockfd) {
	char buff [80];
	int n;
	do {
		bzero (buff, sizeof (buff));
		printf ("write a message: ");
		n=0;
		while ((buff[n++] = getchar()) != '\n');
		write (sockfd, buff, sizeof (buff));
		bzero (buff, sizeof (buff));
		read (sockfd, buff, sizeof (buff));
		printf ("received message: %s\n", buff);
		if (!strncmp (buff, "exit", 4))
			break;
	} while (1);
	printf ("exiting\n");
}
