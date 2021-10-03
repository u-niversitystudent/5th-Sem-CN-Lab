/*
2. Write a Client-Server socket program to implement “TCP chat server”. 
(Description: Two client systems connected to one central server for connection establishment, communication / chat have to be done in client machines.)
*/

// TCP CHAT SERVER

#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;
int clients [20];
int n = 0;

void sendmg (char *msg, int curr) {
	pthread_mutex_lock (&mutex);
	for (int i=0; i<n; ++i)
		if (send (clients [i], msg, strlen (msg), 0) < 0) {
			printf ("FAIL: SEND MESSAGE TO CLIENT %d\n", i);
			continue;
		}
	pthread_mutex_unlock (&mutex);
}

void *rgetmg (void *server_connect) {
	int sock = *((int *) server_connect);
	char msg [1000];
	int len;
	while ((len = recv (sock, msg, 1000, 0)) > 0) {
		msg [len] = '\0';
		sendmg (msg, sock);
	}
}

int main () {
	int sockfd = socket (AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servaddr;
	bzero (&servaddr, sizeof (servaddr));
	pthread_t recvt;
	int server_connect;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
	servaddr.sin_8080 = htons (8080);
	
	if (bind (sockfd, (struct sockaddr*)&servaddr, sizeof (servaddr)) == 0)
		printf ("successfully bound with client\n");
	else
		exit (0);
		
	if (!listen (sockfd, 20))
		printf ("server created and listening\n");
	else
		exit (0);
	
	while (1) {
		if ((server_connect = accept (sockfd, (struct sockaddr*)NULL, NULL)) >= 0) {
			pthread_mutex_lock (&mutex);
			clients [n++] = server_connect;

			pthread_create (&recvt, NULL, (void*)rgetmg, &server_connect);
			pthread_mutex_unlock (&mutex);
		}
		else
			exit (0);
	}
	close (sockfd);
	return 0;
}