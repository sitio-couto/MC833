
// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
#define BUFFLEN 256
#define PORT    8080 
#define MAXLINE 1024 

// Debuggin wrapper for send
int write_udp(int socket, char *buffer, int length, struct sockaddr_in target){
  int i, r_val;

  // Fill message to standard size of buffer
  for (i = length; i < BUFFLEN; ++i) buffer[i] = '\0';

  if ((r_val = sendto(socket, (const char*)buffer, BUFFLEN, MSG_CONFIRM, (const struct sockaddr*)&target, sizeof(struct sockaddr))) == -1) {
    perror("ERROR: send");
    exit(1);
  } else if (r_val == 0) {
    printf("ERROR: pairing socket is closed\n");
    exit(1);
  }

  return r_val;
}

// Debuggin wrapper for recv
int read_udp(int socket, char *buffer, struct sockaddr_in *sender, int* sender_len) {
  int r_val, total = 0;

  while (total != BUFFLEN) {
    if ((r_val = recvfrom(socket, &buffer[total], (BUFFLEN - total), MSG_WAITALL,
                          (struct sockaddr *)sender, sender_len)) == -1) {
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
int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = "Hello from server"; 
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
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
    
    
    n = read_udp(sockfd, buffer, &cliaddr, &len);          
    buffer[n] = '\0'; 
    printf("Client : %s\n", buffer); 
    
    strcpy(buffer, "Wrappers are go (from server).");
    len = strlen(buffer);
    write_udp(sockfd, buffer, len, cliaddr);
    printf("Hello message sent.\n");  
      
    return 0; 
} 
