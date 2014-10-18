#ifndef _TRIE_H_
#define _TRIE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define str_eq(a, b) (strcmp(a, b) == 0)
#define MAX_WORD_LENGTH 100
#define min(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
  char** words;
  int len;
  int cap;
} word_list_t;
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

int count_lines(FILE* f) {
  int count = 0;
  char buf[MAX_WORD_LENGTH];
  while (fgets(buf, sizeof(buf), f) != NULL) ++count;
  return count;
}

void trim_newline_right(char* s) {
  int len = strlen(s);
  if (s[len-1] == '\n') s[len-1] = '\0';
}

word_list_t* wl_create(int cap) {
  word_list_t* wl = (word_list_t*) malloc(sizeof(word_list_t));
  wl->len = 0;
  wl->cap = cap;
  wl->words = malloc(cap*sizeof(char**));
  return wl;
}

word_list_t* wl_from_file(const char* wl_file) {
  word_list_t* wl = wl_create(10);
  char buf[MAX_WORD_LENGTH];
  FILE* f = fopen(wl_file, "r");

  rewind(f);
  while (fgets(buf, sizeof(buf), f) != NULL) {
    trim_newline_right(buf);
    wl_append(wl, buf);
  }

  fclose(f);
  return wl;
}

int wl_len(const word_list_t* wl) {
  return wl->len;
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
trie_node_t* trie_build(const dict_t* l2d, const word_list_t* wl);
trie_node_t* trie_node_create();
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
    if (node->wl == 0) {
      node->wl = wl_create(1);
    }
    wl_append(node->wl, word);
    return;
  }

  char letter = suffix[0];
  char digit = dict_lookup(l2d, letter);
  int idx = digit - '0';
  if (idx >= 0 && idx < 10) {
    if (node->children[idx] == 0) {
      node->children[idx] = trie_node_create();
    }
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
    for (int i = 0; i < 10; ++i) {
      trie_destroy(node->children[i]);
    }
    free(node);
  }
}

const word_list_t* trie_search(trie_node_t* root, const char* digits) {
  if (root == 0) return 0;

  printf("digits: %s\n", digits);

  int len = strlen(digits);
  if (len == 0) return root->wl;

  char digit = digits[0];
  int idx = digit - '0';
  trie_node_t* node = root->children[idx];
  return trie_search(node, digits + 1);
}


void init() {
  word_list_t* wl = wl_from_file("../words.txt");
  /* wl_print(wl); */

  static dict_t _l2d;
  dict_t* l2d = &_l2d;
  dict_init("../phone.txt", l2d);

  /* dict_print(l2d); */
  /* printf("a -> %c\n", dict_lookup(l2d, 'a')); */
  /* printf("B -> %c\n", dict_lookup(l2d, 'B')); */

  trie_node_t* root = trie_build(l2d, wl);
  wl_print(trie_search(root, "843"));
  wl_print(trie_search(root, "43556"));
  wl_print(trie_search(root, "38628466"));

  wl_destroy(wl);
  trie_destroy(root);
  fclose(stdout);
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

void reverse_translate(translator_t* translator, const char* word, char* output) {
  dict_encode_word(&translator->l2d, word, output);
}

#endif
