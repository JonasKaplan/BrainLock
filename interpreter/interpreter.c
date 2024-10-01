#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "program.h"
#include "interpreter.h"

InterpreterStatusCode interpret(const char const* source_file_path) {
    int64_t source_file_byte_count;
    char* source_code;
    uint64_t read_result;

    FILE* source_file = fopen(source_file_path, "r");
    if (source_file == NULL) {
        fprintf(stderr, "No such file \"%s\", exiting\n", source_file_path);
        return INTERPRETER_ERR;
    }
    fseek(source_file, 0L, SEEK_END);
    source_file_byte_count = ftell(source_file);
    rewind(source_file);
    source_code = malloc(source_file_byte_count);
    if (source_code == NULL) {
        fclose(source_file);
        fprintf(stderr, "Memory allocation failure\n");
        return INTERPRETER_ERR;
    }
    read_result = fread(source_code, source_file_byte_count, 1, source_file);
    if (read_result != 1) {
        fclose(source_file);
        free(source_code);
        fprintf(stderr, "Failed to read source file\n");
        return INTERPRETER_ERR;
    }
    fclose(source_file);

    Program* program = program_init(source_code, source_file_byte_count);
    if (program == NULL) {
        fprintf(stderr, "Memory allocation failure\n");
        return INTERPRETER_ERR;
    }
    ProgramStatusCode program_status = PROGRAM_OK;
    while (program_status == PROGRAM_OK) {
        program_status = program_step(program);
    }

    switch (program_status) {
        case PROGRAM_EXIT:
            printf("\nProgram exited normally\n");
            return INTERPRETER_OK;
        case PROGRAM_ERR_INVALID_BRACKETING:
            printf("\nProgram exited on error: invalid bracketing\n");
            return INTERPRETER_ERR;
        case PROGRAM_ERR_STACK_OVERFLOW:
            printf("\nProgram exited on error: stack overflow\n");
            return INTERPRETER_ERR;
        case PROGRAM_ERR_NO_SUCH_FUNCTION:
            printf("\nProgram exited on error: no such function\n");
            return INTERPRETER_ERR;
        default:
            printf("\nProgram exited on error: unknown\n");
            return INTERPRETER_ERR;
    }
}