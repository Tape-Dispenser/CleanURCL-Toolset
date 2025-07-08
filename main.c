/*
 * main.c: CleanURCL Transpiler main function
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>

#include <datatypes.h>

#include "lib/stringutils.h"
#include "lib/map.h"

#include "clean.h"


char* toolsetVersion = "alpha 0.0.2";


// #########################  HELP FUNCTION  #########################

void help() {
  printf("CleanURCL Tools version %s\n", toolsetVersion);
  puts("Copyright (C) 2025, under the GNU Affero General Public License version 3.");
  puts("By Ada (Tape) adadispenser@gmail.com");
  puts("For reporting issues go to https://github.com/Tape-Dispenser/CleanURCL-Toolset/issues");
  puts("");
  puts("urcltools : urcltools [-h] <input path> <-t path | -e 0-3> [-cukv] [-e path] [-p int] [-o path]");
  puts("  urcl translation toolset");
  puts("");
  puts("  Options:");
  puts("    -h           :  print this menu.");
  puts("    -c           :  stop at code cleaning step.");
  puts("    -t <path>    :  pick translation set for the transpiler to use. If no file is specified the program will return an error.");
  puts("    -e [0-3]     :  compile to emulator-ready bitcode with optional complexity level. (0 = corer, 1 = core, 2 = basic, 3 = complex, none = auto). If this option is specified translation file is ignored.");
  puts("    -p <integer> :  how many times to run code through the optimizer (if unspecified defaults to 20). If zero optimization is skipped.");
  puts("    -u           :  only allow urcl-compliant code features (parser will throw an error if code contains CleanURCL features).");
  puts("    -o <path>    :  declare output file path. If no output is declared it will default to $pwd/out.s"); 
  puts("    -k           :  keep temporary files.");
  puts("    -v           :  verbose transpiling. Only works if translation file declares a comment style.");
}

// #########################  OPTIONS SETUP  #########################

// booleans
u8 cleanOnly = 0;         // if this is one then stop after cleaning code
u8 doTranslations = 1;    // if this is zero then assemble to bitcode
u8 baseOnly = 0;          // if this is one then only allow base URCL features
u8 keepTempFiles = 0;     // if this is one then do not delete temporary files created in the compilation process
u8 verboseTranspile = 0;  // if this is one then add comments to output assembly code (only if comments are defined in translation file)


// integers
u8 complexityLevel = 3;     // complex = 3, basic = 2, core = 1, corer = 0
u8 optimizationPasses = 0;  // once optimizer is added change this to 20

// strings
char* translationPath;
char* outputPath;


// ########################  OTHER FUNCTIONS  ########################

u64 stoi(char* input) {
  // convert numerical string to int
  u64 i = 0;
  char c;
  int output = 0;
  while (i <= strlen(input)) {
    c = input[i];
    if (c == 0) {
      break;
    }
    if (c >= '0' && c <= '9') {
      output = output * 10 + (c - '0'); // convert numerical character to integer, append to output        
    } else {
      printf("Error: expected an unsigned byte argument, got \"%s\" instead.\n", input);
      exit(-1);
    }
    i++;
  }
  return output;
}


// #########################  MAIN FUNCTION  #########################

int main(int argc, char **argv) {
  int option;
  char* urclPath;

  // parse arguments
  while ((option = getopt(argc, argv, ":hcukvt:e:p:o:")) != -1) {
    
    switch (option) {
      case 'h': {
        help();
        exit(0);
      }
      case 'c': {
        cleanOnly = 1;
        break;
      }
      case 'u': {
        baseOnly = 1;
        break;
      }
      case 'k': {
        keepTempFiles = 1;
        break;
      }
      case 'v': {
        verboseTranspile = 1;
        break;
      }
      case 'e': {
        doTranslations = 0;
        complexityLevel = (u8) stoi(optarg);
        break;
      }
      case 'p': {
        optimizationPasses = (u8) stoi(optarg);
        break;
      }
      case 't': {
        translationPath = optarg;
        break;
      }
      case 'o': {
        outputPath = optarg;
        break;
      }
      case ':': {
        printf("Option \'%c\' missing value.\n", optopt);
        exit(-1);
      }
      case '?': {
        printf("Unknown option \'%c\'\n", optopt);
        exit(-1);
      }
    }
  }
  if (argc - optind != 1) {
    printf("expected 1 file path input, got %u\n", argc-optind);
    exit(-1);
  }

  // read input file into string
  
  urclPath = argv[optind];
  FILE* urclFile = NULL;
  urclFile = fopen(urclPath, "r");
  if (urclFile == NULL) {
    printf("error no. %d while opening file \"%s\"\n", errno, urclPath);
  }
  int c;
  size_t index = 0;
  char* code = malloc(1 * sizeof(char));
  while ((c = fgetc(urclFile)) != EOF) {
    code = realloc(code, (index + 2) * sizeof(char)); // plus one for the null terminator, plus two for index -> size conversion
    code[index] = c;
    index++; // index now points to the next free character
  }
  // write null terminator
  code[index] = 0;
  fclose(urclFile);

  // printf("input address:  %p\n", code);

  char* temp;
  temp = clean(code);

  // printf("output address: %p\n", temp);
  // puts("");
  // puts("");
  printf("Output Code:\n%s\n", temp);


  exit(0);
}