#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>


#define MAX_BUFFER_SIZE 1024
#define FD_ARRAY_SIZE 2
#define PIPE_INPUT 1
#define PIPE_OUTPUT 0

#define BUFFSIZE 10


const char *readSTDIN(int *count);
void printLine(const char *line);
int *strToIntArray(char *string, int count);
char *itoa(int value, char *result, int base);
void addNumToString(const char beginString[], int number);


//--------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    int elementCount;
    const char *inputElements = readSTDIN(&elementCount);
    // printf("string array: %s\n", inputElements);
    // printf("number of elements: %d\n", elementCount);
    int fd[FD_ARRAY_SIZE], fd1[FD_ARRAY_SIZE];
    int n, m;
    char buf[MAX_BUFFER_SIZE];
    char buf2[MAX_BUFFER_SIZE];

    // set up the pipe

    pipe(fd);
    if (pipe(fd) < 0)
    {
        perror("An error occured when opening the pipe.");
        return 1;
    }
    pipe(fd1);
    if (pipe(fd) < 0)
    {
        perror("An error occured when opening second pipe.");
        return 2;
    }

    // write to pipe
    write(fd[PIPE_INPUT], inputElements, strlen(inputElements));
    close(fd[PIPE_INPUT]);

    write(fd1[PIPE_INPUT], inputElements, strlen(inputElements));
    close(fd1[PIPE_INPUT]);

    // here is the fork
    pid_t child1_pid, child2_pid;
    child1_pid = fork();
    if (child1_pid < 0)
    {
        // Error
        perror("fork failed");
        exit(-1);
    }
    // child process execution 1
    if (child1_pid == 0)
    {
        wait(NULL);
        // printf("hello world from child process child1_pid: %d \n", getpid());
        if ((n = read(fd[PIPE_OUTPUT], buf, MAX_BUFFER_SIZE)) >= 0)
        {
            buf[n] = '\0'; // terminate the string
            // printf("read %d bytes from the pipe: %s\n", n, buf);
            int *intElementsReturned1 = strToIntArray(buf, elementCount);
            int sum = 0;
            for (int i = 0; i < elementCount; i++)
            {
                sum = sum + intElementsReturned1[i];
            }
            addNumToString("Sum: ", sum);
           
            //printf("sum: %d\n", sum);
            close(fd[PIPE_OUTPUT]);
        }
    }
    // creates second fork off main
    if (child1_pid > 0)
    {
        child2_pid = fork();
        if (child2_pid < 0)
        {
            // Error
            perror("fork failed");
            exit(-1);
        }
        wait(NULL);
    }
    // child process execution 2
    if (child2_pid == 0 && child1_pid != 0)
    {
        wait(NULL);
        // printf("hello world from child process child2_pid: %d \n", getpid());
        if ((m = read(fd1[PIPE_OUTPUT], buf2, MAX_BUFFER_SIZE)) >= 0)
        {
            buf[m] = '\0'; // terminate the string
            // printf("read %d bytes from the pipe: %s \n", m, buf2);
            int *intElementsReturned2 = strToIntArray(buf2, elementCount);
            int product = 1;
            for (int i = 0; i < elementCount; i++)
            {
                product = product * intElementsReturned2[i];
            }
            addNumToString("Product: ", product);

            //printf("product: %d\n", product);
            close(fd1[PIPE_OUTPUT]);
        }
    }
    // parent process execution
    else if (child1_pid > 0 && child2_pid > 0)
    {
        wait(NULL);
        // printf("hello world from parent process child1_pid: %d \n", getpid());
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
			perror("read error");
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

	//printf("%d elements\n", elementCount);
	*count = elementCount;
	return (const char *)inputElements;
}

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

int *strToIntArray(char *string, int count)
{
	int *intElements = (int *)malloc((sizeof(int) * count));
	if (intElements == NULL)
	{
		perror("ERROR allocating memory.");
		exit(0);
	}
	char *token;
	const char delim[] = " ";
	token = strtok(string, delim);
	int i = 0;
	while (token != NULL)
	{
		//printf("String[%d]:%s\n", i, token);
		intElements[i] = atoi(token);
		//printf("int[%d]: %d\n", i, intElements[i]);
		token = strtok(NULL, delim);
		i++;
	}
	return (int*) intElements;
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



void addNumToString(const char beginString[], int number)
{
    // constant for first prart of the printLine
            
            // Create a string to hold the end time value
            char strNUM[sizeof(int) + 1];
            // convert the int value of the total time to a string
            itoa((int)number, strNUM, 10);
            // create a temp string (tgt) the proper size of strTime and beginString
            size_t len1 = strlen(beginString), len2 = strlen(strNUM);
            char *tgt = (char *)malloc(len1 + len2 + 1);
            if(tgt == NULL)
            {

		perror("ERROR allocating memory.");
		exit(0);
	}

            // copy the first part to tgt
            strcpy(tgt, beginString);
            // cat the time after the first part of the string
            strcat(tgt, strNUM);
            // send to the printLine function
            printLine(tgt);
}
