#include <stdio.h>
#include <stddef.h>
#include "lib/stack.h"

int main() {
  printf("Creating new stack\n");
  struct charStack stack = newStack();

  printf("Inputs: '1', '2', '3'\n");
  stackPush('1', &stack);
  stackPush('2', &stack);
  stackPush('3', &stack);

  printf("Outputs: ");
  char c;
  stackPop(&c, &stack);
  printf("%c ", c);
  stackPop(&c, &stack);
  printf("%c ", c);
  stackPop(&c, &stack);
  printf("%c\n", c);

  return 0;
}