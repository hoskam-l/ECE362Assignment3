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
    char stringArray[BUFFSIZE];
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
                if (stringArray[i - 1] != ' ')
                {
                    stringArray[elementCount++] = ' ';
                }
            }
            else
            {
                stringArray[elementCount++] = buf[i];
            }
        }
    }
    printf("%d elements\n", elementCount);
    printf("string array: %s\n", stringArray);
    return 0;
}