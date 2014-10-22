#ifndef _TRANSLATOR_H_
#define _TRANSLATOR_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 1024

// helpers:
char* trim_newline_right(char* s) {
  int end = strlen(s) - 1;
  if (end >= 0 && s[end] == '\n') s[end] = '\0';
  return s;
}

void remove_char(char* str, char c) {
  char* head = str;
  char* tail = str;
  while (*head != '\0') {
    if (*head != c) {
      *tail = *head;
      ++tail;
    }
    ++head;
  }
  *tail = '\0';
}

// Implemented using resizing array.
typedef struct {
  char** words;
  int len;
  int cap;
} word_list_t;

word_list_t* wl_create(int cap);
word_list_t* wl_from_file(const char* wordsFile);
void wl_destroy(word_list_t* wl);
const char* wl_at(const word_list_t* wl, int i);
int wl_len(const word_list_t* wl);
void wl_append(word_list_t* wl, const char* word);

// debug:
void wl_print(const word_list_t* wl) {
  if (!wl) {
    printf("[]\n");
    return;
  }

  for (int i = 0, len = wl_len(wl); i < len; ++i)
    printf("%d/%d: %s\n", i, len, wl_at(wl, i));
}

word_list_t* wl_create(int cap) {
  word_list_t* wl = (word_list_t*) malloc(sizeof(word_list_t));
  wl->len = 0;
  wl->cap = cap;
  wl->words = malloc(cap*sizeof(char**));
  return wl;
}

word_list_t* wl_from_file(const char* wl_file) {
  word_list_t* wl = wl_create(50000);
  char buf[MAX_WORD_LENGTH];

  FILE* f = fopen(wl_file, "r");
  // while (fscanf(f, "%[^\n]\n", buf) == 1) wl_append(wl, buf);
  while (fgets(buf, sizeof(buf), f) != NULL) wl_append(wl, trim_newline_right(buf));
  fclose(f);

  return wl;
}

int wl_len(const word_list_t* wl) {
  return wl->len;
}

int wl_num_chars(const word_list_t* wl) {
  int count = 0;
  for (int i = 0, len = wl_len(wl); i < len; ++i) count += strlen(wl->words[i]);
  return count;
}

const char* wl_at(const word_list_t* wl, int i) {
  if (i >= 0 && i < wl_len(wl)) return wl->words[i];
  return 0;
}

void wl_append(word_list_t* wl, const char* word) {
  int cap = wl->cap;
  int len = wl->len;

  int word_size = strlen(word) + 1;

  if (len == cap) {
    char** old_words = wl->words;
    wl->words = malloc(2*cap*sizeof(char**));
    for (int i = 0; i < len; ++i) wl->words[i] = old_words[i];
    wl->cap *= 2;
    free(old_words);
  }

  wl->words[len] = malloc(word_size*sizeof(char));
  strcpy(wl->words[len], word);
  wl->len++;
}

void wl_destroy(word_list_t* wl) {
  for (int i = 0, len = wl_len(wl); i < len; ++i) free(wl->words[i]);
  free(wl->words);
  free(wl);
}

int wl_join(word_list_t* wl, const char* jointer, char* dest) {
  if (!wl) return 0;

  int len = wl_len(wl);
  if (wl_len(wl) == 0) return 0;

  char* head = dest;
  int len_jointer = strlen(jointer);
  for (int i = 0; i < len; ++i) {
    if (i > 0) {
      sprintf(head, "%s", jointer);
      head += len_jointer;
    }
    sprintf(head, "%s", wl->words[i]);
    head += strlen(wl->words[i]);
  }

  return (head - dest);
}

typedef struct {
  char table[26];
} dict_t;

void dict_init(const char* phoneFile, dict_t* l2d);
void dict_print(const dict_t* l2d);
char dict_lookup(const dict_t* l2d, char letter);

void dict_init(const char* phoneFile, dict_t* l2d) {
  FILE* f = fopen(phoneFile, "r");
  char buf[10];
  while (fgets(buf, sizeof(buf), f) != NULL) {
    char digit = buf[0];
    int len = strlen(buf);
    for (int i = 2, end = len - 2; i <= end; ++i) {
      char letter = buf[i];
      int ind = letter - 'A';
      if (ind >= 0 && ind < 26) l2d->table[ind] = digit;
    }
  }
  fclose(f);
}

char dict_lookup(const dict_t* l2d, char letter) {
  int ind = toupper(letter) - 'A';
  if (l2d && ind >= 0 && ind < 26) return l2d->table[ind];
  return '-';
}

void dict_encode_word(const dict_t* l2d, const char* word, char* output_digits) {
  int len = strlen(word);
  for (int i = 0; i < len; ++i) output_digits[i] = dict_lookup(l2d, word[i]);
  output_digits[len] = '\0';
}

void dict_print(const dict_t* l2d) {
  for (int i = 0; i < 26; ++i) {
    char letter = 'A' + i;
    char digit = dict_lookup(l2d, letter);
    printf("%c -> %c\n", letter, digit);
  }
}

