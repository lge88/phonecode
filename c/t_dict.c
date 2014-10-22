#include "translator.h"
#include <stdio.h>
#include <ctype.h>

int main() {
  static dict_t _l2d;
  dict_t* l2d = &_l2d;
  dict_init("../phone.txt", l2d);
  dict_print(l2d);
  printf("a -> %c\n", dict_lookup(l2d, 'a'));
  printf("B -> %c\n", dict_lookup(l2d, 'B'));
  printf("* -> %c\n", dict_lookup(l2d, '*'));
  fclose(stdout);
}
