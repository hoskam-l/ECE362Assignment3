#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>


// exec_prog from: https://stackoverflow.com/questions/5237482/how-do-i-execute-external-program-within-c-code-in-linux-with-arguments
#define MAX_TIME 3

static int exec_prog(const char **argv)
{
        pid_t my_pid;
        int status, timeout;
        const char **newargv = argv;
        
        // New argv ignores first argument
        newargv++;
        
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
                if (-1 == execvp(newargv[0], (char **)newargv))
                {
                        perror("child process execve failed");
                        return -1;
                }
        }
        else
        {
                // Parent, wait for child
                timeout = MAX_TIME;
                while (0 == waitpid(my_pid , &status , WNOHANG)) {
                        if ( --timeout < 0 ) {
                                perror("timeout");
                                return -1;
                        }
                        sleep(1);
                }

                // Check status of child process
                /*if (1 != WIFEXITED(status) || 0 != WEXITSTATUS(status)) {
                        perror("child process failed");
                        return -1;
                }*/
        }
        
        // Success
        return 0;
}

 
int main(int argc, char *argv[])
{
        time_t begin = time(NULL);
        int rc = exec_prog((const char **)argv);
        time_t end = time(NULL) - begin;

        printf("Time: %ld s\n", end);
        return rc;
}