#include "stack.h"

struct Stack* stack_init() {
    struct Stack* result = NULL;
    return result;
}

void stack_push(struct Stack** top, char* token) {
    struct Stack* NewElem = malloc(sizeof(struct Stack));
    NewElem->data = token;
    NewElem->next = *top;
    *top = NewElem;
}

char* stack_pop(struct Stack** top) {
    char* result = NULL;
    if (*top != NULL) {
        result = (*top)->data;
        struct Stack* temp = *top;
        *top = (*top)->next;
        free(temp);
    }
    return result;
}

char* stack_peek(struct Stack* top) {
    char* result = NULL;
    if (top != NULL) {
        result = strdup(top->data);
    }
    return result;
}

int stack_is_empty(struct Stack** top) {
    int result = (*top == NULL);
    return result;
}

void stack_free(struct Stack** top) {
    while (*top != NULL) {
        char* str = stack_pop(top);
        free(str);
    }
}