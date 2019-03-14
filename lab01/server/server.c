#include "server.h"

#define SERV_PORT 12000

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
  sock_fd = socket(PF_INET, SOCK_STREAM, 0); //(IPV4, TCP, IP)
  if(sock_fd == -1) {
    fprintf(stderr, "Socker creatiion failed.\n");
    exit(0);
  }

  // Associate socket with the machine IP address
  if(bind(sock_fd, (SA*)&servaddr, sizeof(servaddr))) {
    fprintf(stderr, "Socket binding failed.\n");
    exit(0);
  }

  // Set up listening socket
  if(listen(sock_fd, LISTENQ)) {
    fprintf(stderr, "Listening failed.\n");
    exit(0);
  }

  while(1){
    clilen = sizeof(cliaddr);
    new_fd = accept(sock_fd, (SA*)&cliaddr, &clilen);
    if((childpid = fork()) == 0) { // If is child process
      close(sock_fd);              // Close listening socket
      // str_echo(new_fd);            // Process request
      exit(0);
    }
    close(new_fd);
  }

}
