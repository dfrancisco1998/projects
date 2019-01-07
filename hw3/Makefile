# Lines starting with # are comments

# Some variable definitions to save typing later on
CC = gcc
CONSERVATIVE_FLAGS = -std=c99 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)
bin: hw3 test_dnasearch

hw3: hw3.o dnasearch.o
	$(CC) -o hw3 hw3.o dnasearch.o

hw3.o: hw3.c dnasearch.h
	$(CC) $(CFLAGS) -c hw3.c

# Links the test_dnasearch executable
test_dnasearch: test_dnasearch.o dnasearch.o
	$(CC) -o test_dnasearch test_dnasearch.o dnasearch.o

# Compiles dnasearch.c into an object file
dnasearch.o: dnasearch.c dnasearch.h
	$(CC) $(CFLAGS) -c dnasearch.c

# Compiles test_dnasearch.c into an object file
test_dnasearch.o: test_dnasearch.c dnasearch.h
	$(CC) $(CFLAGS) -c test_dnasearch.c

# 'make clean' will remove intermediate & executable files
clean:
	rm -f *.o test_dnasearch hw2 *.gcov
