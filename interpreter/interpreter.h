#ifndef INTERPRETER
#define INTERPRETER

/*
Represents the status of the interpreter after executing a program
*/
typedef enum _InterpreterStatusCode {
    INTERPRETER_OK,
    INTERPRETER_ERR,
} InterpreterStatusCode;

/*
Executes a BrainLock source file

`const char* source_file_path`: The path to the source file to be executed (Not OS agnostic)

Returns: An `InterpreterStatusCode` representing the status of the interpreter
*/
InterpreterStatusCode interpret(const char* source_file_path);

#endif