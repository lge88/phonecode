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

  /* word_list_t* wl = wl_from_file("../words.txt"); */
  /* wl_print(wl); */

  /* static dict_t _l2d; */
  /* dict_t* l2d = &_l2d; */
  /* dict_init("../phone.txt", l2d); */

  /* dict_print(l2d); */
  /* printf("a -> %c\n", dict_lookup(l2d, 'a')); */
  /* printf("B -> %c\n", dict_lookup(l2d, 'B')); */

  /* trie_node_t* root = trie_build(l2d, wl); */

  translator_t* translator = translator_create("../phone.txt", "../words.txt");

  char digits[50];

  reverse_translate(translator, "hello", digits);
  printf("hello -> %s\n", digits);

  /* wl_print(trie_search(translator->trie_root, "843")); */
  /* wl_print(trie_search(translator->trie_root, "43556")); */
  /* wl_print(trie_search(translator->trie_root, "38628466")); */

  translator_destroy(translator);

  fclose(stdout);
}
