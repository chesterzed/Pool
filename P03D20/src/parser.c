#include "parser.h"

int is_digit(char c) {
    int result = (c >= '0' && c <= '9');
    return result;
}

int is_alpha(char c) {
    int result = ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
    return result;
}

int is_space(char c) {
    int result = (c == ' ' || c == '\t' || c == '\n');
    return result;
}

int get_token_type(const char* str) {
    int result = TOKEN_NUMBER;

    if (strcmp(str, "(") == 0)
        result = TOKEN_OPEN_PAREN;
    else if (strcmp(str, ")") == 0)
        result = TOKEN_CLOSE_PAREN;
    else if (strcmp(str, "~") == 0)
        result = TOKEN_UNARY_MINUS;
    else if (!strcmp(str, "+") || !strcmp(str, "-") || !strcmp(str, "*") || !strcmp(str, "/") ||
             !strcmp(str, "^"))
        result = TOKEN_OPERATOR;
    else if (!strcmp(str, "cos") || !strcmp(str, "sin") || !strcmp(str, "tg") || !strcmp(str, "ctg") ||
             !strcmp(str, "ln") || !strcmp(str, "sqrt"))
        result = TOKEN_FUNCTION;
    else if (strlen(str) == 1 && is_alpha(str[0]))
        result = TOKEN_VARIABLE;

    return result;
}

char** tokenize(const char* str, int* count) {
    char** tokens = malloc(MAX_TOKENS * sizeof(char*));
    int success = 1;
    int i = 0, len = strlen(str);
    *count = 0;

    for (int k = 0; k < MAX_TOKENS; k++) {
        tokens[k] = NULL;
    }

    while (i < len && success) {
        if (is_space(str[i])) {
            i++;
        } else {
            char buffer[MAX_TOKEN_LEN];
            int j = 0;

            if (str[i] == '-' && (*count == 0 || get_token_type(tokens[*count - 1]) == TOKEN_OPEN_PAREN ||
                                  get_token_type(tokens[*count - 1]) == TOKEN_OPERATOR ||
                                  get_token_type(tokens[*count - 1]) == TOKEN_FUNCTION)) {
                buffer[j++] = '~';
                i++;
            } else if (is_digit(str[i]) || str[i] == '.') {
                while ((is_digit(str[i]) || str[i] == '.') && j < MAX_TOKEN_LEN - 1) {
                    buffer[j++] = str[i++];
                }
            } else if (is_alpha(str[i])) {
                while (is_alpha(str[i]) && j < MAX_TOKEN_LEN - 1) {
                    buffer[j++] = str[i++];
                }
            } else {
                buffer[j++] = str[i++];
            }

            buffer[j] = '\0';
            if (*count < MAX_TOKENS) {
                tokens[*count] = strdup(buffer);
                (*count)++;
            } else {
                success = 0;
            }
        }
    }

    if (!success) {
        for (int k = 0; k < *count; k++) {
            free(tokens[k]);
        }
        free(tokens);
        *count = 0;
        tokens = NULL;
    }

    return tokens;
}

int validate_expression(char** tokens, int count) {
    int valid = 1;
    int paren_balance = 0;
    int last_token_type = TOKEN_NONE;
    int i = 0;

    while (i < count && valid) {
        int type = get_token_type(tokens[i]);
        int should_break = 0;

        if (type == TOKEN_VARIABLE && tokens[i][0] != 'x') {
            valid = 0;
            should_break = 1;
        }

        if (!should_break) {
            switch (type) {
                case TOKEN_NUMBER:
                case TOKEN_VARIABLE:
                    if (last_token_type == TOKEN_NUMBER || last_token_type == TOKEN_VARIABLE ||
                        last_token_type == TOKEN_CLOSE_PAREN)
                        valid = 0;
                    break;

                case TOKEN_OPERATOR:
                    if (last_token_type == TOKEN_NONE || last_token_type == TOKEN_OPERATOR ||
                        last_token_type == TOKEN_OPEN_PAREN || last_token_type == TOKEN_FUNCTION ||
                        last_token_type == TOKEN_UNARY_MINUS)
                        valid = 0;
                    break;

                case TOKEN_UNARY_MINUS:
                    if (last_token_type == TOKEN_NUMBER || last_token_type == TOKEN_VARIABLE ||
                        last_token_type == TOKEN_CLOSE_PAREN)
                        valid = 0;
                    break;

                case TOKEN_FUNCTION:
                    if (last_token_type == TOKEN_NUMBER || last_token_type == TOKEN_VARIABLE ||
                        last_token_type == TOKEN_CLOSE_PAREN)
                        valid = 0;
                    break;

                case TOKEN_OPEN_PAREN:
                    paren_balance++;
                    if (last_token_type == TOKEN_NUMBER || last_token_type == TOKEN_VARIABLE ||
                        last_token_type == TOKEN_CLOSE_PAREN)
                        valid = 0;
                    break;

                case TOKEN_CLOSE_PAREN:
                    paren_balance--;
                    if (paren_balance < 0 || last_token_type == TOKEN_OPERATOR ||
                        last_token_type == TOKEN_OPEN_PAREN || last_token_type == TOKEN_FUNCTION ||
                        last_token_type == TOKEN_UNARY_MINUS)
                        valid = 0;
                    break;

                default:
                    valid = 0;
            }
        }

        last_token_type = type;
        i++;
    }

    if (valid && paren_balance != 0) valid = 0;
    if (valid && (last_token_type == TOKEN_OPERATOR || last_token_type == TOKEN_FUNCTION ||
                  last_token_type == TOKEN_UNARY_MINUS))
        valid = 0;

    return valid;
}