This project is currently being reworked in C, and as such does not currently work.

## Command Line Syntax:
`urcltools [-h] <input path> <-t path | -e [0-3]> [-cuknv] [-p int] [-o path]`
### Options:
- -h : print help menu.
- -c : stop at code cleaning step.
- -t \<path\> : pick translation set for the transpiler to use. If no file is specified the program will return an error.
- -e [0-3] : compile to emulator-ready bitcode with optional complexity level. (0 = corer, 1 = core, 2 = basic, 3 = complex, none = auto). If this option is specified translation file is ignored.
- -p \<integer\> : how many times to run code through the optimizer (if unspecified defaults to 20). If zero, optimization is skipped.
- -u : only allow urcl-compliant code features (parser will throw an error if code contains CleanURCL features).
- -o \<path\> : declare output file path. If no output is declared it will default to $pwd/out.s, or $pwd/out.bin if using emulator mode.
- -k : keep temporary files.
- -n : append a null terminator to the end of every string immediate.
- -v : verbose transpiling. If translation file does not declare a comment style, an error is returned.

## Supported Macros:
- `@DEFINE <A> <B>`: defines `<A>` as a macro equivalent to `<B>`. If `<B>` contains spaces or newlines, it must be a string. (not implemented)
- `@DEBUG`: pauses execution when code reaches this line. (not implemented)
- `@DEBUG onwrite <A>`: pauses execution when memory address, register, or port `<A>` is written to. (not implemented)
- `@DEBUG onread <A>`: pauses execution when memory address, register, or port `<A>` is read from. (not implemented)
