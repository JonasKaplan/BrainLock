
# BrainLock

BrainLock is an esoteric programming language designed as a superset of BrainFuck. Its primary goal was to introduce some aspects of functional programming into the language, in a style as closely aligned with the original as possible. This includes some semblance of recursion, and even a bizarre form of closure, which is how the language's name was derived. All this was achieved with the addition of only three characters.

## Usage

As it currently stands, this language exists only as an interpretation library. The file `interpreter/interpreter.h` provides the public interface for this library. In short, simply call the function `interpret(source_file_path)`. As an example, using a hello world program taken from wikipedia:

example.bl:

```brainfuck
++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.
```

main.c:

```c
#include <stdio.h>
#include "interpreter.h"

void main(void) {
    InterpreterStatusCode result = interpret("./example.bl");
    switch (result) {
        case INTERPRETER_OK:
            return 0;
        case INTERPRETER_ERR:
            return 1;
    }
}
```

## Functionality

The functional aspects of BrainLock come into play with the addition of three characters. These are:

Round brackets, `(` and `)`. Round brackets introduce a function. When the opening bracket is encountered, the interpreter stores all instructions between the two brackets into the function registry. This registry has one byte worth of space, allowing for the storage of a maximum of two hundred and fifty six functions at any one time. The function is always stored in the register corresponding to the byte currently under the tape pointer. For example:

```brainfuck
++([->+<])
```

This example stores the instructions `[->+<]` into the third function register.

Function call, `%`. This instruction executes the function currently stored in the register of the byte currently under the tape pointer. If no such function exists, the program exits with an error. For example:

```brainfuck
(++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.)%
```

This program stores a hello world program into the first function register, then executes it.

Once all the instructions inside the function have executed, the interpreter returns to the instruction from which the function was called, and continues execution as it was before.

## These Are Not Actually Pure Functions

What I have been referring to as functions thus far cannot actually be considered functions in the traditional sense. Rather, they are simple procedures. The state of the program is shared between all executed functions. After all, essentially the entire philosophy of the BrainFuck language is a single, massive shared state. As such, in keeping with BrainFuck's philosophy, all functions operate on the same tape, and do not return values. Instead, return values can simply be written back to the tape. All this can lead to unexpected behaviour:

```brainfuck
(+)%%
```

This program errors at runtime, because the first function call increments the current cell. As a result, the second call attempts to execute the function in the second register, which does not exist.

## Some Curious Examples

The simplest possible recursive function is as follows:

```brainfuck
(%)
```

When a function is called, the byte under the tape pointer must, by definition, be currently set to the registry index of that function. As such, this function infinitely calls itself, resulting in a stack overflow.

The previously alluded to closures can be created simply by introducing a function inside of an existing function:

```brainfuck
(+(-))%%
```

## Credits

All code and documentation created by Jonas M. Kaplan.
