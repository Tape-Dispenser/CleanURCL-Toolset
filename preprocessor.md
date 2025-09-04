# CleanURCL Toolset Preprocessor

## Immediates

When working with immediates in translation files, sometimes it is necessary to modify the CleanURCL immediates in order to suit the needs of the target architecture. A number of preprocessor modifiers and operations exist for precomputing values.

To declare the start of a preprocessor immediate statement, wrap the statement in curly brackets.

Example 1: 
```
...
"umlt": {
...
  "r i i": [
    "addis {A},0,{(B@hu*C)@h}"
    "ori {A},{A},{(B@hu*C)@l}"
  ]
},
...
```

### Values

#### Operands
Operands of a CleanURCL instruction can be accessed by using their capital letters.

Example 2:
```
...
"add": {
  "r r r": [
    "add {A},{B},{C}"
  ],
},
...
```

#### Numbers
Decimal or Hexadecimal immediates can be used. Hexadecimal numbers are prefixed with `0x`.

Example 3:
```
...
"str": {
  "r i": [
    "ldi r3,{A+0x1000}",
    "stw {B},r3"
  ],
},
...
```

### Immediate Modifiers
Codes that can modify immediate values to be more suitable for the target architecture.   
Modifiers can be added using the `@` symbol followed by one or more modifiers.

Example 4:
```
{A@hu}
```

If used on registers, transpiler will throw an error.   


#### Halves
You can access the upper half of an immediate value with `h`.
You can access the lower half of an immediate value with `l`.

Example 5:
```
limm {A},{B@h},{C@l}
```

#### Signedness
You can cast an immediate as signed with `s`.
You can cast an immediate as unsigned with `u`.

Example 6:
```
...
"imm {A},{B@u}",
"sra {A},{B},{C@s}"
...
```

#### Negation / Bitwise NOT
You can perform negation on an immediate with `-`.
You can perform bitwise NOT on an immediate with `~`.

Example 7:
```
...
"add {A},{A},{-B}",
"xor {A},{B},{~C}"
...
```

### Operators
Operators can be used to perform operations on two values.

#### Mathematical
You can perform addition on two values using `+`.
You can perform subtraction on two values using `-`.
You can perform multiplication on two values using `*`.
You can perform division on two values using `/`.
You can perform a modulo operation on two values using `%`.

Example 8:
```
{A+B}
{A * 2 - 3}
{B / A % 7}
```

#### Bitwise
You can perform a right shift on two values using `>>`.
You can perform a left shift on two values using `<<`.
You can perform a bitwise AND on two values using `&`.
You can perform a bitwise OR on two values using `|`.
You can perform a bitwise XOR on two values using `^`.
 
Example 9:
```
{A<<3}
{A>>3}
{B>>A@u} # right shifts by unsigned A times
```

#### Logical
When using logical operators, values will be calculated to whatever the configured true/false values are.

You can test equality using `==`.
You can test inequality using `!=`.
You can test for size difference using `<`, `>`, `<=`, `>=`. 
You can test for boolean conditions using `&&`, `||`
You can perform a boolean NOT on a statement using `!`

Example 10:
```
# note: spacing is not required, just added for readability
{! 5 == 3 && @BITS >= B}
```

### Prioritization
Prioritization can be changed using `()`

Example 11:
```
{(0xFFFF - A) / 4}
```

The priority of operators and modifiers is as follows: 
1. Brackets `()`
2. Immediate Modifiers `@`
3. Immediate Negation `~`, `-`
3. Multiplication, Division, Modulo `*`, `/`, `%`
4. Addition, Subtraction `+`, `-`
5. Bitwise Shift `<<`, `>>`
6. Equality `==`, `!=`
7. Relational `<`, `>`, `<=`, `>=`
8. Logical NOT `!`
9. Logical `&&`, `||`
