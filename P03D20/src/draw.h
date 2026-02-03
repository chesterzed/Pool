#ifndef DRAW_H
#define DRAW_H

#include <stdio.h>
#include <stdlib.h>
#define WIDTH 80
#define HEIGHT 25
#define X_MIN 0
#define X_MAX (4 * M_PI)
#define Y_MIN -1
#define Y_MAX 1

double evaluate_rpn(char** rpn, int count, double x, int* error);
void plot_graph(char** rpn, int rpn_count);

#endif