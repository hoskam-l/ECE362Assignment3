// ECE362
// Assignment 3 part 1
// Dr. Schubert
// by luke hoskam
// by Hayden Galante
// Makefile for Assignment 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFSIZE 10

const char *readSTDIN()
{
    int n;
    char buf[BUFFSIZE];
    char *inputElements = malloc(sizeof(char));
    int stringElements = 0;
    int elementCount = 0;


    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) // Read from STDIN and set the number read to n
    {
        if (n < 0)
        {
            perror("read error");
        }
        for (int i = 0; i < n; i++)
        {
            if (isspace(buf[i]))
            {
                if (!isspace(inputElements[stringElements - 1]))
                {
                    inputElements[stringElements++] = ' ';
                    inputElements = realloc(inputElements, stringElements* sizeof(char));
                    elementCount++;
                }
            }
            else
            {
                inputElements[stringElements++] = buf[i];
                inputElements = realloc(inputElements, stringElements* sizeof(char));
                
            }
        }
    }
    inputElements[stringElements] = '\0';
    
    printf("%d elements\n", elementCount);
    return (const char*)inputElements;
}

int main(int argc, char *argv[])
{

    
    const char *inputElements = readSTDIN();
    printf("string array: %s\n", inputElements);
    return 0;
}