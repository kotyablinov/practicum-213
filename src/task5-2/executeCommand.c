#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void executePathCommand(char ***pointerToStringList,
                        int *pointerToStringNumber) {
  int stringNumber = *pointerToStringNumber;

  char **stringList = *pointerToStringList;
  stringNumber++;
  stringList = (char **)realloc(stringList, stringNumber * sizeof(char *));
  stringList[stringNumber - 1] = NULL;

  pid_t childPid = fork();
  if (childPid == 0) {
    int i = execvp(stringList[0], stringList);
    printf("%s\n", "Command failed with an error.");
  } else {
    wait(0);
    kill(childPid, SIGKILL);
  }

  *pointerToStringList = stringList;
  *pointerToStringNumber = stringNumber;
}

void executeChangeDirectory(char ***pointerToStringList,
                            int *pointerToStringNumber) {
  int numberArguments = (*pointerToStringNumber) - 2;
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
    executePathCommand(pointerToStringList, pointerToStringNumber);
  }
}