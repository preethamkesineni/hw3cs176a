all: PingClient

PingClient: PingClient.c
	gcc -o PingClient PingClient.c
clean:
	rm PingClient
