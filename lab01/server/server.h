#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define BUFFLEN 256     // Length of the message buffer
#define PORT 3490       // the port users will be connecting to
#define BACKLOG 10      // how many pending connections queue will hold
#define MAXDATASIZE 100 // max number of bytes we can send at once

// Debuggin wrapper for send
int write_d(int socket, char *buffer, int length){
  int i, r_val;

  // Fill message to standard size of buffer
  for (i = length; i < BUFFLEN; ++i) buffer[i] = '\0';

  if ((r_val = send(socket, buffer, BUFFLEN, 0)) == -1) {
    perror("ERROR: send");
    exit(1);
  } else if (r_val == 0) {
    printf("ERROR: pairing socket is closed\n");
    exit(1);
  }

  return r_val;
}

// Debuggin wrapper for recv
int read_d(int socket, char *buffer) {
  int r_val, total = 0;

  while (total != BUFFLEN) {
    if ((r_val = recv(socket, &buffer[total], (BUFFLEN - total), 0)) == -1) {
      perror("ERROR: send");
      exit(1);
    } else if (r_val == 0) { // if client not responding
      printf("ERROR: pairing socket is closed\n");
      exit(1);
    } else {
      total += r_val;
    }
  }

  return total;
}
