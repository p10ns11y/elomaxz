CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -g

SRC = src/elomaxz.c
OBJ = $(SRC:.c=.o)
LIB = libelomaxz.a

all: $(LIB) bin/counter

$(LIB): $(OBJ)
	ar rcs $@ $^

src/%.o: src/%.c include/elomaxz.h
	$(CC) $(CFLAGS) -c $< -o $@

bin/counter: examples/counter/main.c $(LIB)
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ examples/counter/main.c $(LIB)

clean:
	rm -f $(OBJ) $(LIB) bin/counter examples/counter/*.o
	rm -rf bin/

run: bin/counter
	./bin/counter

.PHONY: all clean run