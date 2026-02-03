#include "shunting_yard.h"

#include "parser.h"
#include "stack.h"

char** shunting_yard(char** tokens, int count, int* output_count) {
    struct Stack* op_stack = stack_init();
    char** output = malloc(MAX_TOKENS * sizeof(char*));
    *output_count = 0;
    int success = 1;
    int i;

    for (i = 0; i < MAX_TOKENS; i++) {
        output[i] = NULL;
    }

    i = 0;
    while (i < count && success) {
        if (*output_count >= MAX_TOKENS) {
            success = 0;
        } else {
            int type = get_token_type(tokens[i]);
            // int processed = 1;

            if (type == TOKEN_NUMBER || type == TOKEN_VARIABLE) {
                output[(*output_count)++] = strdup(tokens[i]);
            } else if (type == TOKEN_FUNCTION) {
                stack_push(&op_stack, strdup(tokens[i]));
            } else if (type == TOKEN_OPEN_PAREN) {
                stack_push(&op_stack, strdup(tokens[i]));
            } else if (type == TOKEN_CLOSE_PAREN) {
                char* top = stack_peek(op_stack);
                while (!stack_is_empty(&op_stack) && top && strcmp(top, "(") != 0) {
                    free(top);
                    output[(*output_count)++] = stack_pop(&op_stack);
                    top = stack_peek(op_stack);
                }
                if (top) free(top);

                if (stack_is_empty(&op_stack)) {
                    success = 0;
                } else {
                    char* popped_paren = stack_pop(&op_stack);
                    if (popped_paren) free(popped_paren);

                    top = stack_peek(op_stack);
                    if (!stack_is_empty(&op_stack) && top && get_token_type(top) == TOKEN_FUNCTION) {
                        free(top);
                        output[(*output_count)++] = stack_pop(&op_stack);
                    } else if (top) {
                        free(top);
                    }
                }
            } else if (type == TOKEN_OPERATOR || type == TOKEN_UNARY_MINUS) {
                char* top = stack_peek(op_stack);
                while (!stack_is_empty(&op_stack) && top &&
                       (get_token_type(top) == TOKEN_OPERATOR || get_token_type(top) == TOKEN_UNARY_MINUS) &&
                       prio_token(top) >= prio_token(tokens[i])) {
                    free(top);
                    output[(*output_count)++] = stack_pop(&op_stack);
                    top = stack_peek(op_stack);
                }
                if (top) free(top);
                stack_push(&op_stack, strdup(tokens[i]));
            }
        }
        i++;
    }

    while (!stack_is_empty(&op_stack) && success) {
        if (*output_count >= MAX_TOKENS) {
            success = 0;
        } else {
            char* op = stack_pop(&op_stack);
            if (op && strcmp(op, "(") == 0) {
                free(op);
                success = 0;
            } else {
                output[(*output_count)++] = op;
            }
        }
    }

    if (!success) {
        for (int j = 0; j < *output_count; j++) {
            free(output[j]);
        }
        free(output);
        output = NULL;
        *output_count = 0;
    }

    stack_free(&op_stack);
    return output;
}

int prio_token(const char* op) {
    int priority = 0;
    if (!op) return priority;
    if (strcmp(op, "~") == 0)
        priority = 5;
    else if (strcmp(op, "^") == 0)
        priority = 4;
    else if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0)
        priority = 3;
    else if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0)
        priority = 2;
    return priority;
}