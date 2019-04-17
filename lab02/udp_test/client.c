// Client side implementation of UDP client-server model 
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
#include <time.h>
#include <sys/time.h>

#define BUFFLEN 256
#define PORT "8080"
#define MAXLINE 1024 

clock_t start, end;
double cpu_time_used;
int time_count = 0;
char* time_path;
FILE* time_output;

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

// Debuggin wrapper for recv
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

// Driver code 
int main(int argc, char *argv[]) { 
    int sockfd, rv; 
    char buffer[MAXLINE]; 
    struct addrinfo hints, *p, *servers;
    struct sockaddr *servaddr; 
  
    if (argc < 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    if (argc == 3) {
      time_path = argv[2];
      time_output = fopen(time_path, "w");
    }

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servers)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and use the first we can
    for(p = servers; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, 0)) == -1) {
            perror("client: socket");
            continue;
        } else break;
    }

    // Filling server information 
    servaddr = p->ai_addr;

    int n, len; 
    
    strcpy(buffer,"Wrappers full power (from client)");
    len = strlen(buffer);
    write_udp(sockfd, buffer, len, servaddr);
    n = read_udp(sockfd, buffer, servaddr, &len);          
    buffer[n] = '\0';
    printf("Server : %s\n", buffer); 
  
    close(sockfd); 
    return 0; 
} 
