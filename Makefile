CC := gcc
CFLAGS := -W -Wall -Wextra -Wpedantic -std=c2x
LDFLAGS := -lraylib

.PHONY: debug prod clean

debug: CFLAGS += -g3
prod: CFLAGS += -O3
debug prod: minesweeper

minesweeper: ui.o
ui.o: ui.h

clean:
	-rm *.o minesweeper
