/*
 * map.c: Map library originally written for passphrase generator
 * Copyright (C) 2025, Ada Gramiak, <adadispenser@gmail.com>
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Map {
  char** keys;
  char** values;
  size_t length;
  size_t size;
} Map;

// create a new map with empty lists (map constructor)
struct Map empty_map() {
  struct Map map;
  map.size = sizeof(char*);
  map.keys = malloc(map.size);
  map.values = malloc(map.size);
  map.length = 0;
  return map;
}

// create a new map from two input strings (map constructor)
/*
struct Map full_map(char* keys, char* values) {
  struct Map map;
  if (strlen(keys) != strlen(values)) {
    map.keys = 0;
    map.values = 0;
    return map;
  }

  map.keys = malloc((strlen(keys) + 1) * map->size);
  map.values = malloc((strlen(values) + 1) * map->size);

  size_t len = strlen(keys);

  size_t index = 0;
  while (index < len) {
    map.keys[index] = keys[index];
    map.values[index] = values[index];
    index++;
  }
  // null terminator will be copied over in the loop
  return map;
}
*/


// get value (input key)
int mapGet(struct Map* map, char* key, char** output) {
  // returns 0 on success
  // returns -1 if key-value pair does not exist in map
  unsigned long index = 0;
  while (index < map->length) {
    int result = strcmp(key, map->keys[index]);
    if (result == 0) {
      *output = map->values[index];
      return 0;
    }
    index++;
  }
  return -1;
}

// add key-value pair (input new key and new value)
int mapAdd(struct Map* map, char* key, char* value) {
  printf("attempting to add key \"%s\" to map, with address %lx\n", key, key);
  // returns 0 on success
  // returns -1 if key-value pair already exists in map
  char* temp;
  if (mapGet(map, key, &temp) == 0) {
    printf("Key \"%s\" already exists!\n", key);
    return -1;
  }
  
  map->keys = realloc(map->keys, (map->length + 1) * map->size); // map_len + 1 accounts for the value to be added
  if (map->keys == 0) {
    printf("Error while reallocating memory for keys string!\n");
    return -1;
  }
  map->keys[map->length] = key; // add key to keys
  map->values = realloc(map->values, (map->length + 1) * map->size);
  if (map->keys == 0) {
    printf("Error while reallocating memory for values string!\n");
    return -1;
  }
  map->values[map->length] = value;
  map->length++;
  return 0;
}

// edit value in pair (input key and new value)
int mapUpdate(struct Map* map, char* key, char* value) {
  // returns 0 on success
  // returns -1 if key-value pair does not exist in map
  unsigned long index = 0;
  while (index < map->length) {
    if (map->keys[index] == key) {
      map->values[index] = value;
      return 0;
    }
    index++;
  }
  return -1;
}

// remove key-value pair (input key to remove)
int mapDelete(struct Map* map, char* key) {
  // returns 0 on success
  // returns -1 if key-value pair does not exist in map
  unsigned long index = 0;
  while (index < map->length) {
    int result = strcmp(key, map->keys[index]);
    if (result != 0) {
      index++;
      continue;
    }
    // overwrite the entry at index with the last entry in the map
    map->keys[index] = map->keys[map->length];
    map->values[index] = map->values[map->length];
    // overwrite the duplicate entry at the end of the map with a null terminator
    map->keys[map->length] = 0;
    map->values[map->length] = 0;
    // shrink allocated memory size for string
    // original strlen will point to the last character of the original string, which is where the new null terminator will go,
    // therefore no pointer math is needed for malloc
    map->keys = realloc(map->keys, map->length * map->size);
    if (map->keys == 0) {
      printf("Error while reallocating memory for keys string!\n");
      return -1;
    }
    map->values = realloc(map->values, map->length * map->size);
    if (map->keys == 0) {
      printf("Error while reallocating memory for values string!\n");
      return -1;
    }
    map->length--;
    return 0;
  }
  return -1;
}

void mapKill(Map* map) {
  // free all memory associated with a map
  free(map->keys);
  free(map->values);
  map->length = 0;
}
