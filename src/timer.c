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


        // constant for first prart of the printLine
        const char beginString[] = "\nTime(s): ";
        // Create a string to hold the end time value
        char strTime[sizeof(int) + 1];
        // convert the int value of the total time to a string
<<<<<<< HEAD
        itoa((int)end, strTime, DECIMAL);
=======
         snprintf(strTime, sizeof(int) +1 , "%d", end);  

>>>>>>> 80a543cdabd041b4dafbba53b7fc13f94c6a2cec
        //create a temp string (tgt) the proper size of strTime and beginString
        size_t len1 = strlen(beginString), len2 = strlen(strTime);
        char *tgt = (char *)malloc(len1 + len2 + 1);

        // copy the first part to tgt
        strcpy(tgt, beginString);
        // cat the time after the first part of the string
        strcat(tgt, strTime);
        // send to the printLine function
        printLine(tgt);

        
        return 0;
}
