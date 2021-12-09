#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "myShellLib/inputLines.h"
#include "myShellLib/smartScriptSolver.h"
#include "myShellLib/subScript.h"

void showError(char *errorMessage) {
  printf("%s\n", errorMessage);
}

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
  int stringListNumber;

  while (isEOF) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      printf("MYSHELL > %s$ ", cwd);
      fflush(stdout);
    }
    isEOF = readLines(&stringList, &stringListNumber);

    SUBSCRIPT mainScript;
    mainScript.startPosition = 0;
    mainScript.endPosition = stringListNumber - 1;

    char *errorMessage = NULL;
    if (stringListNumber > 0) {
      int code = executeScript(&stringList, mainScript,
                               &errorMessage);
      if (code != EXIT_SUCCESS) {
        showError(errorMessage);
      }
    }

    for (int line = 0; line < stringListNumber - 1; line++) {
      free(stringList[line]);
    }

    free(stringList);
    stringList = NULL;
    stringListNumber = 0;
  }
  return 0;
}