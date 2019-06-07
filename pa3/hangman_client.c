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
#include <ctype.h>
#define MAXLINE 1024 

// Driver code 
int main(int argc, char* argv[]) {

	int port = atoi(argv[2]);	
	int sockfd; 
	char buffer[MAXLINE];  
	struct sockaddr_in	 servaddr; 
	struct hostent *host_entry;
	struct timeval timeout={1,0};

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


	
	
	//servaddr.sin_addr.s_addr = INADDR_ANY; 
	
	int n, len; 
	

	
	char *message = "";
	char word[MAXLINE];
	char guessWord[] = "________";
	int gameSuccess = 0;
	int guesses = 0;
	int wordLength = 0;	
	char userInput[1];
	printf("Ready to start game? (y/n):\n");
	scanf("%s",userInput);
	if(strcmp(userInput,"y") == 0)
	{
		sendto(sockfd, (const char *)message, strlen(message),
                MSG_CONFIRM, (const struct sockaddr *) &servaddr,
                        sizeof(servaddr));
	        //printf("empty message sent.\n");
			
		
                n = recvfrom(sockfd, (char *)word, MAXLINE,
                                MSG_WAITALL, ( struct sockaddr *) &servaddr,
                                &len);
                word[n] = '\0';
		wordLength = word[1] - '0';
		//printf("server:%c \n",word[0]);


		for(int i = 0; i < 8; i++)
		{
			guessWord[i] = word[i + 3];
		}

        	printf("%s\n",guessWord);

		guesses = 0;
		gameSuccess = word[0] - '0';
		
		char incorrectGuesses[] = "Incorrect Guesses:       ";
		int k = 0;
		while( (guesses < 6) || (gameSuccess == 0))
		{
			printf("Letter to guess:");
			scanf("%s",userInput);
			char *guess = userInput;
			int length = strlen(guess);
			if(isdigit(userInput[0]) || length != 1)
			{
				printf("Error! Please guess one letter.\n");
			}
			else
			{
				guess[0] = tolower(guess[0]);
				
				sendto(sockfd, (const char *)guess, strlen(guess),
                		MSG_CONFIRM, (const struct sockaddr *) &servaddr,
                        		sizeof(servaddr));
        			//printf("Guess Sent.\n");
				
				n = recvfrom(sockfd, (char *)word, MAXLINE,
                                MSG_WAITALL, ( struct sockaddr *) &servaddr,
                                &len);
                		word[n] = '\0';
				//printf("%s\n",word);	
				
				if(word[0] == '[')
				{
					printf("YouWin!\n");
					close(sockfd);
					return 0;
				}

				
				if(strlen(word) == 1)
				{
					incorrectGuesses[18 + k] = word[0];
					k++;
					guesses++;			
				}
				else{		
				for(int i = 0; i < 8; i++)
                		{
                		        guessWord[i] = word[i + 3];
        		        }
				}
	
                		printf("%s\n",guessWord);

				printf("%s\n",incorrectGuesses);

				
			}
		}
		if(guesses == 6)
		{
			printf("YouLose!!\n");
		}
		

		close(sockfd);
	}
	else
	{
		close(sockfd);
	}

	printf("GameOver!\n");

	return 0; 
} 

