#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>

#define MAX_BUFFER_SIZE 1024
#define FD_ARRAY_SIZE 2
#define PIPE_INPUT 1
#define PIPE_OUTPUT 0
#define MAX_APPEND_STR_SIZE 32
#define BUFFSIZE 10
#define BAD_READ  "read error\n"
#define BAD_WRITE "write error\n"
#define BAD_INPUT "input error\n"
#define BAD_CLOSE "close error\n"
#define BAD_OPEN "open error\n"
#define BAD_FORK "fork error\n"
#define BAD_ALLOC "allocate error\n"
#define NEW_LINE "\n"


#define Write(f,b,s) if(write(f,b,s) !=s) {err_out(BAD_WRITE,1);}
#define Pipe(fd) if(pipe(fd) < 0) {err_out(BAD_OPEN, 1);}
#define Close(fd) if(close(fd) < 0) {err_out(BAD_CLOSE, 1);}

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
    int n, m;
    char buf[MAX_BUFFER_SIZE];
    char buf2[MAX_BUFFER_SIZE];


    // set up the pipe

    Pipe(fd);
    // pipe(fd);
    // if (pipe(fd) < 0)
    // {
    //     perror("An error occured when opening the pipe.");
    //     return 1;
    // }
    Pipe(fd1);
    // pipe(fd1);
    // if (pipe(fd1) < 0)
    // {
    //     perror("An error occured when opening second pipe.");
    //     return 2;
    // }

    // write to pipe
    Write(fd[PIPE_INPUT], inputElements, strlen(inputElements));
    // int c = write(fd[PIPE_INPUT], inputElements, strlen(inputElements));
    // if (c == -1)
    // {
    //     perror("An error occured writing to pipe 1");
    //     return 3;
    // }
    Close(fd[PIPE_INPUT]);
    //close(fd[PIPE_INPUT]);
    Write(fd1[PIPE_INPUT], inputElements, strlen(inputElements));
    // int d = write(fd1[PIPE_INPUT], inputElements, strlen(inputElements));
    // if (d == -1)
    // {
    //     perror("An error occured writing to pipe 2");
    //     return 4;
    // }
    Close(fd1[PIPE_INPUT]);
    //close(fd1[PIPE_INPUT]);

    // here is the fork
    pid_t child1_pid, child2_pid;
    child1_pid = fork();
    if (child1_pid < 0)
    {
        // Error
        err_out(BAD_FORK,0);
        // perror("fork failed");
        // exit(-1);
    }
    // child process execution 1
    if (child1_pid == 0)
    {
        wait(NULL);
        if ((n = read(fd[PIPE_OUTPUT], buf, MAX_BUFFER_SIZE)) >= 0)
        {
            if (n <0)
            {
                err_out(BAD_READ,0);
                // perror("An error occured reading from pipe 1");
                // return 5;
            }

            buf[n] = '\0'; // terminate the string
            int *intElementsReturned1 = strToIntArray(buf, elementCount);
            int sum = 0;
            for (int i = 0; i < elementCount; i++)
            {
                sum = sum + intElementsReturned1[i];
            }
            addNumToString("Sum: ", sum);

            Close(fd[PIPE_OUTPUT]);
            // if(close(fd[PIPE_OUTPUT]))
            // {
            //     perror("ERROR CLOSING PIPE FOR CHILD1");
            //     exit(-1);

            // }
        }
    }
    // creates second fork off main
    if (child1_pid > 0)
    {
        child2_pid = fork();
        if (child2_pid < 0)
        {
            // Error
            err_out(BAD_FORK,0);
            // perror("fork failed");
            // exit(-1);
        }
        wait(NULL);
    }
    // child process execution 2
    if (child2_pid == 0 && child1_pid != 0)
    {
        wait(NULL);
        if ((m = read(fd1[PIPE_OUTPUT], buf2, MAX_BUFFER_SIZE)) >= 0)
        {
            if (m <0)
            {
                perror("An error occured reading from pipe 2");
                return 6;
            }

            buf[m] = '\0'; // terminate the string
            int *intElementsReturned2 = strToIntArray(buf2, elementCount);
            int product = 1;
            for (int i = 0; i < elementCount; i++)
            {
                product = product * intElementsReturned2[i];
            }
            addNumToString("Product: ", product);

            Close(fd1[PIPE_OUTPUT]);
            // if(close(fd1[PIPE_OUTPUT])<0) {
            //     perror("ERROR CLOSING PIPE FOR CHILD2");
            //     exit(-1);

            // }

        }
    }
    // parent process execution
    else if (child1_pid > 0 && child2_pid > 0)
    {
        wait(NULL);
    }

    return 0;
}

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
            err_out(BAD_READ,1);
        }
        for (i = 0; i < n; i++)
        {
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
    inputElements[stringElements] = '\0';

    *count = elementCount;
    return (const char *)inputElements;
}

// printLine function will print the line sent to STDOUT and add a '/n' to start a new line
void printLine(const char *line)
{
    int n = strlen(line);

    Write(STDOUT_FILENO,line,n);
    Write(STDOUT_FILENO,NEW_LINE,strlen(NEW_LINE));

    // if (write(STDOUT_FILENO, line, n) != n)
    // {
    //     perror("write error");
    //     exit(-1);
    // }
    // // Since printLine we send a \n
    // if (write(STDOUT_FILENO, "\n", 2) != 2)
    // {
    //     perror("write error");
    //     exit(-1);
    // }
}

int *strToIntArray(char *string, int count)
{
    int *intElements = (int *)malloc((sizeof(int) * count));
    if (intElements == NULL)
    {
        err_out(BAD_ALLOC,0);
        // perror("ERROR allocating memory.");
        // exit(0);
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

void addNumToString(const char beginString[], int number)
{
    // constant for first prart of the printLine

    // Create a string to hold the end time value
    //char strNUM[sizeof(int) + 1];
    // convert the int value of the total time to a string
    // snprintf(strNUM, sizeof(int) +6 , "%d", number);
    // create a temp string (tgt) the proper size of strTime and beginString
    //size_t len1 = strlen(beginString), len2 = strlen(strNUM);
    char *tgt = (char *)malloc(sizeof(char) * MAX_APPEND_STR_SIZE);
    if (tgt == NULL)
    {
        err_out(BAD_ALLOC,0);
        // perror("ERROR allocating memory.");
        // exit(-1);
    }

    // copy the first part to tgt
    // strcpy(tgt, beginString);
    // cat the time after the first part of the string
    // strcat(tgt, strNUM);
    // snprintf(tgt, (sizeof(int) +6) + strlen(beginString), "%s %d",beginString, number);
    snprintf(tgt, MAX_APPEND_STR_SIZE, "%s %d", beginString, number);
    // send to the printLine function
    printLine(tgt);
}


// From assingment2 solution
void err_out(char *msg, int printErrNo)
{
    char buffer[MAX_BUFFER_SIZE];
    strcpy(buffer,msg);
    if (printErrNo == 1)
    {
        strcat(buffer, strerror(errno));
    }
    strcat(buffer, "\n");
    write(STDERR_FILENO, buffer, strlen(buffer));
    exit(-1);
}