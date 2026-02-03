#ifndef STACK_H
#define STACK_H
#include <stdlib.h>
#include <string.h>

struct Stack {
    char* data;
    struct Stack* next;
};

struct Stack* stack_init();

void stack_push(struct Stack** top, char* token);
void stack_free(struct Stack** top);

char* stack_pop(struct Stack** top);
char* stack_peek(struct Stack* top);

int stack_is_empty(struct Stack** top);
#endif