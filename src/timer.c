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

#define DECIMAL 10
#define MAX_APPEND_STR_SIZE 32

void addNumToString(const char beginString[], int number)
{

    // create a temp string (tgt) the proper size of strTime and beginString
    char *tgt = (char *)malloc(sizeof(char) * MAX_APPEND_STR_SIZE);
    if (tgt == NULL)
    {

        perror("ERROR allocating memory.");
        exit(-1);
    }
    snprintf(tgt, MAX_APPEND_STR_SIZE, "%s %d", beginString, number);
    // send to the printLine function
    printLine(tgt);
}

// printLine function will print the line sent to STDOUT and add a '/n' to start a new line
void printLine(const char *line)
{
        int n = strlen(line);

        if (write(STDOUT_FILENO, line, n) != n)
        {
                perror("write error");
                exit(-1);
        }
        // Since printLine we send a \n
        if (write(STDOUT_FILENO, "\n", 2) != 2)
        {
                perror("write error");
                exit(-1);
        }
}

// exec_prog from: https://stackoverflow.com/questions/5237482/how-do-i-execute-external-program-within-c-code-in-linux-with-arguments
// This hardly resembles the orginal program from SO.

static int exec_prog(const char **argv)
{
        pid_t child_pid, wpid;
        int status;
        argv++;

        // Create fork
        child_pid = fork();
        if (child_pid < 0)
        {
                // Error
                perror("fork failed");
                return -1;
        }
        else if (child_pid == 0)
        {
                // Child process, execute code
                if (-1 == execvp(argv[0], (char **)argv))
                {
                        perror("child process execve failed");
                        return -1;
                }
        }
        else
        {
                // Parent, wait for child
                //from https://pubs.opengroup.org/onlinepubs/9699919799/
                do
                {
                        wpid = waitpid(child_pid, &status, WUNTRACED);
                        if (wpid == -1)
                        {
                                perror("waitpid");
                                return -1;;
                        }

                        if (WIFEXITED(status))
                        {
                                printLine("child exited");
                        }
                        else if (WIFSIGNALED(status))
                        {
                                printLine("child killed");
                        }
                        else if (WIFSTOPPED(status))
                        {
                                printLine("child stopped");
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
        if(rc<0)
        {
                perror("Error executing program");
                exit(-1);
        }
        // end timer and calculate total time
        end = time(NULL) - begin;

        addNumToString("\nTime(s): ", end);
        
        return 0;
}
