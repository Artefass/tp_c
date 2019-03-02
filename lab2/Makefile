CFLAGS=-std=gnu99 -Wall -Werror

CC=gcc

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)

clean:
	rm *.o *.txt

all: $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@
	
