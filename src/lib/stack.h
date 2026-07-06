/*
 * stack.h: Stack library
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