#!/bin/bash

# debug
gcc -Wall -Wextra -fsanitize=address -g ./clean.c ./lib/stringutils.c ./lib/map.c ./parse.c ./lib/stack.c -o ./build/debug/clean

# build
gcc -Wall ./clean.c ./lib/stringutils.c ./lib/map.c ./parse.c ./lib/stack.c -o ./build/clean

# valgrind
gcc -Wall -Wextra -g ./clean.c ./lib/stringutils.c ./lib/map.c ./parse.c ./lib/stack.c -o ./build/debug/valgrind/clean