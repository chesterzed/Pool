#include "io.h"

int input(char* str) {
    int flag = 0;
    if (fgets(str, MAX_SIZE, stdin) == NULL) {
        flag = 1;
    }
    return flag;
}