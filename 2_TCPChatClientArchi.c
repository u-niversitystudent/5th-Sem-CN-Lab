/*
2. Write a Client-Server socket program to implement “TCP chat server”. 
(Description: Two client systems connected to one central server for connection establishment, communication / chat have to be done in client machines.)
*/

// TCP CHAT CLIENT
// ALSO SEE NOTE AT END ON HOW TO RUN OUTPUT

#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>

char msg [500];

void *getmg (void *server_connect) {
	int sockfd = *((int *)server_connect);
	int len;
	while ((len = recv (sockfd, msg, 500, 0)) > 0) {
		msg [len] = '\0';
		fputs (msg, stdout);
	}
}

int main (int argc, char *argv[]) {
	int sockfd = socket (AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servaddr;
	int len;
	char sendmg [500], name_of_client [100];
	bzero (&servaddr, sizeof (servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
	servaddr.sin_port = htons (8080);
	
	if ((connect (sockfd, (struct sockaddr*)&servaddr, sizeof (servaddr)) == 0))
		printf ("connected to server\n");
	else
		exit (0);	
	pthread_t recvt;
	pthread_create (&recvt, NULL, (void*)&getmg, &sockfd);
		
	strcpy (name_of_client, argv [1]);
	printf ("Enter Your Message: ");
	while (fgets (msg, 500, stdin) > 0) {
		strcpy (sendmg, name_of_client);
		strcat (sendmg, ":");
		strcat (sendmg, msg);
		len = write (sockfd, sendmg, strlen (sendmg));
		if (len < 0)
			printf ("msg failed to send\n");
	}
	
	pthread_join (recvt, NULL);
	close (sockfd);
	return 0;
}

// NOTE: OUTPUT ERROR?
/*
Server
$ gcc -pthread server.c -o outserver
$ ./outserver

Client
$ gcc -pthread client.c -o outclient

On each terminal for client
$ ./outclient client1
$ ./outclient client2
*/
