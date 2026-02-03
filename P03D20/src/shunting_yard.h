#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

int prio_token(const char* op);
char** shunting_yard(char** tokens, int count, int* output_count);

#endif