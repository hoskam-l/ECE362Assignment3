#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "include/myTools.h"




//--------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	int elementCount;
	const char *inputElements = readSTDIN(&elementCount);
	//printf("string array: %s\n", inputElements);
	//printf("number of elements: %d\n", elementCount);
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
	int child1_pid = fork();
	int child2_pid;

	// child process execution 1
	if (child1_pid == 0)
	{
		wait(NULL);
		//printf("hello world from child process child1_pid: %d \n", getpid());
		if ((n = read(fd[0], buf, 1024)) >= 0)
		{
			buf[n] = '\0'; // terminate the string
			//printf("read %d bytes from the pipe: %s\n", n, buf);
			int *intElementsReturned1 = strToIntArray(buf, elementCount);
			int sum = 0;
			for (int i = 0; i < elementCount; i++)
			{
				sum = sum + intElementsReturned1[i];
			}
			printf("sum: %d\n", sum);
			close(fd[0]);
		}
	}
	// creates second fork off main
	if (child1_pid != 0)
	{
		child2_pid = fork();
		wait(NULL);
	}
	// child process execution 2
	if (child2_pid == 0 && child1_pid != 0)
	{
		wait(NULL);
		//printf("hello world from child process child2_pid: %d \n", getpid());
		if ((m = read(fd1[0], buf2, 1024)) >= 0)
		{
			buf[m] = '\0'; // terminate the string
			//printf("read %d bytes from the pipe: %s \n", m, buf2);
			int *intElementsReturned2 = strToIntArray(buf2, elementCount);
			int product = 1;
			for (int i = 0; i < elementCount; i++)
			{
				product = product * intElementsReturned2[i];
			}
			printf("product: %d\n", product);
			close(fd1[0]);
		}
	}
	// parent process execution
	else if (child1_pid != 0 && child2_pid != 0)
	{
		wait(NULL);
		//printf("hello world from parent process child1_pid: %d \n", getpid());
	}

	return 0;
}
