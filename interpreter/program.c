#include "program.h"

#include <stdlib.h>
#include <stdio.h>

#define MAX_DEPTH 32768
#define TAPE_SIZE 32768

typedef struct _IProgram {
    uint64_t* stack;
    uint64_t stack_ptr;
    uint8_t* tape;
    uint64_t tape_ptr;
    char* instructions;
    uint64_t instruction_count;
    uint64_t instruction_ptr;
    uint64_t functions[256];
} IProgram;

Program* program_init(char* instructions, uint64_t instruction_count) {
    IProgram* program = malloc(sizeof(*program));
    uint64_t* stack = calloc(MAX_DEPTH, sizeof(*stack));
    uint8_t* tape = calloc(TAPE_SIZE, sizeof(*tape));
    if ((program == NULL) || (stack == NULL) || (tape == NULL)) {
        return NULL;
    }
    program->stack = stack;
    program->stack_ptr = 0;
    program->tape = tape;
    program->tape_ptr = 0;
    program->instructions = instructions;
    program->instruction_count = instruction_count;
    program->instruction_ptr = 0;
    return program;
}

int64_t square_bracket_value(char bracket) {
    switch (bracket) {
        case '[':
            return -1;
        case ']':
            return 1;
        default:
            return 0;
    }
}

int64_t round_bracket_value(char bracket) {
    switch (bracket) {
        case '(':
            return -1;
        case ')':
            return 1;
        default:
            return 0;
    }
}

ProgramStatusCode program_step(Program* external_program) {
    IProgram* program = (IProgram*)external_program;
    int64_t count;
    switch (program->instructions[program->instruction_ptr]) {
        case '>':
            program->tape_ptr = (program->tape_ptr + 1) % TAPE_SIZE;
            break;
        case '<':
            program->tape_ptr = (((program->tape_ptr - 1) % TAPE_SIZE) + TAPE_SIZE) % TAPE_SIZE;
            break;
        case '+':
            ++program->tape[program->tape_ptr];
            break;
        case '-':
            --program->tape[program->tape_ptr];
            break;
        case '.':
            putchar(program->tape[program->tape_ptr]);
            break;
        case ',':
            program->tape[program->tape_ptr] = getchar();
            break;
        case '[':
            if (program->tape[program->tape_ptr] != 0) {
                break;
            }
            count = -1;
            while (count != 0) {
                if (program->instruction_ptr == program->instruction_count - 1) {
                    return PROGRAM_ERR_INVALID_BRACKETING;
                }
                count += square_bracket_value(program->instructions[++program->instruction_ptr]);
            }
            break;
        case ']':
            if (program->tape[program->tape_ptr] == 0) {
                break;
            }
            count = 1;
            while (count != 0) {
                if (program->instruction_ptr == 1) {
                    return PROGRAM_ERR_INVALID_BRACKETING;
                }
                count += square_bracket_value(program->instructions[--program->instruction_ptr]);
            }
            break;
        case '(':
            program->functions[program->tape[program->tape_ptr]] = program->instruction_ptr;
            count = -1;
            while (count != 0) {
                if (program->instruction_ptr == program->instruction_count - 1) {
                    return PROGRAM_ERR_INVALID_BRACKETING;
                }
                count += round_bracket_value(program->instructions[++program->instruction_ptr]);
            }
            break;
        case ')':
            if (program->stack_ptr == 0) {
                return PROGRAM_ERR_INVALID_BRACKETING;
            }
            program->instruction_ptr = program->stack[program->stack_ptr--];
            break;
        case '%':
            if (++program->stack_ptr == MAX_DEPTH) {
                return PROGRAM_ERR_STACK_OVERFLOW;
            }
            program->stack[program->stack_ptr] = program->instruction_ptr;
            program->instruction_ptr = program->functions[program->tape[program->tape_ptr]];
            break;
    }
    return (++program->instruction_ptr == program->instruction_count) ? PROGRAM_EXIT : PROGRAM_OK;
}
