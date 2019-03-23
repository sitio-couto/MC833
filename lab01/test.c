#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void){
  FILE *in = fopen("server/data/file.txt", "rb"), *out = fopen("test.txt", "wb");
  char buffer[256];
  int j, i, size, msg_len;

  fseek(in, 0, SEEK_END);
  size = ftell(in);
  fseek(in, 0, SEEK_SET);

  // for (i = 0; i < size; ++i)
  //   fputc(fgetc(in), out);

  j = 0;
  for (i = 0; i < size; ++i){ // reads file filling buffer
    buffer[j++] = fgetc(in);
    if (j == BUFFLEN-2 || i == size-1) {
      while ((msg_len = read_d(socket, buffer, 256)) != 256)
        for (i = 0; i < msg_len; ++i) fputc(buffer[i], output);    // sends filled up buffer to client
      j = 0;
    }
  }

  fclose(out);
  fclose(in);
  return 0;
}
