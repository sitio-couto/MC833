#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void){
  printf("%li\n",strlen("aaa\nbbb\nc"));

  // FILE *in = fopen("file.txt", "r"), *out = fopen("test.txt", "w");
  // char buffer[256];
  //
  // // send file packet by packet
  // fseek(in, 0, SEEK_SET);
  // while (fgets(buffer, 256, in))
  //   fputs(buffer, out);
  //
  // fclose(out);
  // fclose(in);
  // return 0;
}
