#include "server.h"
void request_options(int);
void get_profile(char*);
void send_file(int, char*, char*);
int get_path(char*, char*);

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa){

  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void){
  int sockfd, new_fd, pid;  // listen on sock_fd, new connection on new_fd
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr; // connector's address information
  socklen_t sin_size;
  struct sigaction sa;
  int yes=1;
  char s[INET6_ADDRSTRLEN];
  int rv;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      return 1;
  }
  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
      if ((sockfd = socket(p->ai_family, p->ai_socktype,
              p->ai_protocol)) == -1) {
          perror("server: socket");
          continue;
      }
      if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
              sizeof(int)) == -1) {
          perror("setsockopt");
          exit(1);
      }
      if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
          close(sockfd);
          perror("server: bind");
          continue;
      }
    break;
  }

  freeaddrinfo(servinfo); // all done with this structure
  if (p == NULL)  {
      fprintf(stderr, "server: failed to bind\n");
      exit(1);
  }
  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }

  sa.sa_handler = sigchld_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
      perror("sigaction");
      exit(1);
  }

  printf("server: waiting for connections...\n");
  while(1) {  // main accept() loop
      sin_size = sizeof their_addr;
      new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
      if (new_fd == -1) {
          perror("accept");
          continue;
      }
      inet_ntop(their_addr.ss_family,
          get_in_addr((struct sockaddr *)&their_addr),
          s, sizeof s);
      printf("server: got connection from %s\n", s);
      if (!fork()) { // this is the child process
          close(sockfd); // child doesn't need the listener

          request_options(new_fd); // Communication function

          close(new_fd);
          exit(0);
      }
      close(new_fd);  // parent doesn't need this
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////

void request_options(int socket) {
  char buffer[BUFFLEN];
  int msg_len; // TODO: set same message size for client and server

  // notify connections is set
  write_d(socket, "connection is set...", 20);

  // get_profile("maria@unicamp.br");

  while(1){
    // Await new message from client
    printf("server awaiting new message...\n");
    msg_len = read_d(socket, buffer, BUFFLEN);
    buffer[msg_len] = '\0';   // Adjust EOF to the received msg size

    // Test which request the client aksed for
    switch (strtok(buffer, " ")[0]) {
      case '#':
        printf("sending file...\n");
        send_file(socket, buffer, strtok(NULL, " "));
        break;
      default:
        printf("invalid option\n");
    }

    // End connection if requested by client
    if (!strcmp(buffer, "exit")) break;
  }

  return;
}

void names_by_course(char* course) {

  return;
}

void habilities_by_city(char* city) {

  return;
}

void add_experience(char* email, char* experience) {

  return;
}

void get_experience(char* email) {

  return;
}

void get_profile(char* email) {

  char c, file[80];
  FILE *fptr;

  strcpy(file, "server/data/");
  strcat(file, email);
  strcat(file, ".txt");

  if ((fptr = fopen(file,"r")) == NULL){
      printf("Error! opening file: %s", file);
      // Program exits if the file pointer returns NULL.
      exit(1);
  }

  // Read contents from file
  c = fgetc(fptr);
  while (c != EOF){
    printf("%c", c);
    c = fgetc(fptr);
  }

  return;
}

// This function splits files from data and send the to the client
void send_file(int socket, char *buffer, char *file_name) {
  FILE *input;        // File to be sent
  char path[BUFFLEN]; // buffer to place the absolute path

  get_path(path, file_name);
  input = fopen(path, "r");

  while (fgets(buffer, BUFFLEN, input)) // reads file filling buffer
    write_d(socket, buffer, BUFFLEN);     // sends filled up buffer to client
  write_d(socket, "\0", 1);               // notify client that the file has ended

  printf("file sent\n");
  fclose(input);
  return;
}

// Gets the absolute path of the file
int get_path(char *pBuf, char *file_name) {
  char szTmp[32];
  int bytes;

  sprintf(szTmp, "/proc/%d/exe", getpid());
  bytes = readlink(szTmp, pBuf, BUFFLEN);

  for (bytes ; pBuf[bytes] != '/'; --bytes);
  pBuf[bytes] = '\0';

  strcat(pBuf, "/data/");
  strcat(pBuf, file_name);

  return bytes;
}
