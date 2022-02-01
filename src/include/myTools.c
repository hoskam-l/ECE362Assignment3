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