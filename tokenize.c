/*
 * clean.c: URCL Code cleaner, for use as the first step in a transpiler toolchain.
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
#include "lib/map.h"
#include "lineobject.h"

// #############################   CODE  #############################

struct TokenizedOutput {
  Map stringMap;
  struct Line* tokenizedCode;
  size_t lineCount;
};

char* stringToArray(char* input) {
  char* output = malloc((strlen(input) + 1) * sizeof(char));
  strcpy(output, input);
  // convert string literal to an array of integers represented in ascii

  unsigned char isString = 1;

// step 1: drop quote marks and calculate string length
  if (output[0] == '\'') {
    isString = 0;
  }
  cutStringInPlace(&output, 0, 0);
  size_t endIndex = strlen(output) - 1;
  cutStringInPlace(&output, endIndex, endIndex);
  // endIndex is the string length
  //printf("String length: %lu\n", endIndex);

// step 2: replace escape sequences with actual characters
  size_t stringIndex = 0;
  char c;
  char prev = 0;
  
  while (stringIndex < strlen(output)) {
    c = output[stringIndex];

    if (prev == '\\') {
      char* replacement;
      char escapeCode[3];
      escapeCode[0] = '\\';
      escapeCode[1] = c;
      escapeCode[2] = '\0';

      if (replaceEscapeCode(&replacement, escapeCode) == 0) {
        printf("replacing escape code \"%s\"\n", escapeCode);
        replaceStringInPlace(&output, replacement, stringIndex - 1, stringIndex);
        stringIndex--;
      }
    }
    stringIndex++;
    prev = c;
  } 

// step 3: pass to stringToAscii()
  if (isString) {
    stringToIntStringInPlace(&output);
    // add square brackets around list of immediates
    insertStringInPlace(&output, "[", 0);
    insertStringInPlace(&output, "]", strlen(output));
  }
  else {
    char* temp = byteToAscii(output[0]);
    free(output);
    output = temp;
    temp = NULL;
  }
  return output;
}

char* stripWhitespace(char* input) {
  // remove all excess whitespace from a line of urcl code
  // ex. "   ADD      R1 r2            R3   " -> "ADD R1 r2 R3"
  size_t index = 0;
  size_t whiteStart = 0;
  size_t whiteEnd = 0;
  int hasToken = 0;
  int inWhitespace = 0;

  char* workingCopy;
  workingCopy = malloc((strlen(input) + 1) * sizeof(char));
  strcpy(workingCopy, input);

  char c = workingCopy[index];
  while (c != '\0') {
    if (inWhitespace) {
      if (isWhitespace(c)) {
        whiteEnd = index;
        index++;
        c = workingCopy[index];
        continue;
      }
      inWhitespace = 0;
      if (hasToken) {
        replaceStringInPlace(&workingCopy, " ", whiteStart, whiteEnd);
        index = whiteStart + 1;
      }
      else { 
        cutStringInPlace(&workingCopy, whiteStart, whiteEnd);
        index = whiteStart;
      }
      c = workingCopy[index];
      continue;
    } 
    else {
      if (isWhitespace(c)) {
        inWhitespace = 1;
        whiteStart = index;
        whiteEnd = index;
      }
      else {
        hasToken = 1;
      }
    }
    index++;
    c = workingCopy[index];
  }
  return workingCopy;
}

struct TokenizedOutput tokenize(char* inputCode) {
// step one:   replace all strings with a replacement key (ex. &S1, &S2, &S3, etc.), and remove all types of comments
  int inString = 0;
  int inMultiline = 0;
  int inComment = 0;
  char* currentString;
  size_t stringIndex;
  size_t stringCount = 0;
  size_t tokenStart;
  size_t tokenEnd;
  Map stringMap = empty_map();

  size_t index = 0;
  char c = inputCode[index];
  while (c != 0) {
    c = inputCode[index];
    if (inString == 1) {
      currentString[stringIndex] = c;
      stringIndex++;
      currentString = realloc(currentString, (stringIndex + 1) * sizeof(char));
      if (c != '"' && c != '\'') {
        index++;
        continue;
      }
      char prev = inputCode[index - 1];
      if (prev == '\\') {
        index++;
        continue;
      }
      inString = 0;
      // add null terminator to string
      currentString[stringIndex] = '\0';
      tokenEnd = index;
      // add string to map and replace with the string id (&S1, &S2, &S3, etc.)
      stringCount++;
      char* stringID = malloc(23 * sizeof(char)); // string ID is 20 max digits from a u64 + 2 for &S + 1 for null terminator
      sprintf(stringID, "&S%lu", stringCount);
      int returnCode = mapAdd(&stringMap, stringID, currentString);
      if (returnCode != 0) {
        printf("Error while trying to add string \"%s\" to map, with key \"%s\"\n", currentString, stringID);
        exit(-1);
      }
      
      // replace string in inputCode with string identifier
      replaceStringInPlace(&inputCode, stringID, tokenStart, tokenEnd);
      index = tokenStart + strlen(stringID);
      // change currentString to a new pointer
      // currentString needs to be freed on program exit, not here!!!
      char* temp = malloc(23 * sizeof(char));
      currentString = temp;
    } 
    else if (inMultiline == 1) {
      char prev = inputCode[index - 1];
      if (c == '/' && prev == '*') {
        inMultiline = 0;
        tokenEnd = index;
        // delete comment
        // if multiline contains a newline i need to replace the comment with a newline
        size_t tokenIndex = tokenStart;
        size_t newLines = 0;
        while (tokenIndex <= tokenEnd) {
          if (inputCode[tokenIndex] == '\n') {
            newLines++;
          }
          tokenIndex++;
        }
        if (newLines >= 1) {
          char* replacement = malloc((newLines + 1) * sizeof(char));
          size_t replacementIndex = 0;
          while (newLines > 0) {
            replacement[replacementIndex] = '\n';
            replacementIndex++;
            newLines--;
          }
          replacement[replacementIndex] = '\0';
          replaceStringInPlace(&inputCode, replacement, tokenStart, tokenEnd);
          free(replacement);
          index = tokenStart;
        } else {
          cutStringInPlace(&inputCode, tokenStart, tokenEnd);
          index = tokenStart;
        }
        index = tokenStart;
        continue; // no need to increment index, it already points to the character immediately after the (now deleted) comment
      }
    } else if (inComment == 1) {
      if (c == '\n') { // detect the end of the line
        inComment = 0;
        tokenEnd = index - 1;  // -1 in order to not remove newlines before adding line markers
        // delete comment
        replaceStringInPlace(&inputCode, " ", tokenStart, tokenEnd);
        index = tokenStart;
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
          if (index == 0) {
            break; // make sure we don't do an illegal memory access
          }
          if (inputCode[index-1] != '/') {
            break;
          }
          inMultiline = 1;
          tokenStart = index - 1; // make sure / in /* gets selected too
          break;
        case '/':
          if (index == 0) {
            break; // make sure we don't do an illegal memory access
          }
          if (inputCode[index-1] != '/') {
            break;
          }
          inComment = 1;
          tokenStart = index - 1;
          break;
      }

    }  

    index++;
  }

  // Step 2:  add line numbers
  index = 0;
  c = inputCode[index];
  size_t lineCount = 0;
  while (c != 0) {
    if (c != '\n') {
      index++;
      c = inputCode[index];
      continue;
    }
    lineCount++;

    // get length sprintf will return
    FILE* devNull = fopen("/dev/null", "w");
    size_t length = fprintf(devNull, " &L%lu", lineCount);
    fclose(devNull);

    // allocate memory for sprintf
    char* lineMarker = malloc((length + 1) * sizeof(char));
    sprintf(lineMarker, " &L%lu", lineCount);
    insertStringInPlace(&inputCode, lineMarker, index);

    // Prevent memory leaks and bad indexing
    free(lineMarker);
    index+=length+1;
    c = inputCode[index];
  }
  // Add another line marker at the end of the final line
  lineCount++;// step four: output code

  FILE* devNull = fopen("/dev/null", "w");
  size_t length = fprintf(devNull, " &L%lu", lineCount);
  fclose(devNull);

  // allocate memory for sprintf
  char* lineMarker = malloc((length + 1) * sizeof(char));
  sprintf(lineMarker, " &L%lu", lineCount);
  insertStringInPlace(&inputCode, lineMarker, index);

  // Prevent memory leaks and bad indexing
  free(lineMarker);
  index+=length;
  c = inputCode[index];

// step three: tokenize code
  // prepare state
  __uint8_t inToken = 0;
  tokenStart = 0;
  tokenEnd = 0;
  struct Token* tokenList = malloc(sizeof(struct Token));
  size_t tokenIndex = 0;

  index = 0;
  c = inputCode[index];
  size_t codeLength = strlen(inputCode);
  while (index <= codeLength) {
    if (inToken) {
      if (isWhitespace(c) || c == '\0') {
        char* token = getSlice(inputCode, tokenStart, tokenEnd);
        struct Token obj;
        obj.string = token;
        tokenList[tokenIndex] = obj;
        tokenIndex++;
        tokenList = realloc(tokenList, (tokenIndex + 1) * sizeof(struct Token));
        inToken = 0;
      }
      else {
        tokenEnd = index;
      }
    }
    else {
      if (!isWhitespace(c) && c != '\0') {
        tokenStart = index;
        tokenEnd = index;
        inToken = 1;
      }
    }
    index++;
    c = inputCode[index];
  }

  // step 4: put tokens into lines
  struct Line* lines;
  size_t linesIndex = 0;
  lines = malloc(sizeof(struct Line));
  
  struct Line line;
  size_t lineIndex = 0;
  line.tokens = malloc(sizeof(struct Token));
  line.tokenCount = 0;

  index = 0;
  while (index < tokenIndex) {
    struct Token token = tokenList[index];

    // put token in line
    line.tokens[lineIndex] = token;
    line.tokenCount++;
    lineIndex++;
    line.tokens = realloc(line.tokens, (lineIndex + 1) * sizeof(struct Token));
    
    if (token.string[0] == '&' && token.string[1] == 'L') {
      // end of line
      // add line to lines
      lines[linesIndex] = line;
      linesIndex++;
      lines = realloc(lines, (linesIndex + 1) * sizeof(struct Line));

      // make a new line
      line.tokens = NULL;
      line.tokens = malloc(sizeof(struct Token));
      line.tokenCount = 0;
      lineIndex = 0;
    }
    index++;
  }

// step 5: output code
  struct TokenizedOutput output;
  output.stringMap = stringMap;
  output.tokenizedCode = lines;
  output.lineCount = linesIndex;
  return output;
}