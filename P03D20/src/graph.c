#include "draw.h"
#include "io.h"
#include "parser.h"
#include "shunting_yard.h"

int main() {
    char input_arr[MAX_SIZE];
    char** tokens = NULL;
    char** rpn = NULL;
    int count = 0;
    int rpn_count = 0;
    if (input(input_arr)) {
        printf("n/a");
        return 1;
    }
    input_arr[strcspn(input_arr, "\n")] = '\0';
    tokens = tokenize(input_arr, &count);
    if (tokens == NULL || count == 0) {
        printf("n/a\n");
        return 1;
    }

    if (!validate_expression(tokens, count)) {
        printf("n/a\n");
        for (int i = 0; i < count; i++) {
            free(tokens[i]);
        }
        free(tokens);
        return 1;
    }
    rpn = shunting_yard(tokens, count, &rpn_count);
    if (rpn == NULL) {
        printf("n/a\n");
        for (int i = 0; i < count; i++) {
            free(tokens[i]);
        }
        free(tokens);
        return 1;
    }
    double test_x = 0.0;
    int test_error = 0;
    evaluate_rpn(rpn, rpn_count, test_x, &test_error);
    if (test_error) {
        printf("n/a\n");
    } else {
        plot_graph(rpn, rpn_count);
    }

    for (int i = 0; i < count; i++) {
        free(tokens[i]);
    }
    free(tokens);

    for (int i = 0; i < rpn_count; i++) {
        free(rpn[i]);
    }
    free(rpn);

    return 0;
}