
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "lib/stringutils.h"
#include "lib/stack.h"

int main() {
  char* hexValue = "1234567890abcdef1234567890abcdef";
  __uint128_t integer = hexToInt(hexValue);
  printf("%llx\n", integer);
  return 0;
}