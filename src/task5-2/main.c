#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "executeCommand.h"
#include "inputLines.h"

void printList(char **stringList, int stringNumber) {
  //выводим получившийся список
  for (int currStringNumber = 0; currStringNumber < stringNumber - 1;
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
    executeList(&stringList, &stringNumber);

    // printList(stringList, stringNumber);

    for (int line = 0; line < stringNumber - 1; line++) {
      free(stringList[line]);
    }
    free(stringList);
    stringList = NULL;
    stringNumber = 0;
  }
  return 0;
}