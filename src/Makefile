# ECE362
# Assignment 3 
# Dr. Schubert
# by luke hoskam
# by Hayden Galante
# Makefile for Assignment 3

##Required Targest for Assignment 3
# timer:  compile the timer program
# timertest: test the timer program
# piper: compile the piper program
# pipertest: test the piper program
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

