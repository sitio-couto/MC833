# variables
CC=gcc
RM=rm -f

all: servers client

servers: server/server_tcp.c server/server_udp.c
	$(CC) -o server/server_udp server/server_udp.c
	$(CC) -o server/server_tcp server/server_tcp.c

client: client/client.c
	$(CC) -o client/client client/client.c

clean:
	$(RM) server/server_udp server/server_tcp
	$(RM) client/client
