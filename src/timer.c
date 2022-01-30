#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

void printLine(const char *line)
{
        int n = strlen(line);

        if (write(STDOUT_FILENO, line, n) != n)
                perror("write error");

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
                                exit(EXIT_FAILURE);
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

        // Start 
        begin = time(NULL);
        int rc = exec_prog((const char **)argv);
        end = time(NULL) - begin;

        const char beginString[] = "\nTime(s): ";
        char strTime[sizeof(int) + 1];
        itoa((int)end, strTime, 10);
        size_t len1 = strlen(beginString), len2 = strlen(strTime);
        char *tgt = (char *)malloc(len1 + len2 + 1);

        strcpy(tgt, beginString);
        strcat(tgt, strTime);

        printLine(tgt);

        return rc;
}
