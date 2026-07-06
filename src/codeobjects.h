/*
 * codeobjects.h: used by the toolchain to store URCL code and metadata
 * Copyright (C) 2025-2026, Ada (Tape), <adadispenser@gmail.com>
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
#ifndef CODEOBJECTS_H
#define CODEOBJECTS_H

#include <bits/types.h>
#include "lib/map.h"

struct Token {
  char type;
  char* string;
  __int128_t value;
};

struct Line {
  __uint64_t linenumber;
  char linetype;
  struct Token* tokens;
  __uint64_t tokenCount;
};

struct Code {
  Map stringMap;
  struct Line* lines;
  size_t lineCount;
};

#endif