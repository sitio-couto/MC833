#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char buffer[256];
  FILE *profile = fopen("server/data/joao@unicamp.br.txt", "a+");

  do fscanf(profile, " %s", buffer);
  while(strcmp(buffer, "Experiências:"));
  fgets(buffer, 256, profile);
  printf("he lives in %s\n", buffer);

  return 0;
}
