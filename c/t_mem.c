#include "translator.h"
#include <stdio.h>
#include <ctype.h>

int main() {
  translator_t* translator = translator_create("../phone.txt", "../words.txt");

  char out[1024];

  translator_translate(translator, "45683968", out);
  printf("%s\n", out);

  translator_translate(translator, "7225247386", out);
  printf("%s\n", out);

  translator_translate(translator, "3454635254", out);
  printf("%s\n", out);

  translator_destroy(translator);
  fclose(stdout);
}
