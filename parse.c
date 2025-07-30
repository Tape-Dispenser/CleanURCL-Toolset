/*
 * parse.c: parses extra data from URCL code for the next step in a transpiler toolchain,
 *          along with doing error checking
 * Copyright (C) 2025, Ada (Tape), <adadispenser@gmail.com>
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include "tokenize.h"
#include "lineobject.h"
#include "lib/map.h"
#include "lib/stringutils.h"

struct Line* parse(struct TokenizedOutput code) {
  size_t lineIndex = 0;
  size_t tokenIndex;
  while (lineIndex < code.lineCount) {
    struct Line line = code.tokenizedCode[lineIndex];
    tokenIndex = 0;
    while (tokenIndex < line.tokenCount) {
      struct Token token = line.tokens[tokenIndex];
      switch (token.string[0]) {
        case 'R':
        case 'r':
        case '$':
          token.type = 'r'; // Register
          break;
        case 'M':
        case 'm':
        case '#':
          token.type = 'h'; // Heap
          break;
        case '.':
          token.type = 'l'; // Label
          break;
        case '~':
          token.type = 'e'; // rElative
          break;
        case '@':
          token.type = 'm'; // Macro
          break;
        default:
          if (token.string[0] >= '0' && token.string[0] <= '9') {
            if (strlen(token.string) < 3) {
              token.type = 'i'; // decimal immediate
              break;
            }
            if (token.string[1] >= '0' && token.string[1] <= '9') {
              token.type = 'i'; // decimal immedate
              break;
            }
            if (token.string[1] == 'x' || token.string[1] == 'X') {
              // hex immediate
              char* hexValue = getSlice(token.string, 2, strlen(token.string) - 1);
              free(token.string);
              // convert hexValue to integer, put int into token.value then convert to string and put into token.string
              
            }
            if (token.string[1] == 'b' || token.string[1] == 'B') {
              // binary immediate
              char* binValue = getSlice(token.string, 2, strlen(token.string) - 1);
              free(token.string);
              // convert binValue to integer, put int into token.value then convert to string and put into token.string
            }

            break;
          }
      }
      tokenIndex++;
    }
    lineIndex++;
  }
}


