/*
 * clean.c: URCL Code cleaner, for use as the first step in a transpiler toolchain,
 *          or as a standalone program
 * Copyright (C) 2025, Ada Gramiak, <adadispenser@gmail.com>
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
#include "lib/map.h"




// ############################  OPTIONS  ############################

void help() {
  puts("clean: clean [-h] [-o path] infile");
  puts("  Clean up a urcl file by removing unnecessary comments and whitespace. Resulting code is output to stdout.");
  puts("");
  puts("  Options:");
  puts("    -h:    print this menu.");
  puts("    -n:    add original line numbers to the output code");
}

int lineNums = 0;


// #############################   CODE  #############################

void clean(char* urclCode) {
  char* workingCopy = malloc(sizeof(char) * (strlen(urclCode) + 1));
  strcpy(workingCopy, urclCode);

  // step one:   add line numbers
  size_t index = 0;
  char c = workingCopy[index];
  if (lineNums != 0) {
    size_t lineCount = 0;
    while (c != 0) {
      if (c != '\n') {
        index++;
        c = workingCopy[index];
        continue;
      }
      lineCount++;

      // get length sprintf will return
      FILE* devNull = fopen("/dev/null", "w");
      size_t length = fprintf(devNull, "&L%lu", lineCount);
      fclose(devNull);
      // allocate memory for sprintf
      char* lineMarker = malloc((length + 1) * sizeof(char));

      sprintf(lineMarker, "&L%lu", lineCount);
      char* temp = insertString(workingCopy, lineMarker, index);
      free(workingCopy);
      free(lineMarker);
      workingCopy = temp;
      temp = NULL;
      index+=length+1;
      c = workingCopy[index];
    }
  }
  


  // step two:   replace all strings with a replacement key (ex. &S1, &S2, &S3, etc.), remove all multiline comments,
  //             source string must be put into a map

  int inString = 0;
  int inComment = 0;
  char* currentString;
  size_t stringIndex;
  size_t stringCount = 0;
  size_t tokenStart;
  size_t tokenEnd;
  Map stringMap = empty_map();

  index = 0;
  c = workingCopy[index];
  while (c != 0) {
    c = workingCopy[index];

    if (inString == 1) {
      currentString[stringIndex] = c;
      stringIndex++;
      currentString = realloc(currentString, (stringIndex + 1) * sizeof(char));
      if (c == '"' || c == '\'') {
        char prev = workingCopy[index - 1];
        if (prev != '\\') {
          inString = 0;
          currentString[stringIndex] = 0;
          tokenEnd = index;
          // printf("Found a string literal %s starting at character index %lu and ending at %lu.\n", currentString, tokenStart, tokenEnd);
          // add string to map and replace with the string id (&S1, &S2, &S3, etc.)
          stringCount++;
          char* stringID = malloc(23 * sizeof(char)); // string ID is 20 max digits from a u64 + 2 for &S + 1 for null terminator
          sprintf(stringID, "&S%lu", stringCount);
          int returnCode = mapAdd(&stringMap, stringID, currentString);
          if (returnCode != 0) {
            printf("Error while trying to add string \"%s\" to map, with key \"%s\"\n", currentString, stringID);
            exit(-1);
          }
          printf("Associated string %s with id %s\n", currentString, stringID);
          // change currentString to a new pointer
          // currentString's will be freed later
          // i love introducing memory leaks intentionally
          char* temp = malloc(23 * sizeof(char));
          currentString = temp;
        }
      }
      
      
    } else if (inComment == 1) {
      if (c == '/' && workingCopy[index-1] == '*') {
        inComment = 0;
        tokenEnd = index;
        // delete comment
        printf("Found a multiline comment starting at character index %lu and ending at %lu.\n", tokenStart, tokenEnd);
        // if multiline contains a newline i need to replace the comment with a newline
        size_t tokenIndex = tokenStart;
        int containsNewline = 0;
        while (tokenIndex <= tokenEnd) {
          if (workingCopy[tokenIndex] == '\n') {
            containsNewline = 1;
            break;
          }
          tokenIndex++;
        }
        if (containsNewline == 1) {
          char* temp = replaceString(workingCopy, "\n", tokenStart, tokenEnd);
          free(workingCopy);
          workingCopy = temp;
          temp = NULL;
          index = tokenStart;
        } else {
          char* temp = cutString(workingCopy, tokenStart, tokenEnd);
          free(workingCopy);
          workingCopy = temp;
          temp = NULL;
          index = tokenStart;
        }
        index = tokenStart;
        continue; // no need to increment index, it already points to the character immediately after the (now deleted) comment
      }


    } else {
      switch (c) {
        case '"':
        case '\'':
          inString = 1;
          currentString = malloc(2 * sizeof(char));
          stringIndex = 0;
          currentString[stringIndex] = c;
          stringIndex++;
          tokenStart = index;
          break;
        case '*':
          if (workingCopy[index-1] != '/') {
            break;
          }
          inComment = 1;
          tokenStart = index - 1; // make sure / in /* gets selected too
          break;
      }

    }  

    index++;
    c = workingCopy[index];
  }

  // step five:  remove all single line comments

  // step six:   remove all extra whitespace

  // step seven: put all characters and strings back

  // step eight: output code
  printf("%s\n", workingCopy);
  free(workingCopy);
}


// #########################  MAIN FUNCTION  #########################

int main(int argc, char **argv) {
  int option;
  char* urclPath;
  char* temp_arg;

  // parse arguments
  while ((option = getopt(argc, argv, ":hno")) != -1) {
    
    switch (option) {
      case 'h': {
        help();
        exit(0);
      }
      case 'n': {
        lineNums = 1;
        break;
      }
      case ':': {
        printf("Option \'%c\' missing value.\n", optopt);
        exit(-1);
      }
      case '?': {
        printf("Unknown option \'%c\'\n", optopt);
        exit(-1);
      }
    }
  }
  if (argc - optind != 1) {
    printf("expected 1 file path input, got %u\n", argc-optind);
    exit(-1);
  }

  // read input file into string
  
  urclPath = malloc( sizeof(char) * (strlen(argv[optind]) + 1) );
  FILE* urclFile = NULL;
  urclFile = fopen(argv[optind], "r");
  if (urclFile == NULL) {
    printf("error no. %d while opening file \"%s\"\n", errno, urclPath);
  }
  free(urclPath);
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
  printf("Length of input code: %lu, char at end: '%c' (%u)\n", strlen(code), code[index], code[index]);
  printf("Index: %lu\n", index);

  //printf("%s\n", code);
  fclose(urclFile);
  clean(code);


  free(code);
  exit(0);
}