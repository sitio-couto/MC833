#include "client.h"

void receive_file(int, char*, char*);
void make_request(int);

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    int sockfd;
    // int numbytes;
    // char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }
    memset(&hints, 0, sizeof hints);
    // hints.ai_family = AF_UNSPEC;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);

    printf("client: connecting to %s\n", s);
    freeaddrinfo(servinfo); // all done with this structure
    make_request(sockfd);
    close(sockfd);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

void make_request(int socket) {
  char buffer[BUFFLEN];
  int msg_len;

  // receive server connection set confirmation
  msg_len = read(socket, buffer, BUFFLEN);
  buffer[msg_len] = '\0';
  printf("%s\n", buffer);


  while(1) {
    // Scan and send user request
    printf("awaiting input:\n");
    scanf(" %[^\n]", buffer);
    write(socket, buffer, strlen(buffer));

    // Await server commands
    switch (strtok(buffer, " ")[0]) {
      case '#':
        receive_file(socket, buffer, strtok(NULL, " "));
        break;
      default:
        printf("invalid option\n");
    }

  }

  return;
}

void receive_file(int socket, char *buffer, char *file_name) {

}
