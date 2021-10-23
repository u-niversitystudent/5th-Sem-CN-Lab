// THERE ARE TWO DIFFERENT PROGRAMMES (TWO DIFFERENT APPROACHES) INCLUDED IN THIS SAME FILE
// BOTH THE PROGRAMS ARE EQUALLY BUGGED. PLEASE TRY TO RESOLVE ISSUES BY YOURSELF. IF NOT, USE BRUTE FORCE TO BYPASS THE BUGS AND PRINT DIRECT ANSWERS

// Q. WRITE A SOCKET PROGRAM TO IMPLEMENT GO-BACK-N PROTOCOL USING TCP

// THIS IS CLIENT SIDE PROGRAM

// ############## APPROACH 1 (CONVERTED FROM UDP TO TCP) ###################### 

#include <stdio.h>		 	
#include <sys/socket.h>		
#include <arpa/inet.h>		
#include <stdlib.h>			
#include <string.h>			
#include <unistd.h>			
#include <errno.h>			
#include <signal.h>			

struct gbnpacket {
	int type;
	int seq_no;
	int length;
	char data[100];
};

#define TIMEOUT_SECS 3										
#define MAXTRIES 10										
#define MAX 100
#define PORT 8080

int tries = 0;												
int base = 0;
int windowSize;
int sendflag = 1;
int chunkSize = 10;

void CatchAlarm (int ignored) {
	tries += 1;
	sendflag = 1;
}

int max (int a, int b) {
	if (b > a)
		return b;
	return a;
}

int min(int a, int b) {
	if(b>a)
		return a;
	return b;
}

int main () {
	
	int sock = socket (AF_INET, SOCK_STREAM, 0);					
	struct sockaddr_in gbnServAddr;
	if (sock == -1)
		exit (0);
	bzero (&gbnServAddr, sizeof (gbnServAddr));
	gbnServAddr.sin_family = AF_INET;
	gbnServAddr.sin_addr.s_addr = htonl (INADDR_ANY);
	gbnServAddr.sin_port = htons (PORT);						
	if (connect (sock, (struct sockaddr*) &gbnServAddr, sizeof (gbnServAddr)) != 0)
			exit (0);

	char buffer[MAX];
	printf ("Please Enter your data to transmit: ");
	int i;
	for (i=0; (buffer [i] = getchar()) != '\n'; ++i);
	buffer [i] = '\0';
	printf ("Enter Window Size: ");
	scanf (" %d", &windowSize);
	const int datasize = MAX;													
	int nPackets = datasize / chunkSize;											
	if (datasize % chunkSize)
		nPackets++;
	struct sockaddr_in fromAddr;													
	unsigned int fromSize;															
	int respLen;																
	int packet_received = -1;														
	int packet_sent = -1;															  
	struct sigaction myAction;														
	myAction.sa_handler = CatchAlarm;
	if (sigfillset (&myAction.sa_mask) < 0)									
		exit (0);
	myAction.sa_flags = 0;
	if (sigaction (SIGALRM, &myAction, 0) < 0)
		exit (0);
	while ((packet_received < nPackets-1) && (tries < MAXTRIES)) {
		if (sendflag > 0) {
			sendflag = 0;
			int ctr; 																
			for (ctr = 0; ctr < windowSize; ctr++) {
				packet_sent = min(max (base + ctr, packet_sent),nPackets-1); 
				struct gbnpacket currpacket; 				
				if ((base + ctr) < nPackets) {
					bzero (&currpacket, sizeof(currpacket));
					printf ("SENDING PACKET %d PACKET SENT %d PACKET RECEIVED %d\n", base+ctr, packet_sent, packet_received);

					currpacket.type = htonl (1); 				
					currpacket.seq_no = htonl (base + ctr);
					int currlength;
					if ((datasize - ((base + ctr) * chunkSize)) >= chunkSize) 	
						currlength = chunkSize;
					else
						currlength = datasize % chunkSize;
					currpacket.length = htonl (currlength);
					memcpy (currpacket.data, 					
					buffer + ((base + ctr) * chunkSize), currlength);
					if (send (sock, &currpacket, (sizeof (int) * 3) + currlength, 0) != ((sizeof (int) * 3) + currlength))																	
						exit (0);
				}
			}
		}
		fromSize = sizeof (fromAddr);
		alarm (TIMEOUT_SECS);														
		struct gbnpacket currAck;
		while ((respLen = (recv (sock, &currAck, sizeof (int) * 3, 0))) < 0)																
			if (errno == EINTR)	{												
				if (tries < MAXTRIES) {											
					printf ("TIMED OUT, %d TRIES REMAIN\n", MAXTRIES - tries);
					break;
				}
				else
					exit (0);
			}
			else
				exit (0);
			if (respLen) {
				int acktype = ntohl (currAck.type); 								
				int ackno = ntohl (currAck.seq_no); 
				if (ackno > packet_received && acktype == 2) {
					printf ("RECEIVED ACK\n\n"); 								
					packet_received++;
					base = packet_received; 									
					if (packet_received == packet_sent) {					
						alarm (0); 													
						tries = 0;
						sendflag = 1;
					}
					else {															
						tries = 0; 													
						sendflag = 0;
						alarm(TIMEOUT_SECS); 										
					}
				}
			}
    }
	
	int ctr;
	for (ctr = 0; ctr < 10; ctr++) {											
		struct gbnpacket teardown;
		teardown.type = htonl (4);
		teardown.seq_no = htonl (0);
		teardown.length = htonl (0);
		send (sock, &teardown, (sizeof (int) * 3), 0);												
    }

	close (sock); 																
	return 0;
}






