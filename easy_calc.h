#ifndef __EASY_CALC_H
#define __EASY_CALC_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct value_t {
    int64_t value;
    bool defined;
} Value;

// registers and stack
Value reg_a, reg_b, reg_c, reg_d;
Value stack[5];

// input file
FILE *input_stream;

// current char
int cur_char;

// number of parsed char
size_t parsed_char;

#endif
