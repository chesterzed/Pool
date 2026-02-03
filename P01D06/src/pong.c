#include <stdio.h>

void clear_screen() {
    for (int i = 0; i < 50; i++) {
        printf("\n");
    }
}

void field(int size_x, int size_y, int racket_left, int racket_right, int ballY, int ballX, int score_left,
           int score_right) {
    int width = size_x;
    int height = size_y;
    int mid_x = (width - 2) / 2;

    printf("%39d:%d\n", score_left, score_right);
    printf("+");
    for (int i = 0; i < width - 2; i++) {
        printf("=");
    }
    printf("+\n");

    for (int i = 0; i < height - 2; i++) {
        printf("|");
        for (int j = 0; j < width - 2; j++) {
            if (j == 1 && (i == racket_left + 1 || i == racket_left - 1 || i == racket_left)) {
                printf("\033[31m]\033[0m");
            } else if (j == width - 4 &&
                       (i == racket_right + 1 || i == racket_right - 1 || i == racket_right)) {
                printf("\033[34m[\033[0m");
            } else if (i == ballY && j == ballX) {
                printf("\033[32mo\033[0m");
            } else if (j == mid_x) {
                printf("|");
            } else {
                printf(" ");
            }
        }
        printf("|\n");
    }

    printf("+");
    for (int i = 0; i < width - 2; i++) {
        printf("=");
    }
    printf("+\n");
}

int main() {
    const int FIELD_HEIGHT = 25;
    const int FIELD_WIDTH = 80;
    int racket_left = (FIELD_HEIGHT - 1) / 2;
    int racket_right = racket_left;
    int ballX = 5, ballY = racket_left;
    int ball_moveX = 1;
    int ball_moveY = 1;
    int score_left = 0;
    int score_right = 0;

    while (score_left != 21 && score_right != 21) {
        clear_screen();
        field(FIELD_WIDTH, FIELD_HEIGHT, racket_left, racket_right, ballY, ballX, score_left, score_right);

        printf("Управление: A/K (вверх), Z/M (вниз), Q (выход)\n");
        printf("> ");

        char input;
        char next_ch;
        scanf("%c%c", &input, &next_ch);
        if (next_ch != '\n') {
            while (next_ch != '\n') {
                next_ch = getchar();
            }
        }

        if (input == 'q' || input == 'Q') {
            return 0;
        } else if (input == 'a' || input == 'A') {
            if (racket_left > 1) racket_left--;
        } else if (input == 'z' || input == 'Z') {
            if (racket_left < FIELD_HEIGHT - 4) racket_left++;
        } else if (input == 'k' || input == 'K') {
            if (racket_right > 1) racket_right--;
        } else if (input == 'm' || input == 'M') {
            if (racket_right < FIELD_HEIGHT - 4) racket_right++;
        } else if (input != ' ') {
            continue;
        }

        ballX += ball_moveX;
        ballY += ball_moveY;

        if (ballY == FIELD_HEIGHT - 3 || ballY == 0) {
            ball_moveY *= -1;
        }

        if (ballX + 1 == FIELD_WIDTH - 4 &&
            (ballY == racket_right || ballY == racket_right + 1 || ballY == racket_right - 1)) {
            ball_moveX *= -1;
            if (ballY == racket_right) {
                ball_moveY = 0;
            } else if (ballY == racket_right + 1) {
                ball_moveY = 1;
            } else if (ballY == racket_right - 1) {
                ball_moveY = -1;
            }
        }

        if (ballX - 1 == 3 &&
            (ballY == racket_left || ballY == racket_left + 1 || ballY == racket_left - 1)) {
            ball_moveX *= -1;
            if (ballY == racket_left) {
                ball_moveY = 0;
            } else if (ballY == racket_left - 1) {
                ball_moveY = -1;
            } else if (ballY == racket_left + 1) {
                ball_moveY = 1;
            }
        }

        if (ballX >= 79) {
            score_left++;
            ballX = 75;
            ballY = racket_right;
            ball_moveX = -1;
            ball_moveY = 0;
        }

        if (ballX <= 0) {
            score_right++;
            ballX = 4;
            ballY = racket_left;
            ball_moveX = 1;
            ball_moveY = 0;
        }
    }

    clear_screen();
    if (score_left == 21)
        printf("\033[31mPlayer 1 win's!\033[0m\n");
    else
        printf("\033[34mPlayer 2 win's!\033[0m\n");

    getchar();
    return 0;
}
