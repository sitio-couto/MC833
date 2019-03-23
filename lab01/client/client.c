#include "client.h"

void receive_file(int, char*, char*);
void make_request(int);
char* get_name(char*);

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

  // receive server connection set confirmation
  read_d(socket, buffer);
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
  FILE *output;
  int msg_len;
  long int i = 0, base, size;

  output = fopen(get_name(file_name), "wb"); // create file with same name to write
  printf("receving file \"%s\"...\n", get_name(file_name));

  read_d(socket, buffer);           // Read size
  size = strtol(buffer, NULL, 10);  // Cast size to long int

  while (i < size) {
    msg_len = read_d(socket, buffer);        // Read a full buffer.
    for (base = i; (i < base + BUFFLEN) && (i < size); ++i) // Write elements
      fputc(buffer[i%BUFFLEN], output);
  }

  printf("file received\n");
  fclose(output);
  return;
}

char* get_name(char *file_name) {
  int i;

  for(i = strlen(file_name); i >= 0; --i)
    if (file_name[i] == '/') return &(file_name[i+1]);

  return file_name;
}
