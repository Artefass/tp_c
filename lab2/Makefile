CFLAGS = -std=c99 -Wall -Werror -D_POSIX_C_SOURCE=200809L

CC=gcc

TESTFLAGS = -lcheck -pthread  -lrt -lm -lsubunit -g -O0
VALGRINDPARAMS = --leak-check=full --undef-value-errors=yes

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)



TESTSRC=$(wildcard test/*.c)
TESTOBJ=$(TESTSRC:.c=.o)

TOBJECTS=$(filter-out main.o, $(OBJECTS))

TARGET=main

clean:
	rm -f *.o $(TARGET) *.out
	rm -rf ./test/*.o ./test/tests 

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

mkdir_test:
	mkdir -p test

compile_tests: $(TESTOBJ) $(TOBJECTS)
	$(CC) -o test/tests $^ $(TESTFLAGS)
	

run_tests: compile_tests
	./test/tests

run_tests_with_valgrind: compile_tests
	valgrind $(VALGRINDPARAMS) ./test/tests
	

	
