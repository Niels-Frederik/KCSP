# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation

CC = g++
CFLAGS = -Wall -g

# ****************************************************

all: Independent_Partitioning_Algorithm Concurrent_Partitioning_Algorithm TupleGenerator

main.o: main.cpp Independent_Partitioning_Algorithm.h Concurrent_Partitioning_Algorithm.h TupleGenerator.h
	$(CC) $(CFLAGS) -c main.cpp

Independent_Partitioning_Algorithm.o: Independent_Partitioning_Algorithm.h
Concurrent_Partitioning_Algorithm.o: Concurrent_Partitioning_Algorithm.h
TupleGenerator.o: TupleGenerator.h
