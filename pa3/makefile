all: HangmanServer HangmanClient

HangmanServer: hangman_server.c
	gcc -o HangmanServer hangman_server.c

HangmanClient: hangman_client.c
	gcc -o HangmanClient hangman_client.c

clean:
	rm HangmanServer
	rm HangmanClient
