/*
1. Write a Client-Server socket program to implement “TCP client server” architecture. 
(Description: Retrieve information (any text or system info) from TCP server to TCP client.)
*/

// TCP SERVER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void func (int sockfd);
int main () {
	int sockfd, len, connfd;
	struct sockaddr_in servaddr, cli;
	sockfd = socket (AF_INET, SOCK_STREAM, 0);
	
	if (sockfd != -1)
		printf ("socket successfully created\n");
	else exit (0);
	
	bzero (&servaddr, sizeof (servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
	servaddr.sin_8080 = htons (8080);
	
	if ((bind (sockfd, (struct sockaddr*)&servaddr, sizeof (servaddr))) == 0)
		printf ("socket successfully binded\n");
	else exit (0);
	
	if (listen (sockfd, 5) == 0)
		printf ("server is listening in for connections\n");
	else exit (0);
	
	len = sizeof (cli);
	connfd = accept (sockfd, (struct sockaddr*)&cli, &len);
	
	if (connfd >= 0)
		printf ("server has accepted a client\n");
	else exit (0);
	
	func (connfd);
	close (sockfd);
	return 0;
}

void func (int sockfd) {
	char buff[80];
	int n;
	do {
		bzero (buff, 80);
		read (sockfd, buff, sizeof (buff));
		printf ("client says: %s\n", buff);
		bzero (buff, 80);
		printf ("sending server message: ");
		n=0;
		while ((buff [n++] = getchar ()) != '\n');
		write (sockfd, buff, sizeof (buff));	
		if (!strncmp ("exit", buff, 4))
			break;
	} while (1);
	printf ("exiting\n");
}
