#include <stddef.h>
#include <stdlib.h>

struct charStack {
  size_t length;
  size_t pointer;
  char* data;
};

struct charStack newStack() {
  struct charStack stack;

  stack.data = malloc(sizeof(char));
  stack.length = 1;
  stack.pointer = 0;

  return stack;
}

int stackPush(char c, struct charStack* stack) {
  stack->data[stack->pointer] = c;
  stack->pointer++;
  if (stack->pointer > stack->length) {
    stack->length *= 2;
    stack->data = realloc(stack->data, stack->length * sizeof(char));
  }
  if (stack->data == NULL) {
    return -1;
  }
  return 0;
}

int stackPop(char* c, struct charStack* stack) {
  if (stack->pointer == 0) {
    // check for stack underflow
    return -1;
  }
  stack->pointer--;
  *c = stack->data[stack->pointer];
  
  if (stack->pointer < (stack->length / 2)) {
    stack->length /= 2;
    stack->data = realloc(stack->data, stack->length * sizeof(char));
  }
  if (stack->data == NULL) {
    return -1;
  }
  return 0;
}