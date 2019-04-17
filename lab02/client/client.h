#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>

#define BUFFLEN 256 // Length of the message buffer
#define TCP_PORT "3490" // the port client will be connecting to
#define UDP_PORT "8080"

// Funcions signatures
void receive_file(int, char*, char*);
void make_request(int);
char* get_name(char*);
void send_file(int, char*, char*);
void receive_data(int, char*);
char* get_path(char*);

// TCP WRAPPERS ////////////////////////////////////////////////////////////////
// Debuggin wrapper for send
int write_d(int socket, char *buffer, int length){
  int i, r_val;

  // Fiil message to standard size of buffer
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

// UDP SEND AND RECEIVE WRAPPERS ///////////////////////////////////////////////
// Debuggin wrapper for sendto
int write_udp(int socket, char *buffer, int length, struct sockaddr *target){
  int i, r_val;

  // Fill message to standard size of buffer
  for (i = length; i < BUFFLEN; ++i) buffer[i] = '\0';

  if ((r_val = sendto(socket, (const char*)buffer, BUFFLEN, MSG_CONFIRM, target, sizeof(struct sockaddr))) == -1) {
    perror("ERROR: send");
    exit(1);
  } else if (r_val == 0) {
    printf("ERROR: pairing socket is closed\n");
    exit(1);
  }

  return r_val;
}

// Debuggin wrapper for recvfrom
int read_udp(int socket, char *buffer, struct sockaddr *sender, int* sender_len) {
  int r_val, total = 0;

  while (total != BUFFLEN) {
    if ((r_val = recvfrom(socket, &buffer[total], (BUFFLEN - total), MSG_WAITALL,
                          sender, sender_len)) == -1) {
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
