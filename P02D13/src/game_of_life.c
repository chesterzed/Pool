#include <curses.h>
#include <stdio.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25

void input_file(int old_generation[][WIDTH - 2], FILE *stream);
void input_interactive(int old_generation[][WIDTH - 2]);
void game(int old_generation[][WIDTH - 2]);
int change(int row, int col, int old_generation[][WIDTH - 2]);
void draw_world(int generation[][WIDTH - 2]);
int is_all_0(int generation[][WIDTH - 2]);
void init(void);

int main(int argc, char *argv[]) {
  int old_generation[HEIGHT - 2][WIDTH - 2];
  if (argc > 1) {
    FILE *input_stream = stdin;
    input_stream = fopen(argv[1], "r");
    if (!input_stream) {
      fprintf(stderr, "Ошибка открытия файла %s\n", argv[1]);
      return 1;
    }
    input_file(old_generation, input_stream);
    fclose(input_stream);
  }
  init();
  if (argc == 1)
    input_interactive(old_generation);
  (void)freopen("/dev/tty", "r", stdin);
  game(old_generation);
  endwin();
  return 0;
}

void game(int old_generation[][WIDTH - 2]) {
  int new_generation[HEIGHT - 2][WIDTH - 2];
  int status = 1, speed = 200000;
  while (status) {
    int ch;
    draw_world(old_generation);
    usleep(speed);
    for (int row = 0; row < HEIGHT - 2; row++)
      for (int col = 0; col < WIDTH - 2; col++)
        new_generation[row][col] = change(row, col, old_generation);

    for (int p = 0; p < HEIGHT - 2; p++)
      for (int q = 0; q < WIDTH - 2; q++)
        old_generation[p][q] = new_generation[p][q];

    if (is_all_0(old_generation))
      status = 0;

    ch = getch();
    if (ch != ERR) {
      if (ch == ' ')
        status = 0;
      else if (ch == 'a' || ch == 'A')
        speed -= 20000;
      else if (ch == 'z' || ch == 'Z')
        speed += 20000;

      if (speed < 10000)
        speed = 10000;
      if (speed > 1000000)
        speed = 1000000;
    }
  }
}

int change(int row, int col, int old_generation[][WIDTH - 2]) {
  int live_neighbors = 0, new_cell = 0;
  int const d_row[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int const d_col[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
  for (int i = 0; i < 8; i++) {
    int neighbor_row = (row + d_row[i] + HEIGHT - 2) % (HEIGHT - 2);
    int neighbor_col = (col + d_col[i] + WIDTH - 2) % (WIDTH - 2);
    if (old_generation[neighbor_row][neighbor_col] == 1)
      live_neighbors++;
  }
  if ((old_generation[row][col] == 1) &&
      (live_neighbors == 2 || live_neighbors == 3))
    new_cell = 1;
  if ((old_generation[row][col] == 0) && (live_neighbors == 3))
    new_cell = 1;
  return new_cell;
}

void draw_world(int generation[][WIDTH - 2]) {
  clear();

  for (int y = -1; y <= HEIGHT - 2; y++) {
    for (int x = -1; x <= WIDTH - 2; x++) {
      if ((y >= 0 && y < HEIGHT - 2 && x >= 0 && x < WIDTH - 2)) {
        if (generation[y][x] == 1)
          addch('1');
        else
          addch(' ');
      }

      else {
        if (y == -1 || y == HEIGHT - 2)
          addch('-');
        else
          addch('|');
      }
    }

    addch('\n');
  }
  refresh();
}

int is_all_0(int generation[][WIDTH - 2]) {
  int flag = 1;
  for (int i = 0; i < HEIGHT - 2; i++)
    for (int j = 0; j < WIDTH - 2; j++)
      if (generation[i][j] != 0)
        flag = 0;

  return flag;
}

void input_file(int old_generation[][WIDTH - 2], FILE *stream) {
  int ch;
  for (int i = 0; i < HEIGHT - 2; i++) {
    for (int j = 0; j < WIDTH - 2; j++) {
      do {
        ch = fgetc(stream);

      } while (ch != '0' && ch != '1');

      old_generation[i][j] = (ch == '1') ? 1 : 0;
    }
  }
}

void input_interactive(int old_generation[][WIDTH - 2]) {
  int ch;
  for (int i = 0; i < HEIGHT - 2; i++) {
    for (int j = 0; j < WIDTH - 2; j++) {
      do {
        ch = getch();
      } while (ch != '0' && ch != '1');
      old_generation[i][j] = (ch == '1') ? 1 : 0;
    }
  }
}

void init(void) {
  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  curs_set(0);
  keypad(stdscr, true);
}
