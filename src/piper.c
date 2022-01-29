#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFSIZE 10

int main(int argc, char *argv[])
{
    int n;
    char buf[BUFFSIZE];
    char **stringArray;
    int elementCount = 0;

    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) // Read from STDIN and set the number read to n
    {
        if(n <0)
        {
            perror("read error");
        }
        for (int i = 0; i<n; i++)
        {
            if(buf[i] == ' ' || buf[i] == '\n')
            {
                elementCount++;
            }
        }
    }
    printf("%d elements\n", elementCount);
    return 0;
}