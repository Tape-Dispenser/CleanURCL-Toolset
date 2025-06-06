/*
 * parse.h: parses URCL code into structs useable by the next step in a transpiler toolchain,
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

#ifndef PARSE_H
#define PARSE_H

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

char* codeFromFile(char* path);

struct Line* tokenize(char* code);

#endif