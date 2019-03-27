#include "server.h"
void request_options(int);
void get_profile(int, char*, char*);
void send_file(int, char*, char*);
char* get_path(char*);
char* get_name(char*);
void send_data(int, char*, int);
void receive_file(int, char*, char*);

int main(void){
  int sockfd, new_fd, pid;  // listen on sock_fd, new connection on new_fd
  struct sockaddr_in server, client;
  int sin_size;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("gateway socket");
    exit(1);
  }

  memset(&server, 0, sizeof server);
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1) {
      perror("server: bind");
      exit(1);
  }

  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }

  printf("server: waiting for connections...\n");
  while(1) {  // main accept() loop
      sin_size = sizeof client;
      new_fd = accept(sockfd, (struct sockaddr *)&client, &sin_size);
      if(new_fd == -1) {
          perror("accept");
          exit(1);
      }

      printf("server: got connection\n");
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

  // notify connections is set
  strcpy(buffer, "connection is set...");
  write(socket, buffer, strlen(buffer));

  while(1){
    // Await new message from client
    printf("server awaiting new message...\n");
    read_d(socket, buffer);

    // Test which request the client aksed for
    switch (strtok(buffer, " ")[0]) {
      case '#':
        printf("sending file...\n");
        send_file(socket, buffer, strtok(NULL, " "));
        break;
      case '6': // Get full profile
        printf("retrieving profile...\n");
        get_profile(socket, buffer, strtok(NULL, " "));
        printf("profile sent.\n");
        break;
      default:
        printf("invalid option\n");
    }

    // End connection if requested by client
    if (!strcmp(buffer, "exit")) break;
  }

  return;
}

// OPTIONS FUNCTIONS ///////////////////////////////////////////////////////////

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

void get_profile(int socket, char* buffer, char *buff_email) {

  char c, email[BUFFLEN];
  FILE *fptr;

  strcpy(email, buff_email); // Copy email key from buffer

  // Gets image form server

  strcat(strcat(strcat(get_path(buffer), "data/images/"), email), ".jpg");
  send_file(socket, buffer, buffer);

  // Gets the values in the txt file

  strcat(strcat(strcat(get_path(buffer), "data/"), email), ".txt");

  if ((fptr = fopen(buffer,"r")) == NULL){
      printf("Error! opening file: %s\n", buffer);
      exit(1); // Exits if failed to open file
  }

  // Send contents from file
  while (fgets(buffer, BUFFLEN, fptr))
    write_d(socket, buffer, strlen(buffer));
  write_d(socket, buffer, 0); // Send empty buffer to sinal eof

  return;
}

// FILE TRANSFER FUNCTIONS /////////////////////////////////////////////////////

// This function receives a file from the socket
void receive_file(int socket, char *buffer, char *path) {
  FILE *output;
  int msg_len;
  long int i = 0, base, size;

  get_path(path);             // Local path is now a full path
  output = fopen(path, "wb"); // create/erase file to write
  printf("receving file \"%s\"...\n", get_name(path));

  read_d(socket, buffer);           // Read size
  size = strtol(buffer, NULL, 10);  // Cast size to long int

  while (i < size) {
    read_d(socket, buffer); // Read a full buffer.
    for (base = i; (i < base + BUFFLEN) && (i < size); ++i) // Write elements
      fputc(buffer[i%BUFFLEN], output);
  }

  printf("file received\n");
  fclose(output);
  return;
}

// This function splits files from /data/ and send then to the client
void send_file(int socket, char *buffer, char *full_path) {
  FILE *input;           // File to be sent
  long int i = 0, size;  // Size of the file to be sent

  input = fopen(full_path, "rb");
  printf("sending file \"%s\"\n", get_name(full_path));

  // Get size (amount of char in the file)
  fseek(input, 0, SEEK_END);
  size = ftell(input);
  fseek(input, 0, SEEK_SET);

  sprintf(buffer, "%ld", size);             // Cast size to string
  write_d(socket, buffer, strlen(buffer));  // Send file size to client

  while (i < size) { // reads char by char filling buffer until eof
    buffer[(i++)%BUFFLEN] = fgetc(input); // Add char to buffer, then incremente i
    if (i%BUFFLEN == 0 || i == size)      // i buffer full or EOF send data
      write_d(socket, buffer, BUFFLEN);   // sends entire buffer to avoid border issues
  }

  printf("file sent\n");
  fclose(input);
  return;
}

// Gets the full path of the file to be sent
char* get_path(char *path) {
  char szTmp[32];
  int bytes;

  sprintf(szTmp, "/proc/%d/exe", getpid()); // get this process origin file path
  bytes = readlink(szTmp, path, BUFFLEN);   // save path

  for (bytes ; path[bytes] != '/'; --bytes); // removes the process name
  path[bytes+1] = '\0'; // add eof

  return path; // return path size and full path
}

// This was create solely for the purpose of testing the # option
char* get_name(char *path) {
  int i;

  for(i = strlen(path); i >= 0; --i)
    if (path[i] == '/') return &(path[i+1]);

  return path;
}
