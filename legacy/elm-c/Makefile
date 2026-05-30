CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -g

all: bin/counter

bin/counter: examples/counter/main.c src/elm_c.c
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ examples/counter/main.c src/elm_c.c

clean:
	rm -f bin/counter examples/counter/*.o
	rm -rf bin/

run: bin/counter
	./bin/counter

.PHONY: all clean run