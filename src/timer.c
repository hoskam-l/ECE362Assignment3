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


// printLine function will print the line sent to STDOUT and add a '/n' to start a new line
void printLine(const char *line)
{
        int n = strlen(line);

        if (write(STDOUT_FILENO, line, n) != n)
                perror("write error");
        // Since printLine we send a \n
        if (write(STDOUT_FILENO, "\n", 2) != 2)
                perror("write error");
}
// from: https://stackoverflow.com/questions/8257714/how-to-convert-an-int-to-string-in-c
/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
char *itoa(int value, char *result, int base)
{
        // check that the base if valid
        if (base < 2 || base > 36)
        {
                *result = '\0';
                return result;
        }

        char *ptr = result, *ptr1 = result, tmp_char;
        int tmp_value;

        do
        {
                tmp_value = value;
                value /= base;
                *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
        } while (value);

        // Apply negative sign
        if (tmp_value < 0)
                *ptr++ = '-';
        *ptr-- = '\0';
        while (ptr1 < ptr)
        {
                tmp_char = *ptr;
                *ptr-- = *ptr1;
                *ptr1++ = tmp_char;
        }
        return result;
}

// exec_prog from: https://stackoverflow.com/questions/5237482/how-do-i-execute-external-program-within-c-code-in-linux-with-arguments
// This hardly resembles the orginal program.

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
        // end timer and calculate total time
        end = time(NULL) - begin;


        // constant for first prart of the printLine
        const char beginString[] = "\nTime(s): ";
        // Create a string to hold the end time value
        char strTime[sizeof(int) + 1];
        // convert the int value of the total time to a string
        itoa((int)end, strTime, 10);
        //create a temp string (tgt) the proper size of strTime and beginString
        size_t len1 = strlen(beginString), len2 = strlen(strTime);
        char *tgt = (char *)malloc(len1 + len2 + 1);

        // copy the first part to tgt
        strcpy(tgt, beginString);
        // cat the time after the first part of the string
        strcat(tgt, strTime);
        // send to the printLine function
        printLine(tgt);

        // if RC is negative there was an error...
        return rc;
}
