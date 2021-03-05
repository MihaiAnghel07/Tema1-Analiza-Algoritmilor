CC=gcc
CFLAGS=-std=c99 -Wall
EXE=algo

build: algo.c
	$(CC) $^ $(CFLAGS) -o $(EXE)

run: $(EXE)
	./$(EXE)

clean:
	rm -f $(EXE)

.PHONY: clean run
