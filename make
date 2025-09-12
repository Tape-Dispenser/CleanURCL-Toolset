#!/bin/bash

# debug
gcc -Wall -Wextra -fsanitize=address -g ./main.c ./lib/stringutils.c ./lib/map.c ./parse.c ./lib/stack.c ./tokenize.c -o ./build/debug/urcltools -lfyaml

# build
gcc -Wall -Wextra ./main.c ./lib/stringutils.c ./lib/map.c ./parse.c ./lib/stack.c ./tokenize.c -o ./build/urcltools -lfyaml

# valgrind
gcc -Wall -Wextra -ggdb3 ./main.c ./lib/stringutils.c ./lib/map.c ./parse.c ./lib/stack.c ./tokenize.c -o ./build/debug/valgrind/urcltools -lfyaml