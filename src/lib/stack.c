/*
 * stack.c: Stack library
 * Copyright (C) 2025-2026, Ada (Tape), <adadispenser@gmail.com>
 *   Special thanks to: Stella
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
  if (stack->pointer == stack->length) {
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