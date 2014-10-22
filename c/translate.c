#include <stdio.h>
#include <string.h>

#include "translator.h"

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

typedef void (*translate_fn_t)(const translator_t*, const char* in, char* out);
void translate_line_by_line(
    const translator_t* translator,
    translate_fn_t fn) {

  static char buf_out[MAX_WORD_LENGTH];
  static char buf_in[MAX_WORD_LENGTH];

  while (fscanf(stdin, "%[^\n]\n", buf_in) == 1) {
    fn(translator, buf_in, buf_out);
    printf("%s\n", buf_out);
  }
}

int main(int argc, char* argv[]) {
  options_t options = parse_options(argc, argv);

  translator_t* translator = translator_create(options.phoneFile, options.wordsFile);
  translate_fn_t fn;

  if (options.reverse) {
    fn = &translator_reverse_translate;
  } else {
    fn = &translator_translate;
  }
  translate_line_by_line(translator, fn);

  translator_destroy(translator);
  fclose(stdin);
  fclose(stdout);
}
