CFLAGS=-std=c99 -Wall -Werror -D_POSIX_C_SOURCE=200809L

CC=gcc

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)

TARGET=main

clean:
	rm *.o *.txt
	rm ./test/*.o 

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
	
