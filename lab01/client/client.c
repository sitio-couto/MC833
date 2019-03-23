#include "client.h"

void receive_file(int, char*, char*);
void make_request(int);

int main(int argc, char *argv[])
{
    int sockfd, rv;
    struct addrinfo hints, *p, *servers;
    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servers)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servers; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    printf("client: connecting...\n");
    freeaddrinfo(servers); // all done with this structure
    make_request(sockfd);
    close(sockfd);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

void make_request(int socket) {
  char buffer[BUFFLEN];
  int msg_len;

  // receive server connection set confirmation
  msg_len = read_d(socket, buffer, BUFFLEN);
  buffer[msg_len] = '\0';
  printf("%s\n", buffer);


  while(1) {
    // Scan and send user request
    printf("awaiting input:\n");
    scanf(" %[^\n]", buffer);
    write_d(socket, buffer, strlen(buffer));

    // Await server commands
    switch (strtok(buffer, " ")[0]) {
      case '#':
        printf("awaiting file...\n");
        receive_file(socket, buffer, strtok(NULL, " "));
        break;
      default:
        printf("invalid option\n");
    }

  }

  return;
}

void receive_file(int socket, char *buffer, char *file_name) {
  FILE *output = fopen(file_name, "w"); // create file with same name to write
  int msg_len;

  while (buffer[0] != '\0') { // iterate through packets until EOF
    msg_len = read_d(socket, buffer, BUFFLEN);  // read buffer
    buffer[msg_len] = '\0';                   // add eof for string handling
    fputs(buffer, output);                    // write buffer
  }

  printf("file received\n");
  fclose(output);
  return;
}
