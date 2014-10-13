#include <stdio.h>
#include <string.h>

typedef struct {
  char wordsFile[100];
  char phoneFile[100];
  int reverse;
} options_t;

const options_t DEFAULT_OPTIONS = {
  "words.txt",
  "phone.txt",
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

int main(int argc, char* argv[]) {
  options_t options = parse_options(argc, argv);
  print_options(&options);
}
