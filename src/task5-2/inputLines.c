#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void completeWord(int *pointerToWordLength, int *pointerToStringNumber,
                  char **pointerToCurrentWord, char ***pointerToStringList) {
  int wordLength = *pointerToWordLength;
  int stringNumber = *pointerToStringNumber;

  char **stringList = *pointerToStringList;
  char *currentWord = *pointerToCurrentWord;
  currentWord[wordLength - 1] = '\0';
  stringList[stringNumber - 1] = currentWord;
  stringNumber++;
  stringList = (char **)realloc(stringList, stringNumber * sizeof(char *));
  currentWord = (char *)malloc(sizeof(char));
  wordLength = 1;

  *pointerToStringList = stringList;
  *pointerToCurrentWord = currentWord;
  *pointerToWordLength = wordLength;
  *pointerToStringNumber = stringNumber;
}

bool readLines(char ***pointerToStringList, int *pointerToStringNumber) {
  char **stringList = NULL;
  int stringNumber = 0;

  char *currentWord = NULL;
  bool isNewWord, isDoubleSymbol, isOneSymbol;
  int characterNumber, firstQuote, lastQuote, currentPosition, wordLength;

  char *currentString = (char *)malloc(sizeof(char));
  stringList = (char **)malloc(sizeof(char *));
  stringNumber = 1;
  characterNumber = 1;
  firstQuote = -1;
  lastQuote = -1;

  char inputChar = getchar();
  if (inputChar == EOF) {
    return false;
  }
  //считываем всю строку
  while ((inputChar != '\n') && (inputChar != EOF)) {
    if (inputChar == '"') {
      if (firstQuote == -1) {
        firstQuote = characterNumber - 1;
      }
      lastQuote = characterNumber - 1;
    }
    currentString[characterNumber - 1] = inputChar;
    characterNumber++;
    currentString =
        (char *)realloc(currentString, characterNumber * sizeof(char));
    inputChar = getchar();
  }
  currentString[characterNumber - 1] = '\0';
  //обрабатываем строку
  currentPosition = 0;
  wordLength = 1;
  currentWord = (char *)malloc(sizeof(char));
  while (currentPosition < characterNumber - 1) {
    char checkChar = currentString[currentPosition];
    isNewWord = (checkChar == ' ') || (checkChar == '\"');
    isDoubleSymbol =
        (checkChar == '&') || (checkChar == '|') || (checkChar == '>');
    isOneSymbol = (checkChar == ';') || (checkChar == '<') ||
                  (checkChar == '(') || (checkChar == ')');
    if (firstQuote == currentPosition) {
      if (wordLength != 1) {
        completeWord(&wordLength, &stringNumber, &currentWord, &stringList);
      }
      if (lastQuote - firstQuote > 1) {
        currentWord = (char *)realloc(currentWord,
                                      (lastQuote - firstQuote) * sizeof(char));
        int tempPosition = 0;
        while (currentPosition < lastQuote) {
          checkChar = currentString[currentPosition];
          if (checkChar != '\"') {
            currentWord[tempPosition] = checkChar;
            tempPosition++;
          }
          currentPosition++;
        }
        currentWord[tempPosition] = '\0';
        stringList[stringNumber - 1] = currentWord;
        stringNumber++;
        stringList =
            (char **)realloc(stringList, stringNumber * sizeof(char *));
        currentWord = (char *)malloc(sizeof(char));
      } else {
        currentPosition++;
      }
    } else if (isDoubleSymbol) {
      if (wordLength != 1) {
        completeWord(&wordLength, &stringNumber, &currentWord, &stringList);
      }

      if (currentString[currentPosition + 1] == checkChar) {
        currentWord = (char *)realloc(currentWord, 3 * sizeof(char));
        currentWord[0] = checkChar;
        currentWord[1] = checkChar;
        currentWord[2] = '\0';
        currentPosition++;
      } else {
        currentWord = (char *)realloc(currentWord, 2 * sizeof(char));
        currentWord[0] = checkChar;
        currentWord[1] = '\0';
      }
      stringList[stringNumber - 1] = currentWord;
      stringNumber++;
      stringList = (char **)realloc(stringList, stringNumber * sizeof(char *));
      currentWord = (char *)malloc(sizeof(char));
    } else if (isOneSymbol) {
      if (wordLength != 1) {
        completeWord(&wordLength, &stringNumber, &currentWord, &stringList);
      }
      currentWord = (char *)realloc(currentWord, 2 * sizeof(char));
      currentWord[0] = checkChar;
      currentWord[1] = '\0';
      stringList[stringNumber - 1] = currentWord;
      stringNumber++;
      stringList = (char **)realloc(stringList, stringNumber * sizeof(char *));
      currentWord = (char *)malloc(sizeof(char));
    } else if (isNewWord) {
      if (wordLength != 1) {
        completeWord(&wordLength, &stringNumber, &currentWord, &stringList);
      }
    } else {
      currentWord[wordLength - 1] = checkChar;
      wordLength++;
      currentWord = (char *)realloc(currentWord, wordLength * sizeof(char));
    }
    currentPosition++;
  }

  if (wordLength != 1) {
    completeWord(&wordLength, &stringNumber, &currentWord, &stringList);
  } else {
    free(currentWord);
  }
  free(currentString);

  if (inputChar == EOF) {
    return false;
  }

  *pointerToStringList = stringList;
  *pointerToStringNumber = stringNumber;
  return true;
}