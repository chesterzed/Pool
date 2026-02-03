all: build

build: graph.c draw.c parser.c stack.c io.c shunting_yard.c
	gcc -Wall -Werror -Wextra -std=c11 graph.c draw.c parser.c stack.c io.c shunting_yard.c -o ../build/graph -lm

checking_all: checking leaks

checking:
	clang-format -n *.c
	clang-format -n *.h
	cppcheck --enable=all --suppress=missingIncludeSystem *.c

leaks:
	leaks -atExit -- ./../build/graph

clean:
	rm -rf ../build/graph

rebuild: clean all