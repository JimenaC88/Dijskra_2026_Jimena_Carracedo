CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Werror -Wpedantic -g -O0
TARGET  = Pracatico_JimenaCarracedo
OBJS    = main.o grafo.o dijkstra.o enrutamiento.o tests.o carga.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

test: all
	./$(TARGET) --test

clean:
	rm -f $(OBJS) $(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./$(TARGET)

.PHONY: all run test clean valgrind
