#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void writeToFile(bool isAppend, char *filename) {
  if (filename) {
    int fd;
    if (isAppend) {
      fd = open(filename, O_CREAT | O_APPEND, 0666);
    } else {
      fd = open(filename, O_CREAT | O_WRONLY, 0666);
    }

    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
  }
}

void fileToStdIn(char *filename) {
  if (filename) {
    int fd = open(filename, O_RDONLY, 0666);
    dup2(fd, STDIN_FILENO);
  }
}

void executePathCommand(char ***pointerToStringList,
                        int *pointerToStringNumber) {
  int stringNumber = *pointerToStringNumber;

  char **stringList = *pointerToStringList;
  stringNumber++;
  stringList = (char **)realloc(stringList, stringNumber * sizeof(char *));
  stringList[stringNumber - 1] = NULL;

  int i = execvp(stringList[0], stringList);
  printf("%s\n", "Command failed with an error.");
}

void executeCommands(char ***pointerToStringList, int *pointerToStringNumber) {
  int stringNumber = *pointerToStringNumber;
  char **stringList = *pointerToStringList;

  int *inOutSymbols = NULL;
  int inOutSymbolsNumber = 0;

  for (int currentPosition = 0; currentPosition < stringNumber;
       currentPosition++) {
    char *currentWord = stringList[currentPosition];
    bool isInOutSymbol = (strcmp(currentWord, ">") == 0) ||
                         (strcmp(currentWord, ">>") == 0) ||
                         (strcmp(currentWord, "<") == 0);
    if (isInOutSymbol) {
      if (inOutSymbols == NULL) {
        inOutSymbols = (int *)malloc(sizeof(int));
      } else {
        inOutSymbols = (int *)realloc(inOutSymbols,
                                      (inOutSymbolsNumber + 1) * sizeof(int));
      }
      inOutSymbols[inOutSymbolsNumber] = currentPosition;
      inOutSymbolsNumber++;
    }
  }
  if (inOutSymbolsNumber == 0) {
    executePathCommand(pointerToStringList, pointerToStringNumber);
  } else if (inOutSymbolsNumber < 3) {
    char *firstSpecSymbol = stringList[inOutSymbols[0]];

    if (strcmp(firstSpecSymbol, "<") == 0) {
      bool isExistsFilenameIn = inOutSymbols[0] + 1 < stringNumber;

      if (isExistsFilenameIn) {
        fileToStdIn(stringList[inOutSymbols[0] + 1]);
      }

      bool isExistsSecondCommand = (inOutSymbolsNumber == 2) &&
                                   ((inOutSymbols[1] - inOutSymbols[0]) == 2);
      if (isExistsSecondCommand) {
        char *secondSpecSymbol = stringList[inOutSymbols[1]];
        if (strcmp(secondSpecSymbol, ">") == 0 ||
            (strcmp(secondSpecSymbol, ">>") == 0)) {
          bool isAppend = (strcmp(secondSpecSymbol, ">>") == 0);
          if (inOutSymbols[1] + 1 >= stringNumber) {
            printf("%s\n", "No filename for output.");
          } else {
            writeToFile(isAppend, stringList[inOutSymbols[1] + 1]);
          }
        }
      }
    } else if (strcmp(firstSpecSymbol, ">") == 0 ||
               (strcmp(firstSpecSymbol, ">>") == 0)) {
      bool isAppend = (strcmp(firstSpecSymbol, ">>") == 0);
      if (inOutSymbols[0] + 1 >= stringNumber) {
        printf("%s\n", "No filename for output.");
      } else {
        writeToFile(isAppend, stringList[inOutSymbols[0] + 1]);
      }
    }

    int commandLength = inOutSymbols[0];
    executePathCommand(pointerToStringList, &commandLength);
  } else {
    printf("%s\n",
           "Incorrect definition of  input/output redirection in the process");
  }

  if (inOutSymbols != NULL) {
    free(inOutSymbols);
  }
}

void executeChangeDirectory(char ***pointerToStringList,
                            int *pointerToStringNumber) {
  int numberArguments = (*pointerToStringNumber) - 1;
  bool isChangeToHome =
      (numberArguments == 0) || (strcmp((*pointerToStringList)[1], "~") == 0);
  if (isChangeToHome) {
    chdir(getenv("HOME"));
  } else if (numberArguments == 1) {
    int result = chdir((*pointerToStringList)[1]);
    if (result != 0) {
      printf("%s\n", "Wrong path.");
    }
  } else {
    printf("%s\n",
           "The number of arguments of the cd command is greater than 1.");
    return;
  }
}

void executeList(char ***pointerToStringList, int *pointerToStringNumber) {
  if (*pointerToStringNumber == 0) {
    printf("%s\n", "The command was not inputted.");
    return;
  }

  if (strcmp((*pointerToStringList)[0], "cd") == 0) {
    executeChangeDirectory(pointerToStringList, pointerToStringNumber);
  } else {
    executeCommands(pointerToStringList, pointerToStringNumber);
  }
}