#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char buffer[256];
  FILE *profile = fopen("server/data/joao@unicamp.br.txt", "a+");
  int i = 0;

  while(strcmp(fgets(buffer, 256, profile), "Experiência:\n")){};
  do { ++i; } while(fgets(buffer, 256, profile));
  fprintf(profile, "\t(%d)added experience to profile\n", i);

  return 0;
}
