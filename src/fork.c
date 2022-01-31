#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define BUFFSIZE 10

const char *readSTDIN(int *count)
{
	int n;
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
		for (int i = 0; i < n; i++)
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

	printf("%d elements\n", elementCount);
	*count = elementCount;
	return (const char *)inputElements;
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
		printf("String[%d]:%s\n", i, token);
		intElements[i] = atoi(token);
		printf("int[%d]: %d\n", i, intElements[i]);
		token = strtok(NULL, delim);
		i++;
	}
}

//--------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	int elementCount;
	const char *inputElements = readSTDIN(&elementCount);
	printf("string array: %s\n", inputElements);
	printf("number of elements: %d\n", elementCount);
	int fd[2], fd1[2];
	int n, m;
	char buf[1025];
	char buf2[1025];

	// set up the pipe

	pipe(fd);
	if (pipe(fd) == -1)
	{
		perror("An error occured when opening the pipe.");
		return 1;
	}
	pipe(fd1);
	if (pipe(fd) == -1)
	{
		perror("An error occured when opening second pipe.");
		return 2;
	}

	// write to pipe
	write(fd[1], inputElements, strlen(inputElements));
	close(fd[1]);

	write(fd1[1], inputElements, strlen(inputElements));
	close(fd1[1]);

	// here is the fork
	int id = fork();
	int id2;

	// child process execution 1
	if (id == 0)
	{
		wait(NULL);
		printf("hello world from child process id: %d \n", getpid());
		if ((n = read(fd[0], buf, 1024)) >= 0)
		{
			buf[n] = '\0'; // terminate the string
			printf("read %d bytes from the pipe: %s\n", n, buf);
			int * intElementsReturned = strToIntArray(buf,elementCount);
			close(fd[0]);
		}
	}
	// creates second fork off main
	if (id != 0)
	{
		id2 = fork();
		wait(NULL);
	}
	// child process execution 2
	if (id2 == 0 && id != 0)
	{
		wait(NULL);
		printf("hello world from child process id2: %d \n", getpid());
		if ((m = read(fd1[0], buf2, 1024)) >= 0)
		{
			buf[m] = '\0'; // terminate the string
			printf("read %d bytes from the pipe: %s \n", m, buf2);
			close(fd1[0]);
		}
	}
	// parent process execution
	else if (id != 0 && id2 != 0)
	{
		wait(NULL);
		printf("hello world from parent process id: %d \n", getpid());
	}

	return 0;
}
