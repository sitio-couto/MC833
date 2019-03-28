#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  FILE *index = fopen("server/data/index.txt", "r");
  char buffer[256];
  int r;

  while (fgets(buffer, 256, index)) {
    buffer[strlen(buffer)-1] = '\0';
    printf("sending profile: %s||%d\n", buffer, r);
  }

  return 0;
}
