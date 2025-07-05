#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "lib/stringutils.h"
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

  free(stack.data);
  stack.pointer = 0;
  stack.length = 0;


  char* constant = "alskdf\\njaslk\\xFFdfkas\\0j /* penis */ fasdlfkjasldfj;as";
  char* test = malloc((strlen(constant) + 1) * sizeof(char));
  strcpy(test, constant);

  printf("reversing string \"%s\"\n", test);
  char* temp = reverseString(test);
  printf("reversed string: \"%s\"\n", temp);
  

  free(temp);
  temp = cutString(test, 0, 0);
  printf("string with first char cut out: \"%s\"\n", temp);
  
  free(test);

  test = temp;
  temp = NULL;
  size_t index = strlen(test) - 1;
  temp = cutString(test, index, index);
  printf("string with last char cut out: \"%s\"\n", temp);

  free(test);
  free(temp);

  return 0;
}