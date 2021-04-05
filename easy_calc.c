#include "easy_calc.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define INT_CHAR(c) ((int64_t)(c))

void open_input(int argc, const char **argv) {
    if (argc > 2 || (argc == 2 && !strcmp(argv[1], "-h"))) {
        printf(
            "Usage: %s [filename]\n\n"

            "EasyCalc is a simple calcuator which supports 4 register "
            "variables (a, b, c, and d), and a stack variable (s) with 5 "
            "elements. It supports addition and subtraction. All the constants "
            "should be in the range [0, 9].\n\n"

            "If the user does not provide input filename, stdin will be "
            "used.\n\n"

            "Following is the grammar of input files:\n\n"
            "\tProgram    :: Statements\n"
            "\tStatements :: Expr | Statements Expr\n"
            "\tExpr       :: LHS '=' RHS ';'\n"
            "\tLHS        :: Variable\n"
            "\tRHS        :: Variable | Operator Variable | RHS Operator "
            "Variable\n"
            "\tVariable   :: Register | Stack | Constant\n"
            "\tRegister   :: 'a' | 'b' | 'c' | 'd'\n"
            "\tStack      :: 's' '[' Constant ']'\n"
            "\tConstant   :: '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | "
            "'8' | '9'\n"
            "\tOperator   :: '+' | '-'\n\n"

            "Additionally, EasyCalc has three semantic constraints:\n\n"
            "    a) the index of stack can not overflow (0 <= idx <= 5)\n"
            "    b) LHS cannot be a constant\n"
            "    c) every variable should be defined before using\n",
            argv[0]);
        exit(strcmp(argv[1], "-h"));
    }

    if (argc == 1) {
        input_stream = stdin;
    } else {
        if (!(input_stream = fopen(argv[1], "r"))) {
            printf("error (%d): %s\n", errno, strerror(errno));
            exit(1);
        }
    }
}

int64_t get_value(Value *p) {
    if (!p->defined) {
        puts("undefined variable");
        exit(1);
    }

    return p->value;
}

void set_value(Value *p, int64_t v) {
    p->value = v;
    p->defined = true;
}

void get_next_char() {
    while (true) {
        cur_char = fgetc(input_stream);

        if (cur_char == EOF) {
            break;
        }

        if (cur_char != ' ' && cur_char != '\n' && cur_char != '\r') {
            break;
        }
    }
}

int8_t parse_num() {
    if (INT_CHAR('0') <= cur_char && INT_CHAR('9') >= cur_char) {
        return cur_char - INT_CHAR('0');
    }

    return -1;
}

Value *parse_register() {
    switch (cur_char) {
        case (INT_CHAR('a')):
            return &reg_a;
        case (INT_CHAR('b')):
            return &reg_b;
        case (INT_CHAR('c')):
            return &reg_c;
        case (INT_CHAR('d')):
            return &reg_d;
        default:
            return NULL;
    }
}

Value *parse_stack() {
    if (cur_char != INT_CHAR('s')) {
        return NULL;
    }

    get_next_char();
    if (cur_char != INT_CHAR('[')) {
        puts("'[' is requried for stack variable");
        exit(1);
    }

    get_next_char();
    int8_t idx = parse_num();
    if (idx >= 5 || idx < 0) {
        puts("an index (0 - 4) is requried for stack variable");
        exit(1);
    }

    get_next_char();
    if (cur_char != INT_CHAR(']')) {
        puts("']' is requried for stack variable");
        exit(1);
    }

    return stack + idx;
}

void parse_expr() {
    // get LHS
    Value *lhs_p = parse_register();
    if (!lhs_p) {
        lhs_p = parse_stack();
        if (!lhs_p) {
            puts("invalid LHS");
            exit(1);
        }
    }

    get_next_char();
    if (cur_char != INT_CHAR('=')) {
        puts("'=' is required");
        exit(1);
    }

    bool op_is_required = false;
    int64_t sum_value = 0;
    get_next_char();
    do {
        int64_t sign = 1;
        if (cur_char == INT_CHAR('-')) {
            sign = -1;
            get_next_char();
        } else if (cur_char == INT_CHAR('+')) {
            get_next_char();
        } else {
            if (op_is_required) {
                puts("'+', -', or ';' is required");
                exit(1);
            }
        }

        int64_t cur_value = parse_num();
        if (cur_value < 0) {
            // parse num fail
            Value *reg_p = parse_register();
            if (!reg_p) {
                // parse regiser fail
                Value *stack_p = parse_stack();
                if (!stack_p) {
                    puts("invalid RHS");
                    exit(1);
                } else {
                    cur_value = get_value(stack_p);
                }
            } else {
                cur_value = get_value(reg_p);
            }
        }

        sum_value += sign * cur_value;
        op_is_required = true;

        get_next_char();
    } while (cur_char != INT_CHAR(';'));

    set_value(lhs_p, sum_value);
    get_next_char();
}

void parse_program() {
    while (true) {
        if (cur_char == EOF) {
            return;  // we are done
        }
        parse_expr();
    }
}

int main(int argc, const char **argv) {
    open_input(argc, argv);

    get_next_char();
    parse_program();

#define SHOW_VALUE(v, name)                           \
    do {                                              \
        if ((v).defined) {                            \
            printf("%12s: %ld\n", (name), (v).value); \
        } else {                                      \
            printf("%12s: undef\n", (name));          \
        }                                             \
    } while (0)

    SHOW_VALUE(reg_a, "Register A");
    SHOW_VALUE(reg_b, "Register B");
    SHOW_VALUE(reg_c, "Register C");
    SHOW_VALUE(reg_d, "Register D");

    SHOW_VALUE(stack[0], "Stack[0]");
    SHOW_VALUE(stack[1], "Stack[1]");
    SHOW_VALUE(stack[2], "Stack[2]");
    SHOW_VALUE(stack[3], "Stack[3]");
    SHOW_VALUE(stack[4], "Stack[4]");

    exit(0);
}
