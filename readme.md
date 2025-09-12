This is being reworked in C
Command line syntax is changing

Supported Macros:
  `@DEFINE <A> <B>`: defines <A> as a macro equivalent to <B>. If <B> contains spaces or newlines, it must be a string. (not implemented)
  `@DEBUG`: pauses execution when code reaches this line. (not implemented)
  `@DEBUG onwrite <A>`: pauses execution when memory address, register, or port <A> is written to. (not implemented)
  `@DEBUG onread <A>`: pauses execution when memory address, register, or port <A> is read from. (not implemented)
