#!/bin/bash

# debug
gcc -Wall -Wextra -fsanitize=address -g ./main.c ./lib/stringutils.c ./lib/map.c ./parse.c ./lib/stack.c -o ./build/debug/urcltools

# build
gcc -Wall ./main.c ./lib/stringutils.c ./lib/map.c ./parse.c ./lib/stack.c -o ./build/urcltools

# valgrind
gcc -Wall -Wextra -g ./main.c ./lib/stringutils.c ./lib/map.c ./parse.c ./lib/stack.c -o ./build/debug/valgrind/urcltools