// ECE362
// Assignment 3 part 1
// Dr. Schubert
// by luke hoskam
// by Hayden Galante
// Timer Program for Assignment 3

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define DECIMAL 10
#define MAX_APPEND_STR_SIZE 32
#define MAX_BUFFER_SIZE 1024
#define BAD_WRITE "write error\n"
#define BAD_FORK "fork error\n"
#define BAD_ALLOC "allocate error\n"
#define BAD_WAITPID "waitPID error\n"
#define BAD_EXEC "exec error\n"
#define NEW_LINE "\n"
// To clean up the code, a macro for bad write errors
#define Write(f, b, s)                 \
        if (write(f, b, s) != s)       \
        {                              \
                err_out(BAD_WRITE, 1); \
        }

void err_out(char *msg, int printErrNo);
void addNumToString(const char beginString[], int number);
void printLine(const char *line);
static int exec_prog(const char **argv);

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
// addNumToString appends a number to a string for printing with the printLine() function.
void addNumToString(const char beginString[], int number)
{

        // create a temp string (tgt) the proper size of strTime and beginString
        char *tgt = (char *)malloc(sizeof(char) * MAX_APPEND_STR_SIZE);
        if (tgt == NULL)
        {
                err_out(BAD_ALLOC, 0);
        }
        snprintf(tgt, MAX_APPEND_STR_SIZE, "%s %d", beginString, number);
        // send to the printLine function
        printLine(tgt);
}

// printLine function will print the line sent to STDOUT and add a '/n' to start a new line
void printLine(const char *line)
{
        int n = strlen(line);
        Write(STDOUT_FILENO, line, n);
        Write(STDOUT_FILENO, NEW_LINE, strlen(NEW_LINE));
}

// exec_prog from: https://stackoverflow.com/questions/5237482/how-do-i-execute-external-program-within-c-code-in-linux-with-arguments
// This hardly resembles the orginal program from SO.

static int exec_prog(const char **argv)
{
        pid_t childPID, wpid;
        int status;
        argv++;

        // Create fork
        childPID = fork();
        if (childPID < 0)
        {
                // Error
                err_out(BAD_FORK, 0);
        }
        else if (childPID == 0)
        {
                // Child process, execute code
                if (-1 == execvp(argv[0], (char **)argv))
                {
                        err_out(BAD_EXEC, 0);
                }
        }
        else
        {
                // Parent, wait for child
                // from https://pubs.opengroup.org/onlinepubs/9699919799/
                do
                {
                        wpid = waitpid(childPID, &status, WUNTRACED);
                        if (wpid == -1)
                        { // waitpid() returns -1 if there was an error
                                err_out(BAD_WAITPID, 0);
                        }

                        if (WIFEXITED(status))
                        {
                                //  Put code here if you want to do something if the child
                                //  exit successfully.
                        }
                        else if (WIFSIGNALED(status))
                        {
                                //  Put code here if you want to do something if the child
                                //  was killed.
                        }
                        else if (WIFSTOPPED(status))
                        {
                                //  Put code here if you want to do something if the child
                                //  stopped.
                        }
                        else
                        { /* Non-standard case -- may never happen */
                                printLine("Unexpected status");
                        }
                } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }

        // Success
        return 0;
}

int main(int argc, char *argv[])
{
        int begin, end;

        // Start timer
        begin = time(NULL);
        // execute program
        int rc = exec_prog((const char **)argv);
        // if RC is negative there was an error...
        if (rc < 0)
        {
                // should never happen
                err_out(BAD_EXEC, 0);
        }
        // end timer and calculate total time
        end = time(NULL) - begin;

        addNumToString("\nTime(s): ", end);

        return 0;
}
