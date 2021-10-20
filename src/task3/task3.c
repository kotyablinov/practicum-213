#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

struct list {
	char* s;
	struct list* next;
};


void sortList(struct list** firstListPointerToPointer, struct list* inputListPointer)
{
	struct list* firstListPointer = *firstListPointerToPointer;
	struct list* previusListPointer, * currentListPointer;
	currentListPointer = firstListPointer;
	bool isPut;
	// если надо вставить в начале
	if (strcmp(inputListPointer->s, firstListPointer->s) < 0)
	{
		inputListPointer->next = firstListPointer;
		firstListPointer = inputListPointer;
		*firstListPointerToPointer = inputListPointer;
		return;
	}
	// s1 s2 и вставляем s между ними если s1<=s и s2 >= s
	while (currentListPointer->next != NULL)
	{
		isPut = (strcmp(inputListPointer->s, currentListPointer->s) >= 0) && (strcmp(inputListPointer->s, (currentListPointer->next)->s) <= 0);
		if (isPut)
		{
			inputListPointer->next = currentListPointer->next;
			currentListPointer->next = inputListPointer;
			return;
		}
		currentListPointer = currentListPointer->next;
	}
	//вставляем в конце
	inputListPointer->next = NULL;
	currentListPointer->next = inputListPointer;
	return;
}

void printList(struct list* firstListPointer)
{
	struct list* currentListPointer;
	currentListPointer = firstListPointer;
	while (currentListPointer != NULL)
	{
		printf("%s\n", currentListPointer->s);
		currentListPointer = currentListPointer->next;
	}
	return;
}

void cleanList(struct list** firstListPointer)
{
	struct list* currentListPointer;
	while (*firstListPointer != NULL)
	{
		currentListPointer = *firstListPointer;
		*firstListPointer = (*firstListPointer)->next;
		free(currentListPointer->s);
		free(currentListPointer);
	}
	return;
}


int main()
{
	char againFlag = 'y';
	int stringNumber, maxStringSize, currentStringSize;
	char* currentString = NULL;
	struct list* inputListPointer, * firstListPointer;
	char inputSymbol;



	while (againFlag == 'y')
	{
		printf("Input sequence:\n");
		stringNumber = 0;
		inputSymbol = getchar();
		while (inputSymbol != EOF)
		{
			stringNumber += 1;

			//сюда ввод строки
			maxStringSize = 1;
			currentStringSize = 0;
			currentString = (char*)malloc(maxStringSize * sizeof(char));
			while ((inputSymbol != '\n') && (inputSymbol != EOF))
			{
				if (currentStringSize > (maxStringSize - 1))
				{
					maxStringSize += 1;
					currentString = (char*)realloc(currentString, maxStringSize * sizeof(char));
					if (!currentString)
					{
						printf("Allocation error.\n");
						exit(1);
					}
				}
				currentString[currentStringSize] = inputSymbol;
				currentStringSize += 1;
				inputSymbol = getchar();
			}
			//нуль символ в конец строки
			if (currentStringSize > (maxStringSize - 1))
			{
				currentString = (char*)realloc(currentString, (maxStringSize + 1) * sizeof(char));
				if (!currentString)
				{
					printf("Allocation error.\n");
					exit(1);
				}
			}
			currentString[currentStringSize] = '\0';
			if (inputSymbol != EOF)
			{
				inputSymbol = getchar();
			}

			if (stringNumber == 1)
			{
				firstListPointer = (struct list*)malloc(sizeof(struct list));
				if (!firstListPointer)
				{
					printf("Allocation error.\n");
					exit(1);
				}
				firstListPointer->s = currentString;
				firstListPointer->next = NULL;
			}
			else
			{
				inputListPointer = (struct list*)malloc(sizeof(struct list));
				if (!inputListPointer)
				{
					printf("Allocation error.\n");
					exit(1);
				}
				inputListPointer->s = currentString;
				sortList(&firstListPointer, inputListPointer);
			}
		}

		printList(firstListPointer);
		cleanList(&firstListPointer);


		printf("Do you want again? y/n\n");
		againFlag = getchar();
		inputSymbol = getchar();
		printf("\n");
	}
	return 0;
}