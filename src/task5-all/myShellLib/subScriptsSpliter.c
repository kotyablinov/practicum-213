#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "subScript.h"

bool isLeftBrackets(char *currentWord) { return strcmp(currentWord, "(") == 0; }

bool isRightBrackets(char *currentWord) {
  return strcmp(currentWord, ")") == 0;
}

bool isFlowSplitter(char *currentWord) {
  return (strcmp(currentWord, "&&") == 0) || (strcmp(currentWord, "||") == 0) ||
         (strcmp(currentWord, ";") == 0) || (strcmp(currentWord, ">") == 0) ||
         (strcmp(currentWord, ">>") == 0) || (strcmp(currentWord, "<") == 0) ||
         (strcmp(currentWord, "|") == 0);
}

int getSubScripts(char ***pointerToStringList, SUBSCRIPT currentScript,
                  SUBSCRIPT **pointerToSubScripts,
                  int *pointerToSubScriptsNumber,
                  char ***pointerToStackCommands,
                  int *pointerToStackCommandsNumber,
                  char **pointerToErrorMessage) {
  int startOfCurrentScript = currentScript.startPosition;
  int endOfCurrentScript = currentScript.endPosition;

  // Скрипты
  SUBSCRIPT *subScripts = NULL;
  int subScriptsNumber = 0;
  // Стек команд
  char **stackCommands = NULL;
  int stackCommandsNumber = 0;
  // Лист слов
  char **stringList = *pointerToStringList;

  for (int currentString = startOfCurrentScript;
       currentString <= endOfCurrentScript; currentString++) {
    char *currentWord = stringList[currentString];

    if (isRightBrackets(currentWord)) {
      *pointerToErrorMessage = "Strange number of brackets.";
      return EXIT_FAILURE;
    } else if (isLeftBrackets(currentWord)) {
      subScriptsNumber++;
      if (subScripts == NULL) {
        subScripts = (SUBSCRIPT *)malloc(sizeof(SUBSCRIPT));
      } else {
        subScripts = (SUBSCRIPT *)realloc(subScripts,
                                          subScriptsNumber * sizeof(SUBSCRIPT));
      }
      if (currentString + 1 >= endOfCurrentScript) {
        *pointerToErrorMessage = "Strange number of brackets.";
        return EXIT_FAILURE;
      }
      currentString++;
      subScripts[subScriptsNumber - 1].startPosition = currentString;
      subScripts[subScriptsNumber - 1].endPosition = -1;

      int leftBrackets = 1;
      for (; currentString <= endOfCurrentScript; currentString++) {
        currentWord = stringList[currentString];
        if (isLeftBrackets(currentWord)) {
          leftBrackets++;
        } else if (isRightBrackets(currentWord)) {
          leftBrackets--;
        }
        if (currentString == endOfCurrentScript && leftBrackets != 0) {
          *pointerToErrorMessage = "Strange number of brackets.";
          return EXIT_FAILURE;
        }

        if (leftBrackets == 0) {
          subScripts[subScriptsNumber - 1].endPosition = currentString - 1;
          break;
        }
      }
    } else if (isFlowSplitter(currentWord)) {
      stackCommandsNumber++;
      if (stackCommands == NULL) {
        if (stackCommands == NULL) {
          stackCommands = (char **)malloc(sizeof(char *));
        } else {
          stackCommands = (char **)realloc(
              stackCommands, stackCommandsNumber * sizeof(char *));
        }
      }

      if (subScripts != NULL) {
        if (subScripts[subScriptsNumber - 1].endPosition == -1) {
          subScripts[subScriptsNumber - 1].endPosition = currentString - 1;
        }
      }
      stackCommands[stackCommandsNumber - 1] = currentWord;

      if (currentString + 1 <= endOfCurrentScript &&
          isFlowSplitter(stringList[currentString + 1])) {
        *pointerToErrorMessage = "Unusual command flow control.";
        return EXIT_FAILURE;
      }
    } else {
      subScriptsNumber++;
      if (subScripts == NULL) {
        subScripts = (SUBSCRIPT *)malloc(sizeof(SUBSCRIPT));
      } else {
        subScripts = (SUBSCRIPT *)realloc(subScripts,
                                          subScriptsNumber * sizeof(SUBSCRIPT));
      }
      subScripts[subScriptsNumber - 1].startPosition = currentString;
      subScripts[subScriptsNumber - 1].endPosition = -1;
      currentString++;

      for (; currentString <= endOfCurrentScript; currentString++) {
        currentWord = stringList[currentString];
        if (isFlowSplitter(currentWord) || isLeftBrackets(currentWord) ||
            isRightBrackets(currentWord)) {
          subScripts[subScriptsNumber - 1].endPosition = currentString - 1;
          currentString--;
          break;
        }
      }
    }
  }

  if (subScripts[subScriptsNumber - 1].endPosition == -1) {
    subScripts[subScriptsNumber - 1].endPosition = endOfCurrentScript;
  }

  *pointerToSubScriptsNumber = subScriptsNumber;
  *pointerToSubScripts = subScripts;
  *pointerToErrorMessage = NULL;

  *pointerToStackCommands = stackCommands;
  *pointerToStackCommandsNumber = stackCommandsNumber;
  return EXIT_SUCCESS;
}
