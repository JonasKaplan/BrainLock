#ifndef PROGRAM
#define PROGRAM
#include <stdint.h>

typedef void Program;

/*
Represents the status of a program after an instruction is executed
*/
typedef enum _ProgramStatusCode {
    PROGRAM_OK,
    PROGRAM_EXIT,
    PROGRAM_ERR_INVALID_BRACKETING,
    PROGRAM_ERR_STACK_OVERFLOW,
    PROGRAM_ERR_NO_SUCH_FUNCTION,
} ProgramStatusCode;

/*
Creates a new `Program` instance, based on a string of instructions

`char* instructions`: The string of instructions to be executed
`uint64_t instruction_count`: The number of instructions in the string

Returns: A pointer to the created `Program` instance
*/
Program* program_init(char* instructions, uint64_t instruction_count);

/*
Executes a single instruction in the provided `Program`

`Program* program`: The program for which to execute an instruction

Returns: A `ProgramStatusCode` representing the status of the program
*/
ProgramStatusCode program_step(Program* program);

#endif