/*
 * stringutils.c: Collection of string utilities I've written over time
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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stack.h"

char* cutString(char* input, size_t start, size_t end) {
  // cut out a section from input string between start and end indeces (start and end index are both cut out)
  
  // sanity check inputs
  if (start > end) {
    return NULL;
  }
  size_t originalLen = strlen(input);
  if (start > originalLen - 1 || end > originalLen - 1) { // index math
    return NULL;
  }

  // calculate size of new buffer
  size_t newSize = start + (originalLen - end);
  // create new buffer
  char* outputString = malloc(sizeof(char) * newSize);
  // copy starting part to output
  size_t index = 0;
  while (index < start) {
    outputString[index] = input[index];
    index++;
  }
  // index now points to the first character to be skipped
  while (index < newSize - 1) { // convert byte size to pointer
    // prepare yourself im about to do some really funny pointer math
    outputString[index] = input[index-start+end+1]; // instead of pointing to the first character outside of start section,
    // index now points to the first character inside the end section (end pointer + 1)
    index++;
  }
  // write null terminator
  outputString[index] = 0;
  return outputString;
}

void cutStringInPlace(char** base, size_t start, size_t end) {
  char* temp = cutString(*base, start, end);
  char* deleteThis = *base;
  *base = temp;
  free(deleteThis);
}

char* replaceString(char* base, char* replacement, size_t start, size_t end) {
  // replace a section in base string with replacement string, resizing base as needed

  // sanity check inputs
  if (start > end) {
    return NULL;
  }
  size_t originalLen = strlen(base);
  if (start > originalLen - 1 || end > originalLen - 1) { // index math
    return NULL;
  }

  // calculate size of new buffer
  size_t replaceLength = strlen(replacement);
  size_t newSize = start + originalLen - end + replaceLength;
  // create new buffer
  char* outputString = malloc(sizeof(char) * newSize);

  // copy starting part to output
  size_t outputIndex = 0;
  size_t partsIndex = 0;
  while (outputIndex < start) {
    char c = base[partsIndex];
    outputString[outputIndex] = c;
    partsIndex++;
    outputIndex++;
  }
  // outputIndex now points to next free character in array
  // copy replacement to output
  partsIndex = 0;
  while (partsIndex < replaceLength) {
    char c = replacement[partsIndex];
    outputString[outputIndex] = c;
    partsIndex++;
    outputIndex++;
  }
  // partsIndex now needs to point to the first character after the section of base to be deleted
  // copy end of base to output
  partsIndex = end + 1;
  while (partsIndex < originalLen) {
    char c = base[partsIndex];
    outputString[outputIndex] = c;
    outputIndex++;
    partsIndex++;
  }
  // write null terminator
  outputString[outputIndex] = 0;
  return outputString;
}

void replaceStringInPlace(char** base, char* replacement, size_t start, size_t end) {
  char* temp = replaceString(*base, replacement, start, end);
  char* deleteThis = *base;
  *base = temp;
  free(deleteThis);
}

char* insertString(char* base, char* insert, size_t insertIndex) {
  // insert string into base string starting at index
  // inserting is very similar to replacing, you just have a replacement selection of zero length (start == end)

  // sanity check inputs
  size_t originalLength = strlen(base);
  if (insertIndex > originalLength) {
    return NULL;
  }
  
  // calculate size of new buffer
  size_t insertLength = strlen(insert);
  size_t newSize = originalLength + insertLength + 1;
  // create new buffer
  char* outputString = malloc(sizeof(char) * newSize);

  // copy starting part to output
  size_t outputIndex = 0;
  size_t partsIndex = 0;
  while (outputIndex < insertIndex) {
    char c = base[partsIndex];
    outputString[outputIndex] = c;
    partsIndex++;
    outputIndex++;
  }
  // outputIndex now points to next free character in array
  // copy replacement to output
  partsIndex = 0;
  while (partsIndex < insertLength) {
    char c = insert[partsIndex];
    outputString[outputIndex] = c;
    partsIndex++;
    outputIndex++;
  }
  // partsIndex now needs to point to the first character after the section of base to be deleted
  // copy end of base to output
  partsIndex = insertIndex;
  while (partsIndex < originalLength) {
    char c = base[partsIndex];
    outputString[outputIndex] = c;
    outputIndex++;
    partsIndex++;
  }
  // write null terminator
  outputString[outputIndex] = 0;
  return outputString;
}

void insertStringInPlace(char** base, char* insert, size_t index) {
  char* temp = insertString(*base, insert, index);
  char* deleteThis = *base;
  *base = temp;
  free(deleteThis);
}

void printUntil(char* string, size_t stopIndex) {
  // print until a specific index in given string (including pointed to character)
  char* newstring = malloc((stopIndex+1) * sizeof(char));
  size_t index = 0;
  do {
    newstring[index] = string[index];
    index++;
  } while (index < stopIndex);
  newstring[index] = 0;
  printf("%s\n", newstring);
  free(newstring);
  return;
}

int isWhitespace(char input) {
  switch (input) {
    case ' ':
    case '\n':
    case '\t':
    case '\r':
      return 1;
    default:
      return 0;
  }
}

char* reverseString(char* input) {
  char* output = malloc((strlen(input) + 1) * sizeof(char));
  size_t stringLength = strlen(input);
  size_t index = 0;
  struct charStack stack = newStack();
  while (index < stringLength) {
    stackPush(input[index], &stack);
    index++;
  }
  index = 0;
  while (index < stringLength) {
    stackPop(&output[index], &stack);
    index++;
  }
  output[stringLength] = '\0';

  free(stack.data);
  stack.length = 0;
  stack.pointer = 0;

  return output;  
}

char* byteToAscii(unsigned char input) {
  // TODO: reverse the string
  char* output = malloc(2 * sizeof(char));
  unsigned int outputLength = 1;

  int index = -1;
  char digit;
  do {
    index++;
    digit = input % 10;
    input = input / 10;
    outputLength += 1;
    output = realloc(output, outputLength * sizeof(char));
    output[index] = digit + '0';
  }
  while (input > 0);
  output[++index] = '\0';
  char* temp = reverseString(output);
  free(output);
  return temp;
}

char* stringToIntString(char* input) {
  // convert string to space-seperated integer strings ("hello!" -> "104 101 108 108 111 33 0")
  size_t index = 0;
  size_t outputIndex = 0;
  char c;
  char* output = malloc(sizeof(char));
  output[0] = '\0';
  char* temp;
  size_t oldLen;
  do {
    c = input[index];
    temp = insertString(output, byteToAscii(c), outputIndex);
    free(output);
    output = temp;
    temp = NULL;
    // put space at end of string
    oldLen = strlen(output);
    output = realloc(output, (oldLen + 2) * sizeof(char));
    output[oldLen] = ' ';
    output[oldLen + 1] = '\0';
    outputIndex = strlen(output);
    index++;
    
  } while (c != '\0');
  // remove trailing whitespace
  output[strlen(output) - 1] = '\0';
  return output;
}

void stringToIntStringInPlace(char** input) {
  char* temp = stringToIntString(*input);
  char* deleteThis = *input;
  *input = temp;
  free(deleteThis);
  
}

char* getSlice(char* base, size_t startIndex, size_t endIndex) {
  // sanity check inputs
  size_t baseLength = strlen(base);
  if (startIndex >= baseLength || endIndex >= baseLength) {
    return NULL;
  }
  if (startIndex > endIndex) {
    return NULL;
  }

  size_t index = 0;
  char c = base[index];
  char* output = malloc(1 * sizeof(char));
  size_t outputIndex = 0;
  output[0] = '\0';
  while (c != 0) {
    if (index >= startIndex) {
      output = realloc(output, outputIndex + 2); // +1 for new char, +1 for null terminator
      output[outputIndex] = c;
      output[outputIndex + 1] = '\0';
      outputIndex++;
    }
    if (index == endIndex) {
      return output;
    }

    index++;
    c = base[index];
  }
  return NULL;
}

__int8_t replaceEscapeCode(char** output, char* input) {
  // output is a pointer to a single character,
  // input must be the entire escape code,
  // ex. "\n"
  // NOT "n"


  /* supported escape codes:
  *  \" : "               (0x22)
  *  \\ : \               (0x5C)
  *  \b : backspace       (0x08)
  *  \f : form feed       (0x0C)
  *  \n : new line        (0x0A)
  *  \r : carriage return (0x0D)
  *  \t : horizontal tab  (0x09)
  *  \v : vertical tab    (0x0B)
  *  \' : '               (0x27)
  *  \/ : /               (0x2F)
  *  \0 : null            (0x00)
  *  
  */

  *output = 0;

  // input sanity checks
  if (strlen(input) != 2) {
    printf("Warning: Error while converting escape code \"%s\": invalid escape code length.\n", input);
    return -1;
  }
  if (input[0] != '\\') {
    printf("Warning: escape code must start with '\\'\n");
    return -1;
  }

  switch (input[1]) {
    case '\\':
      *output = "\\";
      return 0;
    case '/':
      *output = "/";
      return 0;
    case '\"':
      *output = "\"";
      return 0;
    case '\'':
      *output = "'";
      return 0;
    /*
      for null escape code, length encoded strings are needed
      case '0':
      *output = "\0";
      return 0;
    */
    case 'b':
      *output = "\b";
      return 0;
    case 'f':
      *output = "\f";
      return 0;
    case 'n':
      *output = "\n";
      return 0;
    case 'r':
      *output = "\r";
      return 0;
    case 't':
      *output = "\t";
      return 0;
    case 'v':
      *output = "\v";
      return 0;
    default:
      printf("Warning: Unrecognized escape code \"%s\"\n", input);
      return -1;
  }
}

__int128_t hexToInt(char* hexInput) {
    __int128_t output = 0;
    __uint8_t nibbleIndex = 0;
    while (nibbleIndex < strlen(hexInput)) {
      char nibble = hexInput[strlen(hexInput) - nibbleIndex - 1];
      if (nibble >= '0' && nibble <= '9') {
        output += ((__int128_t)(nibble - '0')) << (4 * nibbleIndex);
      }
      if (nibble >= 'a' && nibble <= 'f') {
        output += ((__int128_t)(nibble - 'a' + 10)) << (4 * nibbleIndex);
      }
      if (nibble >= 'A' && nibble <= 'F') {
        output += ((__int128_t)(nibble - 'A' + 10)) << (4 * nibbleIndex);
      }
      nibbleIndex++;
    }
    return output;
  }

