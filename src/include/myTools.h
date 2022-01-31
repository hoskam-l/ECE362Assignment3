#ifndef _MYTOOLS_H
#define _MYTOOLS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define BUFFSIZE 10

const char *readSTDIN(int *count);
void printLine(const char *line);
int *strToIntArray(char *string, int count);
char *itoa(int value, char *result, int base);
void addNumToString(const char beginString[], int number);

#endif // !_MYTOOLS_H


