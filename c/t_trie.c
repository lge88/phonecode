#include "trie.h"
#include <stdio.h>
#include <ctype.h>

void transform_letter_and_print(char c, int i) {
  if (i > 0) printf(" ");
  printf("%c", toupper(c));
}

int main() {
  /* word_t* w = word_create("hello world.\n"); */

  /* for (int i = 0, len = word_len(w); i < len; ++i) */
    /* transform_letter_and_print(word_at(w, i), i); */

  /* FILE* f = fopen(file_name, "r"); */
  /* printf("n lines: %d\n", count_lines("../words.txt")); */
  /* fclose(f); */

  word_list_t* wl = wl_from_file("../words.txt");
  for (int i = 0, len = wl_len(wl); i < len; ++i)
    printf("%s\n", wl_at(wl, i));

  /* word_list_print(wl); */
  /* word_destroy(w); */

  wl_destroy(wl);
  fclose(stdout);
}
