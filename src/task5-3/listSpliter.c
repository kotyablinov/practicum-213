#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool trySplitList(char ***pointerToStringList, int *pointerToStringListNumber,
                  char ****pointerToProcessList,
                  int *pointerToProcessListNumber,
                  int **pointerToProcessLineLength, char *controlWord) {
  char **stringList = *pointerToStringList;
  int stringListNumber = *pointerToStringListNumber;
  int currentProcessLength = 0;
  int processListNumber = 0;

  char **currentProcess = NULL;
  char ***processList = NULL;
  int *processLineLength = NULL;

  bool hasControlWord = false;

  for (int currentString = 0; currentString < stringListNumber;
       currentString++) {
    char *currentWord = stringList[currentString];

    if (strcmp(currentWord, controlWord) != 0) {
      if (currentProcess == NULL) {
        currentProcess = (char **)malloc(sizeof(char *));
      } else {
        currentProcess = (char **)realloc(
            currentProcess, (currentProcessLength + 1) * sizeof(char *));
      }
      currentProcess[currentProcessLength] = currentWord;
      currentProcessLength++;
    }

    if ((currentString == stringListNumber - 1) ||
        (strcmp(currentWord, controlWord) == 0)) {
      if (strcmp(currentWord, controlWord) == 0) {
        hasControlWord = true;
      }
      if (currentProcessLength == 0) {
        // TODO ошибка
      }
      if (processList == NULL) {
        processList = (char ***)malloc(sizeof(char **));
        processLineLength = (int *)malloc(sizeof(int));
      } else {
        processList = (char ***)realloc(
            processList, (processListNumber + 1) * sizeof(char **));
        processLineLength = (int *)realloc(
            processLineLength, (processListNumber + 1) * sizeof(int));
      }
      processList[processListNumber] = currentProcess;
      processLineLength[processListNumber] = currentProcessLength;

      processListNumber++;

      currentProcess = NULL;
      currentProcessLength = 0;
    }
  }
  *pointerToProcessList = processList;
  *pointerToProcessListNumber = processListNumber;
  *pointerToProcessLineLength = processLineLength;

  return hasControlWord;
}