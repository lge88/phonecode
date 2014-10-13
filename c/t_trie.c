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
  /*   transform_letter_and_print(word_at(w, i), i); */

  /* word_destroy(w); */

  /* FILE* f = fopen(file_name, "r"); */
  /* printf("n lines: %d\n", count_lines("../words.txt")); */
  /* fclose(f); */

  word_list_t* wl = wl_from_file("../words.txt");
  /* for (int i = 0, len = wl_len(wl); i < len; ++i) */
  /*   printf("%s\n", wl_at(wl, i)); */

  static dict_t _l2d;
  dict_t* l2d = &_l2d;

  dict_init("../phone.txt", l2d);
  /* dict_print(l2d); */

  /* printf("a -> %c\n", dict_lookup(l2d, 'a')); */

  trie_node_t* root = trie_build(l2d, wl);
  printf("%s\n", trie_search(root, "43556"));
  printf("%s\n", trie_search(root, "38628466"));

  wl_destroy(wl);
  trie_destroy(root);
  fclose(stdout);
}
