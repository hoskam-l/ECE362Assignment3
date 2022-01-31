#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "include/myTools.h"

#define MAX_BUFFER_SIZE 1024
#define FD_ARRAY_SIZE 2
#define PIPE_INPUT 1
#define PIPE_OUTPUT 0



//--------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    int elementCount;
    const char *inputElements = readSTDIN(&elementCount);
    // printf("string array: %s\n", inputElements);
    // printf("number of elements: %d\n", elementCount);
    int fd[FD_ARRAY_SIZE], fd1[FD_ARRAY_SIZE];
    int n, m;
    char buf[MAX_BUFFER_SIZE];
    char buf2[MAX_BUFFER_SIZE];

    // set up the pipe

    pipe(fd);
    if (pipe(fd) < 0)
    {
        perror("An error occured when opening the pipe.");
        return 1;
    }
    pipe(fd1);
    if (pipe(fd) < 0)
    {
        perror("An error occured when opening second pipe.");
        return 2;
    }

    // write to pipe
    write(fd[PIPE_INPUT], inputElements, strlen(inputElements));
    close(fd[PIPE_INPUT]);

    write(fd1[PIPE_INPUT], inputElements, strlen(inputElements));
    close(fd1[PIPE_INPUT]);

    // here is the fork
    pid_t child1_pid, child2_pid;
    child1_pid = fork();
    if (child1_pid < 0)
    {
        // Error
        perror("fork failed");
        exit(-1);
    }
    // child process execution 1
    if (child1_pid == 0)
    {
        wait(NULL);
        // printf("hello world from child process child1_pid: %d \n", getpid());
        if ((n = read(fd[PIPE_OUTPUT], buf, MAX_BUFFER_SIZE)) >= 0)
        {
            buf[n] = '\0'; // terminate the string
            // printf("read %d bytes from the pipe: %s\n", n, buf);
            int *intElementsReturned1 = strToIntArray(buf, elementCount);
            int sum = 0;
            for (int i = 0; i < elementCount; i++)
            {
                sum = sum + intElementsReturned1[i];
            }
            addNumToString("Sum: ", sum);
           
            //printf("sum: %d\n", sum);
            close(fd[PIPE_OUTPUT]);
        }
    }
    // creates second fork off main
    if (child1_pid > 0)
    {
        child2_pid = fork();
        if (child2_pid < 0)
        {
            // Error
            perror("fork failed");
            exit(-1);
        }
        wait(NULL);
    }
    // child process execution 2
    if (child2_pid == 0 && child1_pid != 0)
    {
        wait(NULL);
        // printf("hello world from child process child2_pid: %d \n", getpid());
        if ((m = read(fd1[PIPE_OUTPUT], buf2, MAX_BUFFER_SIZE)) >= 0)
        {
            buf[m] = '\0'; // terminate the string
            // printf("read %d bytes from the pipe: %s \n", m, buf2);
            int *intElementsReturned2 = strToIntArray(buf2, elementCount);
            int product = 1;
            for (int i = 0; i < elementCount; i++)
            {
                product = product * intElementsReturned2[i];
            }
            addNumToString("Product: ", product);

            //printf("product: %d\n", product);
            close(fd1[PIPE_OUTPUT]);
        }
    }
    // parent process execution
    else if (child1_pid > 0 && child2_pid > 0)
    {
        wait(NULL);
        // printf("hello world from parent process child1_pid: %d \n", getpid());
    }

    return 0;
}
