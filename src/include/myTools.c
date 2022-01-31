#include "myTools.h"


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