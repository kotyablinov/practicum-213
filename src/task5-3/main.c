#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "inputLines.h"
#include "listSpliter.h"
#include "pipingExecutor.h"

void printList(char **stringList, int stringNumber) {
  //выводим получившийся список
  for (int currStringNumber = 0; currStringNumber < stringNumber;
       currStringNumber++) {
    printf("%s\n", stringList[currStringNumber]);
  }
}

int main() {
  bool isEOF = true;
  char **stringList = NULL;
  int stringNumber;

  while (isEOF) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      printf("MYSHELL > %s$ ", cwd);
    }
    isEOF = readLines(&stringList, &stringNumber);

    char ***processList = NULL;
    int processListNumber = 0;
    int *processLineLength = NULL;

    trySplitList(&stringList, &stringNumber, &processList, &processListNumber,
                 &processLineLength, "|");

    executePiping(&processList, &processListNumber, &processLineLength);

    for (int line = 0; line < processListNumber - 1; line++) {
      free(processList[line]);
    }

    for (int line = 0; line < stringNumber - 1; line++) {
      free(stringList[line]);
    }
    free(stringList);
    stringList = NULL;
    stringNumber = 0;
  }
  return 0;
}