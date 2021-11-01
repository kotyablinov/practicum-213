#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Lines {
	long int* start;
	int checked;
	int length;
};

void main(int argc, char* argv[])
{
	FILE* currentFile;
	struct Lines* fileLines;

	char* currentMaxString, * maxSameString, * currentCheckString;
	char currentChar;
	int linesNumber, currentLength, maxLineIndex, currentLineIndex;
	int currentCheckLine, sameLineMaxCount, sameLineCurrentCount, currentCharIndex;
	bool condition;

	if (argc == 1) {
		printf("There is no input file.\n");
		return;
	} else if (argc > 2) {
		printf("Several files. Check first.\n");
	}
	currentFile = fopen(argv[1], "r");
	if (currentFile == NULL) {
		printf("Wrong path of the file.\n");
		return;
	}
	currentChar = fgetc(currentFile);

	//обработка пустого файла
	if (currentChar != EOF) {
		fileLines = (struct Lines*)malloc(sizeof(struct Lines));
		fileLines[0].checked = 0;
		fileLines[0].start = 0;
		currentLength = 0;
		linesNumber = 1;
	} else {
		fclose(currentFile);
		printf("File is empty.\n");
		return;
	}

	//проставляем указатели в файле на начала строк
	while (currentChar != EOF) {
		if (currentChar == '\n') {
			fileLines[linesNumber - 1].length = currentLength;
			currentLength = 0;
			linesNumber += 1;
			fileLines = (struct Lines*)realloc(fileLines, linesNumber * sizeof(struct Lines));
			fileLines[linesNumber - 1].checked = 0;
			fileLines[linesNumber - 1].start = ftell(currentFile);
		} else {
			currentLength += 1;
		}
		currentChar = fgetc(currentFile);
	}
	fileLines[linesNumber - 1].length = currentLength;

	fseek(currentFile, 0, SEEK_SET);
	maxLineIndex = 0;
	sameLineMaxCount = 1;
	// считываем первую строку
	maxSameString = (char*)malloc((fileLines[0].length + 1) * sizeof(char));
	for (currentCharIndex = 0; currentCharIndex < fileLines[0].length; currentCharIndex++) {
		maxSameString[currentCharIndex] = fgetc(currentFile);
	}
	maxSameString[fileLines[0].length] = '\0';
	//сравниваем строки если больше чем одна
	if (linesNumber != 1) {
		for (currentLineIndex = 0; currentLineIndex < linesNumber - 1; currentLineIndex++) {
			if (fileLines[currentLineIndex].checked != 0) {
				continue;
			}

			fseek(currentFile, fileLines[currentLineIndex].start, SEEK_SET);
			sameLineCurrentCount = 1;
			currentMaxString = (char*)malloc((fileLines[currentLineIndex].length + 1) * sizeof(char));
			for (currentCharIndex = 0; currentCharIndex < fileLines[currentLineIndex].length; currentCharIndex++) {
				currentMaxString[currentCharIndex] = fgetc(currentFile);
			}
			currentMaxString[fileLines[currentLineIndex].length] = '\0';
			fileLines[currentLineIndex].checked = 1;

			for (currentCheckLine = currentLineIndex + 1; currentCheckLine < linesNumber; currentCheckLine++) {
				//проверяем только в случае, если такую строку не проверяли и если их длины совпадают
				condition = (fileLines[currentCheckLine].checked == 0) && (fileLines[currentCheckLine].length == fileLines[currentLineIndex].length);
				if (condition) {
					fseek(currentFile, fileLines[currentCheckLine].start, SEEK_SET);
					currentCheckString = (char*)malloc((fileLines[currentCheckLine].length + 1) * sizeof(char));
					for (currentCharIndex = 0; currentCharIndex < fileLines[currentCheckLine].length; currentCharIndex++) {
						currentCheckString[currentCharIndex] = fgetc(currentFile);
					}
					currentCheckString[fileLines[currentCheckLine].length] = '\0';

					if (strcmp(currentCheckString, currentMaxString) == 0) {
						sameLineCurrentCount += 1;
						fileLines[currentCheckLine].checked = 1;
					}
					free(currentCheckString);
				}
			}

			if (sameLineCurrentCount > sameLineMaxCount) {
				maxLineIndex = currentLineIndex;
				sameLineMaxCount = sameLineCurrentCount;
				free(maxSameString);
				maxSameString = currentMaxString;
			} else {
				free(currentMaxString);
			}
		}
	}
	printf("%s\n", maxSameString);
	free(maxSameString);
	fclose(currentFile);
	free(fileLines);
}