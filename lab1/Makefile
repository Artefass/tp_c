CFALGS = -std=gnu99 -Wall -Werror

TESTFLAGS = -lcheck -lm -lpthread -lrt -lsubunit

CC = gcc

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

testSrc = $(filter-out main.c, $(SOURCES))
testCaseSrc = $(wildcard ./test/*.c)
testCaseObj = $(testCaseSrc:.c=.o) 

clean:
	rm -f *.o *.txt
	rm -f test/*.o test/test

all: $(OBJECTS)

%.o: %.c
	$(CC) $(CFALGS) -c $< -o $@

make_test_dir:
	mkdir -p ./test

compile_test: $(testSrc) $(testCaseObj)
	$(CC) -o test/test $^ $(TESTFLAGS)

test: make_test_dir compile_test
	./test/test

compile_simple_test: $(testCaseObj)
	$(CC) -o test/test $^ $(TESTFLAGS)

simple_test: make_test_dir compile_simple_test
	./test/test
	
	
