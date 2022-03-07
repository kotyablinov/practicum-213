
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define CTRL_Z ('Z' & 0x1f)

int inputLines(char*** lines, int* countLines)
{
	char* tempLine;
	char** linesPointer;
	int maxArraySize, maxLineSize, currentLineSize, currentArraySize;
	char inputSymbol;

	currentArraySize = 0; //сколько указателей на текущий момент
	maxArraySize = 1; //выделено памяти под указатели
	linesPointer = (char**)malloc(maxArraySize * sizeof(char*));

	bool isEOF = false;

	while (!feof(stdin) && !isEOF)
	{
		maxLineSize = 1;
		currentLineSize = 0;

		tempLine = (char*)malloc(maxLineSize * sizeof(char));

		while (!feof(stdin))
		{
			inputSymbol = getchar();
			bool isCaseEOL;
			isCaseEOL =  (inputSymbol == EOF)|| (inputSymbol == CTRL_Z) || feof(stdin);
			if (isCaseEOL)
			{
				isEOF = true;
				break;
			}
			if (inputSymbol == '\n')
			{
				break;
			}
			if (currentLineSize > (maxLineSize - 1))
			{
				maxLineSize += 1;
				tempLine = (char*)realloc(tempLine, maxLineSize * sizeof(char));
			}
			tempLine[currentLineSize] = inputSymbol;
			currentLineSize++;
		}

		if (currentLineSize > (maxLineSize - 1))
		{
			tempLine = (char*)realloc(tempLine, (maxLineSize + 1) * sizeof(char));
		}
		tempLine[currentLineSize] = '\0';

		if (currentArraySize > (maxArraySize - 1)) //занесение новой строки
		{
			maxArraySize += 1;
			linesPointer = (char**)realloc(linesPointer, maxArraySize * sizeof(char*));
		}
		linesPointer[currentArraySize] = tempLine;
		currentArraySize++;
	}

	*lines = linesPointer;
	*countLines = currentArraySize;

	return 0;
}

//True при (a[j] > a[j+1])
bool stringCompare(char* first, char* second)
{
	int minLength = 0;
	int i = 0;
	if (strlen(first) < strlen(second))
	{
		minLength = strlen(first);
	}
	else
	{
		minLength = strlen(second);
	}

	if (strlen(first) == 0)
	{
		return false;
	}
	else if (strlen(second) == 0)
	{
		return true;
	}

	while ((first[i] == second[i]) && (i < minLength))
	{
		i++;
	}
	bool isCaseLarger = (i == minLength) && (strlen(first) != strlen(second));
	if (isCaseLarger) //одна строка - подстрока второй
	{
		if (strlen(first) > strlen(second))
		{
			return true;
		}
	}
	else
	{
		if (first[i] > second[i])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

int swap(char** left, char** right)
{
	char* tempPointer;
	tempPointer = *left;
	*left = *right;
	*right = tempPointer;
	return 0;
}

int sortString(char*** lines, int n)
{
	char** stringArray;
	int i, j;
	stringArray = *lines;
	for (i = 0; i < n - 1; i++)
	{
		for (j = 0; j < n - i - 1; j++)
		{
			if (stringCompare(stringArray[j], stringArray[j + 1]))
			{
				swap(&stringArray[j], &stringArray[j + 1]);
			}
		}
	}

	return 0;
}

int printLines(char** rows, int rowsNumber)
{
	int i;
	for (i = 0; i < rowsNumber; i++)
	{
		//printf("Array[%d]: %s\n",i, rows[i]);
		printf("%s\n", rows[i]);
	}
	return 0;
}

int main()
{
	int linesNumber = 0;
	char** lines;
	inputLines(&lines, &linesNumber);
	sortString(&lines, linesNumber);
	printLines(lines, linesNumber);

	int i;
	for (i = 0; i < linesNumber; i++)
	{
		free(lines[i]);
	}
	free(lines);

	return 0;
}
