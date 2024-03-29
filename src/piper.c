// Assignment 3 part 2
// Dr. Schubert
// by luke hoskam
// by Hayden Galante
// Piper Program for Assignment 3
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>

#define MAX_BUFFER_SIZE 10
#define FD_ARRAY_SIZE 2
#define PIPE_INPUT 1
#define PIPE_OUTPUT 0
#define MAX_APPEND_STR_SIZE 32
#define BUFFSIZE 10
#define BAD_READ "read error\n"
#define BAD_WRITE "write error\n"
#define BAD_CLOSE "close error\n"
#define BAD_OPEN "open error\n"
#define BAD_FORK "fork error\n"
#define BAD_ALLOC "allocate error\n"
#define NEW_LINE "\n"
// functions defined to from assingment2 solution
#define Write(f, b, s)         \
    if (write(f, b, s) != s)   \
    {                          \
        err_out(BAD_WRITE, 1); \
    }
#define Pipe(fd)              \
    if (pipe(fd) < 0)         \
    {                         \
        err_out(BAD_OPEN, 1); \
    }
#define Close(fd)              \
    if (close(fd) < 0)         \
    {                          \
        err_out(BAD_CLOSE, 1); \
    }

const char *readSTDIN(int *count);
void printLine(const char *line);
int *strToIntArray(char *string, int count);
void addNumToString(const char beginString[], int number);
void err_out(char *msg, int printErrNo);
//--------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    int elementCount;
    const char *inputElements = readSTDIN(&elementCount);
    int fd[FD_ARRAY_SIZE], fd1[FD_ARRAY_SIZE];
    int n;
    char buf[MAX_BUFFER_SIZE];
    //char buf[MAX_BUFFER_SIZE];

    // set up the pipe

    Pipe(fd);
    Pipe(fd1);
    Write(fd[PIPE_INPUT], inputElements, strlen(inputElements));
    Close(fd[PIPE_INPUT]);
    Write(fd1[PIPE_INPUT], inputElements, strlen(inputElements));
    Close(fd1[PIPE_INPUT]);

    // here is the fork
    pid_t child1PID, child2PID;
    child1PID = fork();
    if (child1PID < 0)
    {
        // Error
        err_out(BAD_FORK, 0);
    }
    // child process execution 1
    if (child1PID == 0)
    {
        char *charElements1;
        int runOnce = 0;
        int totalRead = 0;
        while ((n = read(fd[PIPE_OUTPUT], buf, MAX_BUFFER_SIZE)) > 0)
        {

            if (runOnce == 0)
            {
                charElements1 = (char *)malloc(sizeof(char) * n + 1);
                strncpy(charElements1, buf, n);
                runOnce++;
            }
            else
            {
                charElements1 = (char *)realloc(charElements1, (sizeof(char) * n) + strlen(charElements1));
                strncat(charElements1, buf, n);
            }
            totalRead = totalRead + n;
        }
        if (n < 0)
        {
            err_out(BAD_READ, 0);
        }
        // cleanup
        Close(fd[PIPE_OUTPUT]);
        buf[n] = '\0'; // terminate the string
        // change from string to an array of ints
        int *intElementsReturned1 = strToIntArray(charElements1, elementCount);
        int sum = 0;
        for (int i = 0; i < elementCount; i++)
        {
            sum = sum + intElementsReturned1[i];
        }
        addNumToString("Sum: ", sum);
    }
    // creates second fork off main
    if (child1PID > 0)
    {
        child2PID = fork();
        if (child2PID < 0)
        {
            // Error
            err_out(BAD_FORK, 0);
        }
    }
    // child process execution 2
    if (child2PID == 0 && child1PID != 0)
    {
        char *charElements2;
        int runOnce = 0;
        int totalRead = 0;
        while ((n = read(fd1[PIPE_OUTPUT], buf, MAX_BUFFER_SIZE)) > 0)
        {

            if (runOnce == 0)
            {
                charElements2 = (char *)malloc(sizeof(char) * n + 1);
                strncpy(charElements2, buf, n);
                runOnce++;
            }
            else
            {
  
                charElements2 = (char *)realloc(charElements2, (sizeof(char) * n) + strlen(charElements2));
                strncat(charElements2, buf, n);
            }
            totalRead = totalRead + n;
        }
   
        if (n < 0)
        {
            err_out(BAD_READ, 0);
        }
        // cleanup
        Close(fd1[PIPE_OUTPUT]);
        int product = 1;
        int *intElementsReturned2 = strToIntArray(charElements2, elementCount);
        for (int i = 0; i < elementCount; i++)
        {
            product = product * intElementsReturned2[i];
        }
        addNumToString("Product: ", product);
    }

    // parent process execution
    else if (child1PID > 0 && child2PID > 0)
    {
        wait(NULL);
    }

    return 0;
}
// Reads STDIN and returns a string and passes the number of elements back by pointer
const char *readSTDIN(int *count)
{
    int n, i;
    char buf[BUFFSIZE];
    char *inputElements = malloc(sizeof(char));
    int stringElements = 0;
    int elementCount = 0;

    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) // Read from STDIN and set the number read to n
    {
        if (n < 0)
        {
            err_out(BAD_READ, 1);
        }
        for (i = 0; i < n; i++)
        { // isspace finds ' ' and '\n' when found put a space in our string else put the char in the string
            if (isspace(buf[i]))
            {
                if (!isspace(inputElements[stringElements - 1]))
                {
                    inputElements[stringElements++] = ' ';
                    inputElements = realloc(inputElements, stringElements * sizeof(char));
                    elementCount++;
                }
            }
            else
            {
                inputElements[stringElements++] = buf[i];
                inputElements = realloc(inputElements, stringElements * sizeof(char));
            }
        }
    }
    // Add termiating char
    inputElements[stringElements] = '\0';
    // sets the pointer of elementCout to return and retuns the string
    *count = elementCount;
    return (const char *)inputElements;
}

// printLine function will print the line sent to STDOUT and add a '/n' to start a new line
void printLine(const char *line)
{
    int n = strlen(line);
    Write(STDOUT_FILENO, line, n);
    Write(STDOUT_FILENO, NEW_LINE, strlen(NEW_LINE));
}
// takes the input of a string of numbers seperated by a space and returns an integer array
int *strToIntArray(char *string, int count)
{
    int *intElements = (int *)malloc((sizeof(int) * count));
    if (intElements == NULL)
    {
        err_out(BAD_ALLOC, 0);
    }
    char *token;
    const char delim[] = " ";
    token = strtok(string, delim);
    int i = 0;
    while (token != NULL)
    {
        intElements[i] = atoi(token);
        token = strtok(NULL, delim);
        i++;
    }
    return (int *)intElements;
}
// appends a number to a string and prints the string
void addNumToString(const char beginString[], int number)
{
    char *tgt = (char *)malloc(sizeof(char) * MAX_APPEND_STR_SIZE);
    if (tgt == NULL)
    {
        err_out(BAD_ALLOC, 0);
    }
    snprintf(tgt, MAX_APPEND_STR_SIZE, "%s %d", beginString, number);
    // send to the printLine function
    printLine(tgt);
}

// From assingment2 solution
void err_out(char *msg, int printErrNo)
{
    char buffer[MAX_BUFFER_SIZE];
    strcpy(buffer, msg);
    if (printErrNo == 1)
    {
        strcat(buffer, strerror(errno));
    }
    strcat(buffer, "\n");
    write(STDERR_FILENO, buffer, strlen(buffer));
    exit(-1);
}