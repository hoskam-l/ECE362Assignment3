#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include<signal.h>
#include <sys/types.h>
#include <sys/wait.h>


// exec_prog from: https://stackoverflow.com/questions/5237482/how-do-i-execute-external-program-within-c-code-in-linux-with-arguments
#define MAX_TIME 3


 
int main(int argc, char *argv[])
{
        pid_t my_pid;
        int status, timeout;
        char **newargv = argv++;
        time_t t_begin, t_end;
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
                // Child process, execute code
                if (-1 == execvp(argv[0], newargv))
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
                while (0 == waitpid(my_pid , &status , WNOHANG)) {
                        if ( --timeout < 0 ) {
                                perror("timeout");
                                return -1;
                        }
                        sleep(1);
                }
                kill(my_pid, SIGKILL);
                t_end = time(NULL) - t_begin;
                printf("Time: %ld s\n", t_end);

                // Check status of child process
                /*if (1 != WIFEXITED(status) || 0 != WEXITSTATUS(status)) {
                        perror("child process failed");
                        return -1;
                }*/
        }
        
        // Success
        return 0;
}