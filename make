#!/bin/bash

# debug
gcc -Wall -Wextra -fsanitize=address -g ./clean.c ./lib/stringutils.c ./lib/map.c -o ./build/debug/clean

# build
gcc ./clean.c ./lib/stringutils.c ./lib/map.c -o ./build/clean

# valgrind
gcc -Wall -Wextra -g ./clean.c ./lib/stringutils.c ./lib/map.c -o ./build/debug/valgrind/clean