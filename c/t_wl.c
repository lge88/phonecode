#include "translator.h"
#include <stdio.h>
#include <ctype.h>

int main() {
  word_list_t* wl = wl_from_file("../words.txt");
  wl_print(wl);
  wl_destroy(wl);
  fclose(stdout);
  fclose(stdin);
}
