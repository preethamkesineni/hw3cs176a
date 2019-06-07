// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <time.h>
#define PORT	 8080 
#define MAXLINE 1024 

// Driver code 

char* choose_random_word(const char *filename) {
    FILE *f;
    size_t lineno = 0;
    size_t selectlen;
    char selected[256]; /* Arbitrary, make it whatever size makes sense */
    char current[256];
    selected[0] = '\0'; /* Don't crash if file is empty */

    f = fopen(filename, "r"); /* Add your own error checking */
    int randomLine = rand() % 15;
    //printf("%d\n",randomLine);
    while (fgets(current, sizeof(current), f)) {
        if ( randomLine == lineno) {
            strcpy(selected, current);
	    lineno++;
        }
	else
	{
		lineno++;
	}
    }
    fclose(f);
    selectlen = strlen(selected);
    if (selectlen > 0 && selected[selectlen-1] == '\n') {
        selected[selectlen-1] = '\0';
    }
    return strdup(selected);
}

int main() {
       	srand(time(0));	
	int sockfd; 
	char buffer[MAXLINE]; 
	char* word = "        ";
	struct sockaddr_in servaddr, cliaddr;
		
	
	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// Bind the socket with the server address 
	
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
			sizeof(servaddr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	
	int len, n; 

	n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
				MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
				&len); 
	buffer[n] = '\0'; 
	
	//char* message = "applesauce";

     	//sendto(sockfd, (const char *)message, strlen(message),  
        //MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
        //    len); 
	
	char* wordToGuess = choose_random_word("hangman_words.txt");
	printf("%s\n",wordToGuess);	
	int wordLength = strlen(wordToGuess);
	wordLength = wordLength + 3;
	char message[] = "           ";
	int guesses = 0;
	
	int asciiWordlength = strlen(wordToGuess) + 48;
	
	message[0] = '0';
	message[1] = (char )(asciiWordlength);
	message[2] = (char )48;
	
	//printf("%s/n",message);
	
	for(int i = 3; i < wordLength;i++)
	{
		message[i] = (char )95;
	}
	message[wordLength] = '\0';
	

	for(int i = 0; i < wordLength;i++)
	{	
		//printf("%c",message[i]);
		//word[i] = message[i];
		
	}
	printf("\n");
	

	//word[wordLength] = '\0';

	printf("%s\n",message);

	sendto(sockfd, (const char *)message, sizeof(message),
                MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                        len);
	//char* incorrect = "   Incorrect\n";
	int correct = 0;
	int fullWord = strlen(wordToGuess);
	int succ = 0;
	while(1)
	{
		correct = 0;
		int len, n;
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                                MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                                &len);
                        buffer[n] = '\0';
                        printf("Client: Gussed %s\n", buffer);
	//	if(buffer[0] == '8');
	//	{
	//		char* gameOver = "8GameOver!";
	//		sendto(sockfd, (const char *)gameOver, sizeof(gameOver),
          //              MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            //            len);
		
	//	}

		for(int i = 0; i < strlen(wordToGuess); i++)
		{
			if(wordToGuess[i] == buffer[0])
			{
				message[i+3] = buffer[0];
				correct = 1;
				succ++;	
			}
		}
		if(succ == fullWord)
		{
			char* win = "[";
			sendto(sockfd, (const char *)win, sizeof(win),
                        MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                        len);

		}

		if (correct){
			sendto(sockfd, (const char *)message, sizeof(message),
                        MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                        len);
		}
		else
		{
			sendto(sockfd, (const char *)buffer, MAXLINE,
                        MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                        len);
		}
	}



	return 0; 
} 

