# variables
CC=gcc -o
RM=rm -f

all: server_udp server_tcp client_dual

server_udp: server/server_udp.c
	$(CC) server/server_udp server/server_udp.c

server_tcp: server/server_tcp.c
	$(CC) server/server_tcp server/server_tcp.c

client_dual: client/client.c
	$(CC) client/client client/client.c

clean:
	$(RM) server/server_udp 
	$(RM) server/server_tcp
	$(RM) client/client
