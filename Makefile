CC := gcc
CFLAGS := -W -Wall -Wextra -Wpedantic -std=c2x
LDFLAGS := -lraylib

.PHONY: debug prod clean

debug: CFLAGS += -g3
prod: CFLAGS += -O3
debug prod: minesweeper

minesweeper: ui.o tile.o smiley.o options.o digital_digits.o board.o \
	board_tile.o textures.o
minesweeper.o: ui.h shared.h
ui.o: ui.h tile.h smiley.h options.h digital_digits.h board.h board_tile.h \
	textures.h shared.h
tile.o: tile.h
smiley.o: smiley.h textures.h tile.h
options.o: options.h tile.h textures.h shared.h
digital_digits.o: digital_digits.h textures.h
board.o: board.h board_tile.h tile.h shared.h
board_tile.o: board_tile.h tile.h textures.h shared.h
textures.o: textures.h

clean:
	-rm *.o minesweeper
