#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Lines
{
	long int* start;
	int length;
};

void main(int argc, char* argv[])
{
	FILE* currentFile, * outputFile;
	struct Lines* fileLines;

	char currentChar;
	int linesNumber, currentLength, maxLength, temporalLength, firstIndexSort, secondIndexSort, currentString, currentCharIndex;
	long int* temporalStart;

	if (argc < 3)
	{
		printf("Incorrect arguments.\n");
		return;
	}

	currentFile = fopen(argv[1], "r");
	if (currentFile == NULL)
	{
		printf("Wrong path of the file.\n");
		return;
	}
	currentChar = fgetc(currentFile);

	//обработка пустого файла
	if (currentChar != EOF)
	{
		fileLines = (struct Lines*)malloc(sizeof(struct Lines));
		fileLines[0].start = 0;
		currentLength = 0;
		linesNumber = 1;
	}
	else
	{
		fclose(currentFile);
		printf("File is empty.\n");
		return;
	}
	outputFile = fopen("output.txt", "w");
	maxLength = strtol(argv[2], NULL, 10);

	//проставляем указатели в файле на начала строк
	while (currentChar != EOF)
	{
		if (currentChar == '\n')
		{
			fileLines[linesNumber - 1].length = currentLength;
			currentLength = 0;
			linesNumber += 1;
			fileLines = (struct Lines*)realloc(fileLines, linesNumber * sizeof(struct Lines));
			fileLines[linesNumber - 1].start = ftell(currentFile);
		}
		else
		{
			currentLength += 1;
		}

		currentChar = fgetc(currentFile);
	}
	fileLines[linesNumber - 1].length = currentLength;

	//сортируем по длине
	for (firstIndexSort = 0; firstIndexSort < linesNumber - 1; firstIndexSort++)
	{
		for (secondIndexSort = 0; secondIndexSort < linesNumber - firstIndexSort - 1; secondIndexSort++)
		{
			if (fileLines[secondIndexSort].length > fileLines[secondIndexSort + 1].length)
			{
				temporalLength = fileLines[secondIndexSort].length;
				temporalStart = fileLines[secondIndexSort].start;
				fileLines[secondIndexSort].length = fileLines[secondIndexSort + 1].length;
				fileLines[secondIndexSort].start = fileLines[secondIndexSort + 1].start;
				fileLines[secondIndexSort + 1].length = temporalLength;
				fileLines[secondIndexSort + 1].start = temporalStart;
			}
		}
	}

	for (currentString = 0; currentString < linesNumber; currentString++)
	{
		fseek(currentFile, fileLines[currentString].start, SEEK_SET);
		for (currentCharIndex = 0; currentCharIndex < fileLines[currentString].length; currentCharIndex++)
		{
			putc(fgetc(currentFile), outputFile);
		}
		putc('\n', outputFile);
	}

	if (fileLines[linesNumber].length > maxLength)
	{
		printf("The file contains lines longer than the specified length.\n");
	}

	fclose(currentFile);
	fclose(outputFile);
	free(fileLines);
}