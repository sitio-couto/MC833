#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

int main(int argc, char *argv){
  int socketfd, new_socket, status;
  struct addrinfo hints;
  struct addrinfo *servinfo;

  int l, next;

  menset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  if (!(status = getaddrinfo(NULL, "3490", &hints, &servinfo))) {
    printf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  } else {
    l = servinfo;
    while (l) {
      printf("Canonical name: %s\n", l.ai_canonname);

    }
  }

  socketfd = socket(PF_INET, SOCK_STREAM, 0);


  if (!socketfd) {
    printf("erro\n");
  } else {
    printf("Sockerfd = %u\n", &socketfd);
  }


}