// ################### APPROACH 2 (COMPARITIVELY LESS BUGGED THAN APPROACH 1, BUT NOT AS GOOD A PROGRAMME AS APPROACH 1) ###########################
// IN THIS APPROACH, AFTER COMPILING, RUN AS ./client <Frame Input>
// <Frame Input> is a int type argument. It is number of Data Packets to Transmit. For Example. ./client 5
// TRY TO UNDERSTAND AND RENAME VARIABLES FOR THIS APPROACH FROM THE APPROACH 1.

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<ctype.h>
#include<unistd.h>
#include<arpa/inet.h>

#define W 5

char a[10];
char b[10];

void alpha9(int);

int main (int argc, char *argv[]) {
    int s, f, wl, c = 1, x, i = 0, j, n, p = 0, e = 0;
    struct sockaddr_in ser;
    s = socket(AF_INET,SOCK_STREAM,0);
    ser.sin_family = AF_INET;
    ser.sin_port = 6500;
    ser.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect (s, (struct sockaddr *) &ser, sizeof(ser));
    printf ("\nTCP Connection Established.\n");
    f = atoi (argv[1]);
    alpha9 (f);
    send (s, a, sizeof(a), 0);
    strcpy (b, "Time Out ");
    while (1) {
        for (i = 0; i < W; i++) {
            alpha9 (c);
            send (s, a, sizeof(a), 0);
            if (c <= f) {
                printf ("\nFrame %d Sent", c);
                c++;
            }
        }
        i = 0;
        wl = W;
        while (i < W) {
            recv (s, a, sizeof (a), 0);
            p = atoi (a);
            if (strcmp (a,b) == 0) {
                e = c - wl;
				if (e < f) {
					printf("\nTime Out, Resent Frame %d onwards",e);
				}
				break;
            }
            else {
                if(p<=f) {
                    printf ("\nFrame %s Acknowledged", a);
                    wl--;
                }
                else {
                    break;
                }
            }
            if (p > f) {
                break;
            }
            i++;
        }
        if (wl == 0 && c > f) {
            send (s, b, sizeof(b), 0);
            break;
        }
        else {
            c=c-wl;
            wl=W;
        }
    }
    close(s);
    return 0;
}

void alpha9(int z) {
    int k,i=0,j,g;
    k=z;
    while(k>0) {
        i++;
        k=k/10;
    }
    g=i;
    i--;
    while(z>0) {
        k=z%10;
        a[i]=k+48;
        i--;
        z=z/10;
    }
    a[g]='\0';
}
