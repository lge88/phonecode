#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
const char* wl_at(word_list_t* wl, int i);
int wl_len(word_list_t* wl);

int count_lines(FILE* f) {
  int count = 0;
  char buf[MAX_WORD_LENGTH];
  while (fgets(buf, sizeof(buf), f) != NULL) ++count;
  return count;
}

word_list_t* wl_from_file(const char* wl_file) {
  word_list_t* wl = (word_list_t*) malloc(sizeof(word_list_t));
  char buf[MAX_WORD_LENGTH];
  FILE* f = fopen(wl_file, "r");

  int num_words = count_lines(f);
  word_t** words = malloc(num_words*sizeof(word_t*));
  int i = 0;

  rewind(f);
  while (fgets(buf, sizeof(buf), f) != NULL) words[i++] = word_create(buf);
  wl->words = words;
  wl->len = num_words;

  fclose(f);
  return wl;
}

int wl_len(word_list_t* wl) {
  return wl->len;
}

const char* wl_at(word_list_t* wl, int i) {
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
dict_t* dict_create(const char* phoneFile);
void dict_destroy(dict_t* l2d);
char dict_lookup(const dict_t* l2d, char letter);

typedef struct trie_node_t {
  char digit;
  struct trie_node_t* children[10];
  word_t word;
} trie_node_t;
trie_node_t* trie_create(const dict_t* l2d, const word_t* words, int num_words);
void trie_destroy(trie_node_t* root);
const word_t* trie_search(trie_node_t* root, const char* digits);
