# LSK: Lattay's Starter Kit
# Default makefile for LSK project
# Use 'make' to build, 'make run' to build and run the game, 'make debug' (needs gdb) to enter debugger
# 'make clean' to remove build temporary files
# 'make wipe' to go back to source only state

CC=gcc
DB=gdb
DEBUG=1

CFLAGS=-Wall -Wextra -Wpedantic --std=c11

ifeq ($(DEBUG),0)
	CFLAGS+=-O3
else
	CFLAGS+=-g
endif

LFLAGS=-lraylib -lm
EXE=fleetd.exe
SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c,build/%.o,$(SRC))

.PHONY: all clean wipe

all: $(EXE)

run: $(EXE)
	./$(EXE)


debug: $(EXE)
	$(DB) ./$(EXE)

$(EXE): $(OBJ)
	$(CC) $(LFLAGS) $^ -o $@

build/%.o: src/%.c src/%.h | build/
	$(CC) $(CFLAGS) -c $< -o $@

tags: $(SRC)
	ctags --extras=rq $(SRC)

build/:
	mkdir -p build

clean:
	rm -rf build
	rm -f test_*

wipe: clean
	rm -f *.png
	rm -f run_test
	rm -f tags
	rm -f $(EXE)
