# Lines starting with # are comments

# Some variable definitions to save typing later on
CC = g++
CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
#DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) 


hw5: hw5.o modelBuilder.o
	$(CC) -o hw5 hw5.o modelBuilder.o

hw5.o: hw5.cpp modelBuilder.h
	$(CC) $(CFLAGS) -c hw5.cpp

# Compiles dnasearch.c into an object file
modelBuilder.o: modelBuilder.cpp modelBuilder.h
	$(CC) $(CFLAGS) -c modelBuilder.cpp


# 'make clean' will remove intermediate & executable files
clean:
	rm -f *.o hw3 modelBuilder  *.gcov
