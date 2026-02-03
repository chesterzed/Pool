#ifndef PARSER_H
#define PARSER_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TOKENS 256
#define MAX_TOKEN_LEN 32

#define TOKEN_NONE 0
#define TOKEN_NUMBER 1
#define TOKEN_OPERATOR 2
#define TOKEN_OPEN_PAREN 3
#define TOKEN_CLOSE_PAREN 4
#define TOKEN_FUNCTION 5
#define TOKEN_VARIABLE 6
#define TOKEN_UNARY_MINUS 7

int is_digit(char c);
int is_alpha(char c);
int is_space(char c);
int get_token_type(const char* str);
char** tokenize(const char* str, int* count);
int validate_expression(char** tokens, int count);

#endif