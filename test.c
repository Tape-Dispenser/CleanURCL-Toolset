
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
  char* constant = "hello, faggots!";
  char* string = malloc(sizeof(char) * 16);
  strcpy(string, constant);
  printf("%s\n", string);
  cutStringInPlace(&string, 3, 7);
  printf("%s\n", string);
  return 0;
}