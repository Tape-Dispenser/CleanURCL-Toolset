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

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>

#include <libfyaml.h>

#include "tokenize.h"
#include "codeobjects.h"
#include "lib/map.h"
#include "lib/stringutils.h"

void parse(struct Code* input, struct fy_document* translations) {
  struct Code code = *input;
  size_t lineIndex = 0;
  size_t tokenIndex;
  Map defineMap = empty_map();
  while (lineIndex < code.lineCount) {
    struct Line line = code.lines[lineIndex];
    tokenIndex = 0;
    while (tokenIndex < line.tokenCount) {
      struct Token token = line.tokens[tokenIndex];
      token.type = '\0';
      token.value = 0;

      printf("Token: \"%s\"\n", token.string);

      // step one: replace all @DEFINE macros
      
      // if token is in define list, replace it
      char* temp;
      int returnCode = mapGet(&defineMap, token.string, &temp);
      if (returnCode == 0 && token.string != temp) {
        printf("Found token \"%s\" to be replaced with \"%s\"\n", token.string, temp);
        free(token.string);
        token.string = strdup(temp);
      }
      // if token is the first token in a line, capitalize it, and test if it is a define macro
      if (tokenIndex == 0) {
        char* capitalized = capitalize(token.string);
        if (strcmp(capitalized, "@DEFINE") == 0) {
          // add macro to define map
          printf("Detected macro at marker %s.\n", line.tokens[line.tokenCount - 1].string);
          if (line.tokenCount != 4) {
            fprintf(stderr, "Error at line %s, expected 2 arguments in @DEFINE statement, but got %lu.\n", line.tokens[line.tokenCount - 1].string, line.tokenCount);
            exit(-1);
          }
          printf("Should replace \"%s\" with \"%s\"\n", line.tokens[1].string, line.tokens[2].string);
          mapAdd(&defineMap, line.tokens[1].string, line.tokens[2].string);
        }
        free(capitalized);
      }
      
      // TODO step two: calculate defined constants
      // this will require defining the layout for how a translation file looks, as well as parsing data from translation file,
      // ideally parse that data into some sort of struct containing maps



      // write token back to line
      line.tokens[tokenIndex] = token;
      tokenIndex++;
    }
    // write line back to lines
    code.lines[lineIndex] = line;
    lineIndex++;
  }
  // write back working code copy to main function's code
  *input = code;
  return;
}


