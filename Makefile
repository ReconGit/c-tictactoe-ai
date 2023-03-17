CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -pedantic
LDFLAGS := -lm

SRCS := src/ui.c src/tictactoe.c src/minimax.c
OBJS := $(SRCS:%.c=obj/%.o)

all: obj game_only tests_only
game: obj game_only
tests: obj tests_only

game_only: $(OBJS) obj/main.o
	$(CC) $^ -o bin/game $(LDFLAGS)
tests_only: $(OBJS) obj/tests.o
	$(CC) $^ -o bin/tests $(LDFLAGS)

obj/main.o: src/main.c
	cppcheck --enable=performance --error-exitcode=1 $<
	$(CC) $(CFLAGS) -c $< -o obj/main.o
obj/tests.o: tests/tests.c tests/greatest.h
	cppcheck --enable=performance --error-exitcode=1 $<
	$(CC) $(CFLAGS) -c $< -o obj/tests.o

obj:
	mkdir -p obj bin obj/src
obj/%.o: %.c
	cppcheck --enable=performance --error-exitcode=1 $<
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj bin
