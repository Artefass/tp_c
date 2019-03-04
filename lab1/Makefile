CFALGS = -std=c99 -Wall -Werror

TESTFLAGS = -lcheck -lm -lpthread -lrt -lsubunit
VALGRINDPARAMS = --leak-check=full --undef-value-errors=yes
VALGRINDFLAGS  =  -g -O0

CC = gcc

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

TARGET = main.out

clean:
	rm -f *.o *.txt $(TARGET)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(VALGRINDFLAGS)

%.o: %.c
	$(CC) $(CFALGS) -c $< -o $@

run_with_valgrind:
	valgrind $(VALGRINDPARAMS) ./main.out
	
	
