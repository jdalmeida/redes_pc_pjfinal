# Makefile para compilação do projeto de rede
# @author João Gabriel de Almeida

CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = rede
SOURCES = main.c grafo.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

