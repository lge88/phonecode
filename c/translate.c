#include <stdio.h>
#include <string.h>

#include "trie.h"

typedef struct {
  char wordsFile[100];
  char phoneFile[100];
  int reverse;
} options_t;

const options_t DEFAULT_OPTIONS = {
  "../words.txt",
  "../phone.txt",
  0
};

void _parse_options(int argc, char* argv[], options_t* options_ptr) {
  if (argc <= 0) return;

  char* arg = argv[0];
  if (!strcmp(arg, "-p") || !strcmp(arg, "--phone-file")) {
    strcpy(options_ptr->phoneFile, argv[1]);
    argc--;
    argv++;
  } else if (!strcmp(arg, "-w") || !strcmp(arg, "--words-file")) {
    strcpy(options_ptr->wordsFile, argv[1]);
    argc--;
    argv++;
  } else if (!strcmp(arg, "-r") || !strcmp(arg, "--reverse")) {
    options_ptr->reverse = 1;
  }

  _parse_options(argc-1, argv+1, options_ptr);
}

options_t parse_options(int argc, char* argv[]) {
  options_t options = DEFAULT_OPTIONS;
  _parse_options(argc, argv, &options);
  return options;
}

void print_options(options_t* options_ptr) {
  printf("wordsFile: %s\n", options_ptr->wordsFile);
  printf("phoneFile: %s\n", options_ptr->phoneFile);
  printf("reverse: %s\n", options_ptr->reverse ? "true" : "false");
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


typedef void (*translate_fn_t)(const translator_t*, const char* in, char* out);

void reverse_translate(const translator_t* translator, const char* word, char* digits) {
  translator_encode(translator, word, digits);
  remove_char(digits, '-');

  char tmp[100];
  strcpy(tmp, digits);
  sprintf(digits, "%s: %s", word, tmp);
}

void translate(const translator_t* translator, const char* digits, char* sentences) {
  word_list_list_t _wll;
  word_list_list_t* wll = &_wll;
  wll_init(wll);

  translator_decode(translator, digits, wll);

  const char* first_jointer = " ";
  const char* second_jointer = "\n";
  int len_second_jointer = strlen(second_jointer);
  char* head = sentences;
  word_list_node_t* cur = wll->head;

  while (cur) {
    head += wl_join(cur->wl, first_jointer, head);

    if (cur->next) {
      sprintf(head, "%s", second_jointer);
      head += len_second_jointer;
    }
    cur = cur->next;
  }

  wll_destroy(wll);
}

void translate_line_by_line(
    const translator_t* translator,
    translate_fn_t fn) {

  char buf_out[1024];
  char buf_in[1024];

  while (fgets(buf_in, sizeof(buf_in), stdin) != NULL) {
    trim_newline_right(buf_in);
    fn(translator, buf_in, buf_out);
    printf("%s\n", buf_out);
  }
}

int main(int argc, char* argv[]) {
  options_t options = parse_options(argc, argv);

  translator_t* translator = translator_create(options.phoneFile, options.wordsFile);
  translate_fn_t fn;

  if (options.reverse) {
    fn = &reverse_translate;
  } else {
    char out[100];
    translate(translator, "4355696753", out);
    printf("4355696753: %s", out);
    /* fn = &translate; */
  }
  /* translate_line_by_line(translator, fn); */

  translator_destroy(translator);
  fclose(stdin);
  fclose(stdout);
}
