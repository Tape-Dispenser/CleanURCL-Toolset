/*
 * stringutils.h: Collection of string utilities I've written over time
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

#ifndef STRINGUTILS_H
#define STRINGUTILS_H
#include <string.h>

char* cutString(char* input, size_t start, size_t end);

void cutStringInPlace(char** base, size_t start, size_t end);

char* replaceString(char* base, char* replacement, size_t start, size_t end);

void replaceStringInPlace(char** base, char* replacement, size_t start, size_t end);

char* insertString(char* base, char* insert, size_t insertIndex);

void insertStringInPlace(char** base, char* insert, size_t index);

void printUntil(char* string, size_t stopIndex);

int isWhitespace(char input);

char* stringToIntString(char* input);

void stringToIntStringInPlace(char** input);

char* byteToAscii(unsigned char input);

char* reverseString(char* input);

char* getSlice(char* base, size_t startIndex, size_t endIndex);

__int8_t replaceEscapeCode(char** output, char* input);

__int128_t hexToInt(char* hexInput);

#endif