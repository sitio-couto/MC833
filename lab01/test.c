#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  FILE *profile = fopen("server/data/joao@unicamp.br.txt", "r");
  char buffer[256];
  int i;

  for (i = 0; i < 6; ++i) // Skip lines until experience data
    printf("%s\n", fgets(buffer, 256, profile));

  while (fgets(buffer, 256, profile))
    printf("|%s|\n", buffer);

  return 0;
}
