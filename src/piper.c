#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFSIZE 5000

int main(int argc, char *argv[])
{
    int n;
    char buf[BUFFSIZE];
    char *inputElements = malloc(sizeof(char));
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
                if (!isspace(inputElements[elementCount - 1]))
                {
                    inputElements[elementCount++] = ' ';
                    inputElements = realloc(inputElements, elementCount* sizeof(char));
                }
            }
            else
            {
                inputElements[elementCount++] = buf[i];
                inputElements = realloc(inputElements, elementCount* sizeof(char));
            }
        }
    }
    printf("%d elements\n", elementCount);
    printf("string array: %s\n", inputElements);
    return 0;
}