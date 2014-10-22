#include "translator.h"
#include <stdio.h>
#include <ctype.h>

int main() {
  translator_t* translator = translator_create("../phone.txt", "../words.txt");

  wl_print(trie_search(translator->trie_root, "843"));
  wl_print(trie_search(translator->trie_root, "43556"));
  wl_print(trie_search(translator->trie_root, "38628466"));

  translator_destroy(translator);
  fclose(stdout);
}
