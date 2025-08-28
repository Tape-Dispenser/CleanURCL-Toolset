/*
 * map.h: Map library written for passphrase generator
 * Copyright (C) 2025, Ada (Tape), <adadispenser@gmail.com>
 *   Special thanks to: Stella
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

#ifndef MAP_H
#define MAP_H
#include <stddef.h>

typedef struct Map {
  char** keys;
  char** values;
  size_t length;
  size_t size;
} Map;

struct Map empty_map();

//struct Map full_map(char** keys, char** values);

int mapAdd(struct Map* map, char* key, char* value);

int mapGet(struct Map* map, char* key, char** output);

int mapUpdate(struct Map* map, char* key, char* value);

int mapDelete(struct Map* map, char* key);

void mapKill(Map* map);

#endif
