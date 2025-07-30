#ifndef LINEOBJECT_H
#define LINEOBJECT_H

#include <bits/types.h>

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

#endif