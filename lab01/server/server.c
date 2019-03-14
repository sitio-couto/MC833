#include "server.h"

int main(int argc, char *argv){
  struct sockaddr_in cliaddr, servaddr;
  int sock_fd, new_fd;  // listening socket and new connection Socket
  pid_t childpid;
  socklen_t clilen;

  bzero(&servaddr, sizeof(servaddr));           // Initialize servaddr with bits 0
  servaddr.sin_family = AF_INET;                // Set to IPV4
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Attribute localhost
  servaddr.sin_port = htons(SERV_PORT);         // Attribute port

  // Create and check listening socket
  sock_fd = socket(PF_INET, SOCK_STREAM, 0) //(IPV4, TCP, IP)
  if(sock_fd == -1) {
    fprintf(stderr, "Socker creatiion failed.\n");
    exit(0);
  }

  // Associate socket with the machine IP address
  if(bind(sock_fd, (SA*)&servaddr, sizeof(servaddr))) {
    fprintf(stderr, "Socket binding failed.\n");
    exit(0);
  }

  
}
