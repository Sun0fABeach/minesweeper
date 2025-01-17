CC := gcc
CFLAGS := -W -Wall -Wextra -Wpedantic -std=c2x
LDFLAGS := -lraylib

.PHONY: debug prod clean

debug: CFLAGS += -g3
prod: CFLAGS += -O3
debug prod: minesweeper

minesweeper: ui.o tile.o textures.o
minesweeper.o: ui.h shared.h
ui.o: ui.h tile.h textures.h shared.h
tile.o: tile.h
textures.o: textures.h

clean:
	-rm *.o minesweeper
