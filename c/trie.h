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
  char* val;
} word_t;
word_t* word_create(const char* val);
void word_destroy(word_t* word);
const char* word_value(const word_t* word);
int word_len(const word_t* word);
char word_at(const word_t* word, int i);

word_t* word_create(const char* val) {
  size_t len = min(strlen(val) + 1, MAX_WORD_LENGTH);
  word_t* word = (word_t*) malloc(sizeof(word_t));
  word->val = malloc(len*sizeof(char));
  strncpy(word->val, val, len);
  return word;
}

void word_destroy(word_t* word) {
  free(word->val);
  free(word);
}

const char* word_value(const word_t* word) {
  return word->val;
}

int word_len(const word_t* word) {
  return strlen(word->val);
}

char word_at(const word_t* word, int i) {
  int l = word_len(word);
  if (i >= 0 && i < l) return word->val[i];
  return '\0';
}

typedef struct {
  word_t** words;
  int len;
} word_list_t;
word_list_t* wl_from_file(const char* wordsFile);
void wl_destroy(word_list_t* wl);
const char* wl_at(const word_list_t* wl, int i);
int wl_len(const word_list_t* wl);

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

word_list_t* wl_from_file(const char* wl_file) {
  word_list_t* wl = (word_list_t*) malloc(sizeof(word_list_t));
  char buf[MAX_WORD_LENGTH];
  FILE* f = fopen(wl_file, "r");

  int num_words = count_lines(f);
  word_t** words = malloc(num_words*sizeof(word_t*));
  int i = 0;

  rewind(f);
  while (fgets(buf, sizeof(buf), f) != NULL) {
    trim_newline_right(buf);
    words[i++] = word_create(buf);
  }
  wl->words = words;
  wl->len = num_words;

  fclose(f);
  return wl;
}

int wl_len(const word_list_t* wl) {
  return wl->len;
}

const char* wl_at(const word_list_t* wl, int i) {
  if (i >= 0 && i < wl_len(wl)) return word_value(wl->words[i]);
  return 0;
}

void wl_destroy(word_list_t* wl) {
  for (int i = 0, len = wl_len(wl); i < len; ++i) word_destroy(wl->words[i]);
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

void dict_print(const dict_t* l2d) {
  for (int i = 0; i < 26; ++i) {
    char letter = 'A' + i;
    char digit = dict_lookup(l2d, letter);
    printf("%c -> %c\n", letter, digit);
  }
}

typedef struct trie_node_t {
  char* word;
  struct trie_node_t* children[10];
} trie_node_t;
trie_node_t* trie_build(const dict_t* l2d, const word_list_t* wl);
trie_node_t* trie_node_create();
void trie_insert(trie_node_t* root, const dict_t* l2d, const char* word);
void trie_destroy(trie_node_t* root);
const char* trie_search(trie_node_t* root, const char* digits);

trie_node_t* trie_node_create() {
  trie_node_t* node = malloc(sizeof(trie_node_t));
  node->word = 0;
  for (int i = 0; i < 10; ++i) node->children[i] = 0;
  return node;
}

void _trie_insert(trie_node_t* node, const dict_t* l2d, const char* word, const char* suffix) {
  if (node == 0 || word == 0) return;

  int len = strlen(suffix);
  if (len == 0) return;

  if (len == 1) {
    node->word = malloc((strlen(word) + 1)*sizeof(char));
    strcpy(node->word, word);
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
  for (int i = 0, num_words = wl_len(wl); i < num_words; ++i) {
    const char* word = wl_at(wl, i);
    trie_insert(root, l2d, word);
  }
  return root;
}

void trie_destroy(trie_node_t* node) {
  if (node) {
    if (node->word) free(node->word);
    for (int i = 0; i < 10; ++i) {
      trie_destroy(node->children[i]);
    }
    free(node);
  }
}

const char* trie_search(trie_node_t* root, const char* digits) {
  if (root == 0) return 0;

  int len = strlen(digits);
  if (len == 0) return 0;
  if (len == 1) return root->word;

  char digit = digits[0];
  int idx = digit - '0';
  trie_node_t* node = root->children[idx];
  return trie_search(node, digits + 1);
}

#endif
