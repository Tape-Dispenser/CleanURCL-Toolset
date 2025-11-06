#!/bin/bash

# debug
#gcc -Wall -Wextra -fsanitize=address -g ./src/main.c ./src/lib/stringutils.c ./src/lib/map.c ./src/parse.c ./src/lib/stack.c ./src/tokenize.c -o ./build/debug/urcltools -lfyaml

# build
gcc -Wall -Wextra ./src/main.c ./src/lib/stringutils.c ./src/lib/map.c ./src/parse.c ./src/lib/stack.c ./src/tokenize.c -o ./build/urcltools -lfyaml

# valgrind
#gcc -Wall -Wextra -ggdb3 ./src/main.c ./src/lib/stringutils.c ./src/lib/map.c ./src/parse.c ./src/lib/stack.c ./src/tokenize.c -o ./build/debug/valgrind/urcltools -lfyaml