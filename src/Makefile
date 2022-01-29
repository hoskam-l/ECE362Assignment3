# ECE362
# Assignment 2 Problem 2
# Dr. Schubert
# by luke hoskam
# Makefile for Assignment 2

##Required Targest for Assignment 2
# log:  compile the log program
# logtest: test the log program
# header: compile the header program
# headertest: test the header program
# all: compile and test both programs

# Compiler settings
CC = gcc 
CXXFLAGS = -std=c99 -Wall
VPATH = SRC INCLUDE 





APPNAME1 = timer
APPNAME2 = piper

$(APPNAME1): $(APPNAME1).o
	$(CC) $(CXXFLAGS) -o $@ $^ 

$(APPNAME1).o: $(APPNAME1).c
	$(CC) $(CXXFLAGS) -c $<




$(APPNAME2): piper.o
	$(CC) $(CXXFLAGS) -o $@ $^ 

piper.o: piper.c 
	$(CC) $(CXXFLAGS) -c $<




clean: 
	rm -f *.o *.exe piper timer

clearscr: 
	clear

