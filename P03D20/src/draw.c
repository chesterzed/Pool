#include "draw.h"

#include "parser.h"

double evaluate_rpn(char** rpn, int count, double x, int* error) {
    double result = 0;
    double* stack = malloc(256 * sizeof(double));
    int top = -1;
    *error = 0;
    int i = 0;

    while (i < count && !*error) {
        int type = get_token_type(rpn[i]);
        int processed = 1;

        if (type == TOKEN_NUMBER) {
            stack[++top] = atof(rpn[i]);
        } else if (type == TOKEN_VARIABLE) {
            stack[++top] = x;
        } else if (type == TOKEN_OPERATOR || type == TOKEN_FUNCTION || type == TOKEN_UNARY_MINUS) {
            if (strcmp(rpn[i], "~") == 0) {
                if (top >= 0) {
                    stack[top] = -stack[top];
                } else {
                    *error = 1;
                }
            } else {
                double a = 0, b = 0;
                if (type == TOKEN_OPERATOR) {
                    if (top >= 1) {
                        b = stack[top--];
                        a = stack[top--];
                    } else {
                        *error = 1;
                        processed = 0;
                    }
                } else {
                    if (top >= 0) {
                        a = stack[top--];
                    } else {
                        *error = 1;
                        processed = 0;
                    }
                }

                if (processed) {
                    if (strcmp(rpn[i], "+") == 0)
                        stack[++top] = a + b;
                    else if (strcmp(rpn[i], "-") == 0)
                        stack[++top] = a - b;
                    else if (strcmp(rpn[i], "*") == 0)
                        stack[++top] = a * b;
                    else if (strcmp(rpn[i], "/") == 0) {
                        if (b == 0.0) {
                            *error = 1;
                        } else {
                            stack[++top] = a / b;
                        }
                    } else if (strcmp(rpn[i], "^") == 0)
                        stack[++top] = pow(a, b);
                    else if (strcmp(rpn[i], "sin") == 0)
                        stack[++top] = sin(a);
                    else if (strcmp(rpn[i], "cos") == 0)
                        stack[++top] = cos(a);
                    else if (strcmp(rpn[i], "tg") == 0)
                        stack[++top] = tan(a);
                    else if (strcmp(rpn[i], "ctg") == 0) {
                        if (tan(a) == 0.0) {
                            *error = 1;
                        } else {
                            stack[++top] = 1.0 / tan(a);
                        }
                    } else if (strcmp(rpn[i], "ln") == 0) {
                        if (a <= 0.0) {
                            *error = 1;
                        } else {
                            stack[++top] = log(a);
                        }
                    } else if (strcmp(rpn[i], "sqrt") == 0) {
                        if (a < 0.0) {
                            *error = 1;
                        } else {
                            stack[++top] = sqrt(a);
                        }
                    }
                }
            }
        }
        i++;
    }

    if (!*error && top >= 0) {
        result = stack[top];
    } else {
        *error = 1;
    }
    free(stack);
    return result;
}

void plot_graph(char** rpn, int rpn_count) {
    char graph[HEIGHT][WIDTH];
    int i, j;
    int error = 0;

    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            graph[i][j] = '.';
        }
    }

    for (j = 0; j < WIDTH && !error; j++) {
        double x = X_MIN + (X_MAX - X_MIN) * j / (WIDTH - 1);
        double y = evaluate_rpn(rpn, rpn_count, x, &error);

        if (!error) {
            if (y < Y_MIN) y = Y_MIN;
            if (y > Y_MAX) y = Y_MAX;

            int row = (int)round((y - Y_MIN) / (Y_MAX - Y_MIN) * (HEIGHT - 1));
            row = HEIGHT - 1 - row;

            if (row >= 0 && row < HEIGHT) {
                graph[row][j] = '*';
            }
        }
    }

    if (!error) {
        for (i = 0; i < HEIGHT; i++) {
            for (j = 0; j < WIDTH; j++) {
                putchar(graph[i][j]);
            }
            putchar('\n');
        }
    }
}
