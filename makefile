## This is a simple Makefile 

# Define what compiler to use and the flags.
CC=cc
CXX=g++
CCFLAGS= -g -std=c++11 -Wall -Werror
LDLIBS= -lm
#all: test_list
all: proj

# Compile all .c files into .o files
# % matches all (like * in a command)
# $< is the source file (.c file)
%.o : %.cpp
	$(CXX) -c $(CCFLAGS) $<

proj.o: proj.cpp file_input.h trace.h
file_input.o: file_input.cpp trace.h
trace.o: trace.cpp trace.h

###################################
# BEGIN SOLUTION
proj: proj.o file_input.o trace.o
	$(CXX) -o proj proj.o file_input.o trace.o $(CCFLAGS) $(LDLIBS)


clean:
	rm -f core *.o proj