typedef struct trie_node_t {
  word_list_t* wl;
  struct trie_node_t* children[10];
} trie_node_t;
trie_node_t* trie_node_create();
trie_node_t* trie_build(const dict_t* l2d, const word_list_t* wl);
void trie_insert(trie_node_t* root, const dict_t* l2d, const char* word);
void trie_destroy(trie_node_t* root);
const word_list_t* trie_search(trie_node_t* root, const char* digits);

trie_node_t* trie_node_create() {
  trie_node_t* node = malloc(sizeof(trie_node_t));
  node->wl = 0;
  for (int i = 0; i < 10; ++i) node->children[i] = 0;
  return node;
}

void _trie_insert(trie_node_t* node, const dict_t* l2d, const char* word, const char* suffix) {
  if (node == 0 || word == 0) return;

  int len = strlen(suffix);
  if (len == 0) {
    if (node->wl == 0) node->wl = wl_create(1);
    wl_append(node->wl, word);
    return;
  }

  char letter = suffix[0];
  char digit = dict_lookup(l2d, letter);
  int idx = digit - '0';
  if (idx >= 0 && idx < 10) {
    if (node->children[idx] == 0) node->children[idx] = trie_node_create();
    _trie_insert(node->children[idx], l2d, word, suffix + 1);
  }
}

void trie_insert(trie_node_t* root, const dict_t* l2d, const char* word) {
  _trie_insert(root, l2d, word, word);
}

trie_node_t* trie_build(const dict_t* l2d, const word_list_t* wl) {
  trie_node_t* root = trie_node_create();
  for (int i = 0, len = wl_len(wl); i < len; ++i) {
    const char* word = wl_at(wl, i);
    trie_insert(root, l2d, word);
  }
  return root;
}

void trie_destroy(trie_node_t* node) {
  if (node) {
    if (node->wl) wl_destroy(node->wl);
    for (int i = 0; i < 10; ++i) trie_destroy(node->children[i]);
    free(node);
  }
}

const word_list_t* trie_search(trie_node_t* root, const char* digits) {
  if (root == 0) return 0;

  int len = strlen(digits);
  if (len == 0) return root->wl;

  char digit = digits[0];
  int idx = digit - '0';
  trie_node_t* node = root->children[idx];
  return trie_search(node, digits + 1);
}

typedef struct {
  word_list_t* wl;
  trie_node_t* trie_root;
  dict_t l2d;
} translator_t;

translator_t* translator_create(const char* phoneFile, const char* wordsFile) {
  translator_t* translator = malloc(sizeof(translator_t));

  dict_init(phoneFile, &translator->l2d);
  translator->wl = wl_from_file(wordsFile);
  translator->trie_root = trie_build(&translator->l2d, translator->wl);

  return translator;
}

void translator_destroy(translator_t* translator) {
  wl_destroy(translator->wl);
  trie_destroy(translator->trie_root);
  free(translator);
}

void translator_encode(const translator_t* translator, const char* word, char* output) {
  dict_encode_word(&translator->l2d, word, output);
}

void translator_decode(const translator_t* translator, const char* digits, word_list_t* solns) {
  if (!solns) return;

  int len = strlen(digits);
  if (len == 0) {
    wl_append(solns, "");
    return;
  }

  trie_node_t* root = translator->trie_root;
  for (int i = 1; i <= len; ++i) {
    char prefix[MAX_WORD_LENGTH];
    const char* rest = digits + i;

    strncpy(prefix, digits, i);
    prefix[i] = '\0';

    const word_list_t* first_words = trie_search(root, prefix);
    if (first_words) {
      word_list_t* wl_partials = wl_create(10);
      translator_decode(translator, rest, wl_partials);

      int num_partial_solns = wl_len(wl_partials);
      if (num_partial_solns > 0) {
        int num_first_words = wl_len(first_words);

        for (int j = 0; j < num_first_words; ++j) {
          const char* first_word = wl_at(first_words, j);

          for (int k = 0; k < num_partial_solns; ++k) {
            const char* partial_soln = wl_at(wl_partials, k);
            char soln[1024];
            char* p = soln;

            strcpy(p, first_word);
            p += strlen(first_word);
            if (strlen(partial_soln) > 0) sprintf(p, " %s", partial_soln);

            wl_append(solns, soln);
          }
        }
      }

      wl_destroy(wl_partials);
    }
  }
}

void translator_reverse_translate(const translator_t* translator, const char* word, char* digits) {
  translator_encode(translator, word, digits);
  remove_char(digits, '-');

  char tmp[100];
  strcpy(tmp, digits);
  sprintf(digits, "%s: %s", word, tmp);
}

void translator_translate(const translator_t* translator, const char* digits, char* sentences) {
  word_list_t* solns = wl_create(100);
  translator_decode(translator, digits, solns);

  const char* jointer = "\n";
  int digits_len = strlen(digits);
  char* p = sentences;

  for (int i = 0, len = wl_len(solns); i < len; ++i) {
    const char* soln = wl_at(solns, i);
    if (i > 0) sprintf(p++, "\n");
    sprintf(p, "%s: %s", digits, soln);
    p += (digits_len + strlen(soln) + 2);
  }

  wl_destroy(solns);
}

#endif
