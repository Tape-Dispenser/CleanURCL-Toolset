/*
 * parse.c: parses URCL code into structs useable by the next step in a transpiler toolchain,
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
#include "lib/stringutils.h"

/*
  since parsing returns a struct and not a text file,
  parsing will have no main function and instead be broken up into two functions,
  one to read the text from a file input, and one to parse the string into the Line array.
  the first function could be removed if i rewrite clean to just return the output as a string,
  then call all the functions provided with a master file
*/


struct Line {
  int originalLineNum;
  char type;
  /*
  valid line types:
    'h': header
    'm': macro
    'l': label
    'i': instruction
    'd': directive
  if a line object has a type other than these 5 valid types an error should be thrown
  */
  char** tokens;
};

char* codeFromFile(char* path) {
  FILE* urclFile = NULL;
  urclFile = fopen(path, "r");
  if (urclFile == NULL) {
    printf("error no. %d while opening file \"%s\"\n", errno, path);
  }
  int c;
  size_t index = 0;
  char* code = malloc(1 * sizeof(char));
  while ((c = fgetc(urclFile)) != EOF) {
    code = realloc(code, (index + 2) * sizeof(char)); // plus one for the null terminator, plus two for index -> size conversion
    code[index] = c;
    index++; // index now points to the next free character
  }
  // write null terminator
  code[index] = 0;

  return code;
}

struct Line* tokenize(char* code) {
  int inToken = 0;
  int tokenIndex = 0;
  int tokenStart = 0;
  int tokenEnd = 0;

  char* token = malloc(1 * sizeof(char));
  token[0] = '\0';

  int index = 0;
  char c = code[index];
  while (c != '\0') {

    if (inToken) {
      if (isWhitespace(c)) {
        // end of token
        printf("token: \"%s\"\n", token);
        free(token);
        tokenIndex = 0;
        token = malloc(1 * sizeof(char));
        token[tokenIndex] = '\0';
        inToken = 0;
        
      }
      else {
        token = realloc(token, (strlen(token) + 2) * sizeof(char));
        token[tokenIndex] = c;
        tokenIndex++;
        token[tokenIndex] = '\0';
        tokenEnd++;
      }
    }
    else {
      if (!isWhitespace(c)) {
        token = realloc(token, 2 * sizeof(char));
        token[0] = c;
        token[1] = '\0';
        inToken = 1;
        tokenIndex++;
        tokenStart = index;
        tokenEnd = index;
      }
    }
    
    
    index++;
    c = code[index];
  }

  return 0;

}

struct Line* lines;












/*

*/






