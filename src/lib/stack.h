#ifndef STACK_H
#define STACK_H

#include <stddef.h>

struct charStack {
  size_t length;
  size_t pointer;
  char* data;
};

struct charStack newStack();

int stackPush(char c, struct charStack* stack);

int stackPop(char* c, struct charStack* stack);

#endif