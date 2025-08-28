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