#define _POSIX_C_SOURCE 200809L
#define _OPEN_SYS_ITOA_EXT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sysexits.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// exec_prog from: https://stackoverflow.com/questions/5237482/how-do-i-execute-external-program-within-c-code-in-linux-with-arguments
#define MAX_TIME 3

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

int main(int argc, char *argv[])
{
        pid_t my_pid;
        int status, timeout;
        // strcpy(argv[0], argv[1]);
        char **newargv = argv;
        newargv++;
        int t_begin, t_end;

        // Create fork
        my_pid = fork();
        if (my_pid < 0)
        {
                // Error
                perror("fork failed");
                return -1;
        }
        else if (my_pid == 0)
        { // Child process, execute code
                if (-1 == execvp(newargv[0], newargv))
                {
                        perror("child process execve failed");
                        return -1;
                }
        }
        else
        {
                // Parent, wait for child
                t_begin = time(NULL);
                timeout = MAX_TIME;
                while (0 == waitpid(my_pid, &status, WNOHANG))
                {
                        if (--timeout < 0)
                        {
                                perror("timeout");
                                return -1;
                        }
                        sleep(1);
                }
                t_end = time(NULL) - t_begin;

                const char beginString[] = "\nTime: ";
                char strTime[sizeof(int) + 1];
                itoa((int)t_end, strTime, 10);
                size_t len1 = strlen(beginString), len2 = strlen(strTime);
                char *tgt = (char *)malloc(len1 + len2 + 1);

                strcpy(tgt, beginString);
                strcat(tgt, strTime);

                printLine(tgt);
        }

        // Success
        return 0;
}