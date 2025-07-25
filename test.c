#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "lib/stringutils.h"
#include "lib/stack.h"

void test(char** testString) {
  *testString = "hello, world!";
}

int main() {
  char* string = "Hi!";
  printf("%s\n", string);
  test(&string);
  printf("%s\n", string);
  return 0;
}