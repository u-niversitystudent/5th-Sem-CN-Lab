// THIS FILE INCLUDES TWO PROGRAMMES (TWO DIFFERENT APPRAOCHES). 
// BOTH THE APPROACHES ARE EQUALLY BUGGED. TRY TO RESOLVE THE BUGS BY YOURSELF. USE BRUTE FORCE TO BYPASS IF REQUIRED

// Q. WRITE A SOCKET PROGRAM TO IMPLEMENT GO-BACK-N PROTOCOL USING TCP

// ############################# APPROACH 1 (CONVERTED FROM UDP TO TCP) ##########################

#include <stdio.h>									
#include <sys/socket.h>									
#include <arpa/inet.h>									
#include <stdlib.h>									
#include <string.h>									
#include <unistd.h>									
#include <errno.h>										
#include <memory.h>
#include <signal.h>										

struct gbnpacket {
	int type;
	int seq_no;
	int length;
	char data[100];
};

#define MAX 100
#define PORT 8080
#define lossRate 0.5
#define chunkSize 10

void CatchAlarm (int ignored) {
	exit (0);
}

int main () {
	int sock = socket (AF_INET, SOCK_STREAM, 0);			
	struct sockaddr_in gbnServAddr;
	if (sock == -1)
		exit (0)
	bzero (&gbnServAddr, sizeof (gbnServAddr));
	gbnServAddr.sin_family = AF_INET;
	gbnServAddr.sin_addr.s_addr = htonl (INADDR_ANY);
	gbnServAddr.sin_port = htons (PORT);
	if (bind (sock, (struct sockaddr *) &gbnServAddr, sizeof (gbnServAddr)) != 0)
		exit (0);
	struct sockaddr_in gbnClntAddr;						
	unsigned int cliAddrLen;							
	if (listen (sock, 5) != 0)
		exit (0);			
	int connfd = accept (sock, (struct sockaddr*) &gbnClntAddr, &cliAddrLen);
	if (connfd < 0)
		exit (0);

	char buffer[MAX+1];
	buffer[MAX] = '\0';
	bzero (buffer, MAX);
	int recvMsgSize;									
	int packet_rcvd = -1;							
	srand48(123456789);				
	struct sigaction myAction;
	myAction.sa_handler = CatchAlarm;
	if (sigfillset (&myAction.sa_mask) < 0) 			
		exit (0);
	myAction.sa_flags = 0;
	if (sigaction (SIGALRM, &myAction, 0) < 0)
		exit (0);
	while (1) {							
		cliAddrLen = sizeof (gbnClntAddr);
		struct gbnpacket currPacket; 						
		bzero (&currPacket, sizeof(currPacket));
		recvMsgSize = recv (connfd, &currPacket, sizeof (currPacket), 0);		
		currPacket.type = ntohl (currPacket.type);
		currPacket.length = ntohl (currPacket.length); 		
		currPacket.seq_no = ntohl (currPacket.seq_no);
		if (currPacket.type == 4) { 					
			printf ("%s\n", buffer);
			struct gbnpacket ackmsg;
			ackmsg.type = htonl(8);
			ackmsg.seq_no = htonl(0);				
			ackmsg.length = htonl(0);
			if (send (connfd, &ackmsg, sizeof (ackmsg), 0) != sizeof (ackmsg))	
				exit (0);	
			alarm (7);
			while (1) {
				while ((recv (connfd, &currPacket, sizeof (int)*3+chunkSize, 0)) < 0)	
					if (errno == EINTR)					
						exit (0);					
				if (ntohl(currPacket.type) == 4) { 			
					ackmsg.type = htonl(8);
					ackmsg.seq_no = htonl(0);				
					ackmsg.length = htonl(0);
					if (send (connfd, &ackmsg, sizeof (ackmsg), 0) != sizeof (ackmsg))	
						exit (0);
				}
			}
			exit (0);
		}
		else {
			if(lossRate > drand48())
				continue; 									
			printf ("RECEIVE PACKET %d length %d\n", currPacket.seq_no, currPacket.length);
			if (currPacket.seq_no == packet_rcvd + 1) {
				packet_rcvd++;
				int buff_offset = chunkSize * currPacket.seq_no;
				memcpy (&buffer[buff_offset], currPacket.data, currPacket.length);
			}
	  
			printf ("SEND ACK %d\n\n", packet_rcvd);
			struct gbnpacket currAck; 						
			currAck.type = htonl (2); 					
			currAck.seq_no = htonl (packet_rcvd);
			currAck.length = htonl(0);
			if (send (connfd, &currAck, sizeof (currAck), 0) != sizeof (currAck))
				exit (0);
		}
    }
	
	printf ("SERVER EXIT\n");
	close (sock);
	return 0;
}





// ################### APPROACH 2 (COMPARITIVELY LESS BUGGED THAN APPROACH 1, BUT NOT AS GOOD A PROGRAMME AS APPROACH 1) ###########################
// TRY TO UNDERSTAND AND RENAME VARIABLES FOR THIS APPROACH FROM THE APPROACH 1.

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <unistd.h>

#define W 5
#define P1 50
#define P2 10

char a [10];
char b [10];
void alpha9 (int);

int main () {
    struct sockaddr_in ser, cli;
    int s, n, sock, i, j, c=1, f;
    unsigned int s1;
    
	s = socket (AF_INET,SOCK_STREAM,0);
    ser.sin_family = AF_INET;
    ser.sin_port = 6500;
    ser.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(s, (struct sockaddr *) &ser, sizeof(ser));
    listen (s, 1);
    n = sizeof (cli);
    sock = accept (s, (struct sockaddr *)&cli, &n);
    printf ("\nTCP Connection Established.\n");
    
	s1 = (unsigned int) time(NULL);
    srand (s1);
    strcpy (b,"Time Out ");
    recv (sock, a, sizeof(a), 0);
    f = atoi(a);
    
	while (1) {
        for(i=0;i<W;i++) {
            recv(sock,a,sizeof(a),0);
            if(strcmp(a,b)==0) {
                break;
            }
        }
        i=0;
        while(i<W) {
            j=rand()%P1;
            if(j<P2) {
                send(sock,b,sizeof(b),0);
                break;
            }
            else {
                alpha9(c);
                if(c<=f+1) {
                    printf("\nFrame %s Received ",a);
                    send(sock,a,sizeof(a),0);
                }
                else{
                    break;
                }
                c++;
            }
            if(c>f) {
                break;
            }
            i++;
        }
    }
    close(sock);
    close(s);
    return 0;
}

void alpha9 (int z) {
    int k, i=0, j, g;
    k = z;
    while (k > 0) {
        i++;
        k = k/10;
    }
    g = i;
    i--;
    while (z > 0) {
        k = z%10;
        a [i] = k+48;
        i--;
        z = z/10;
    }
    a [g] = '\0';
}
