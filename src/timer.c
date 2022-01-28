#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// exec_prog from: https://stackoverflow.com/questions/5237482/how-do-i-execute-external-program-within-c-code-in-linux-with-arguments
#define MAX_TIME 3

int main(int argc, char *argv[])
{
        int childPID, parentPID;
        pid_t my_pid;
        int status, timeout;
        // strcpy(argv[0], argv[1]);
        char **newargv = argv;
        newargv++;
        time_t t_begin, t_end;

        parentPID = (int) getpid();
        // Create fork
        my_pid = fork();
        if (my_pid < 0)
        {
                // Error
                perror("fork failed");
                return -1;
        }
        else if (my_pid == 0)
        {
                childPID = (int) getpid();
                // Child process, execute code
                // while (execvp(newargv[0], (char **)newargv))
                // {
                // }

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
                printf("\nTime: %ld s\n", t_end);
                printf("The parent PID is %d\n",parentPID);
                printf("The child PID is %d\n",childPID);
        }

        // Success
        return 0;
}