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

$(APPNAME1)test: $(APPNAME1)
	./$(APPNAME1) ls -l /usr/bin
	./$(APPNAME1) cat timer.c
	./$(APPNAME1) ping google.com -c5
	./$(APPNAME1) sleep 5
	

$(APPNAME2): piper.o myTools.o
	$(CC) $(CXXFLAGS) -o $@ $^ 

$(APPNAME2).o: piper.c 
	$(CC) $(CXXFLAGS) -c $<

$(APPNAME2)test: $(APPNAME2)
	@echo Test1:
	./$(APPNAME2) <piperinput1.txt
	@echo Test2:
	./$(APPNAME2) <piperinput2.txt
	@echo Test3:
	./$(APPNAME2) <piperinput3.txt

myTools.o: include/myTools.c include/myTools.h
	gcc -c include/myTools.c

all: clean clearscr $(APPNAME1) $(APPNAME2)

clean: 
	rm -f *.o *.exe piper timer

clearscr: 
	clear

