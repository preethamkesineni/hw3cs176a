// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <netdb.h>

#define MAXLINE 1024 

// Driver code 
int main(int argc, char* argv[]) { 
	int sockfd;

	int port = atoi(argv[2]);
	char buffer[MAXLINE]; 
	char *message; 
	struct sockaddr_in	 servaddr; 
	struct timeval timeout={1,0};
	struct hostent *host_entry;

		
	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	

	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(port); 
	
	if(argv[1][0] == 'c')
	{
		host_entry = (struct hostent*)gethostbyname(argv[1]);
		memcpy(&servaddr.sin_addr, host_entry->h_addr_list[0], host_entry->h_length);
	}
	else{
		servaddr.sin_addr.s_addr = inet_addr(argv[1]); 
	}
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval));	
	
	int n, len; 
	int packetsReceived = 0;
	double rttMin = 100000;
	double rttMax = 0;
	double rttAvg = 0;
	int percentLoss = 0;


	for(int i = 1; i < 11; i++)
	{
		sleep(1);
		message = "fuckmylife";
		sendto(sockfd, (const char *)message, strlen(message), 
                MSG_CONFIRM, (const struct sockaddr *) &servaddr,
                        sizeof(servaddr));
        	//printf("fuckeverythingrightnow.\n");
		clock_t begin = clock();
		
		 n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                                MSG_WAITALL, (struct sockaddr *) &servaddr,
                                &len);
                        buffer[n] = '\0';
		
		clock_t end = clock();

		double timeElapsed = (end - begin)/(double)(CLOCKS_PER_SEC/1000); 
		if (n>= 0) {
			if(rttMin > timeElapsed)
			{
				rttMin = timeElapsed;
			}
			if(rttMax < timeElapsed)
			{
				rttMax = timeElapsed;
			}

			rttAvg += timeElapsed;

			packetsReceived++;
        	        printf("PING received from ");
			printf(argv[1]);
			printf(" seq#=%d",i);
			printf(" time=%fms\n",timeElapsed);
		
		}
		else{
			printf("Request timeout for icmp_seq %d\n",i);
		}	
                    
		
	}

	rttAvg = rttAvg/packetsReceived;
	percentLoss = (10 - packetsReceived) * 10;

	printf("--- ping statistics --- 10 packages transmitted, %d received,",packetsReceived);
       	printf("%d%% packet loss rtt min/avg/max = ",percentLoss); 
	printf("%f ",rttMin);
	printf("%f ",rttAvg);
	printf("%f ms\n",rttMax);
	

	close(sockfd); 
	return 0; 
} 

