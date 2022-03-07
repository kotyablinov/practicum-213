#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void addWordToStringList(char ***pointerToStringList,
                         int *pointerToStringListNumber,
                         char **pointerToCurrentWord,
                         int *pointerToWordLength) {
  int wordLength = *pointerToWordLength;
  int stringListNumber = *pointerToStringListNumber;

  char **stringList = *pointerToStringList;
  char *currentWord = *pointerToCurrentWord;

  if (stringList == NULL) {
    stringList = (char **)malloc(sizeof(char *));
  } else {
    stringList =
        (char **)realloc(stringList, (stringListNumber + 1) * sizeof(char *));
  }

  stringList[stringListNumber] = currentWord;
  stringListNumber++;

  currentWord = NULL;
  wordLength = 0;

  *pointerToStringList = stringList;
  *pointerToCurrentWord = currentWord;
  *pointerToWordLength = wordLength;
  *pointerToStringListNumber = stringListNumber;
}

void addCharToString(char **pointerToCurrentString,
                     int *pointerToCurrentStringLength, char inputChar) {
  char *currentString = *pointerToCurrentString;
  int currentStringLength = *pointerToCurrentStringLength;

  if (currentString == NULL) {
    currentString = (char *)malloc(sizeof(char));
  } else {
    currentString = (char *)realloc(currentString,
                                    (currentStringLength + 1) * sizeof(char));
  }
  currentString[currentStringLength] = inputChar;
  currentStringLength++;

  *pointerToCurrentString = currentString;
  *pointerToCurrentStringLength = currentStringLength;
}

void parseLines(char *currentString, int characterNumber, int firstQuote,
                int lastQuote, char ***pointerToStringList,
                int *pointerToStringNumber) {
  char **stringList = NULL;
  int stringListNumber = 0;

  //обрабатываем строку
  int currentPosition = 0;
  int wordLength = 0;
  char *currentWord = NULL;

  bool isNewWord, isDoubleSymbol, isOneSymbol;

  while (currentPosition < characterNumber - 1) {
    char checkChar = currentString[currentPosition];
    isNewWord = (checkChar == ' ') || (checkChar == '\"');
    isDoubleSymbol =
        (checkChar == '&') || (checkChar == '|') || (checkChar == '>');
    isOneSymbol = (checkChar == ';') || (checkChar == '<') ||
                  (checkChar == '(') || (checkChar == ')');
    if (firstQuote == currentPosition) {
      if (wordLength != 0) {
        addCharToString(&currentWord, &wordLength, '\0');
        addWordToStringList(&stringList, &stringListNumber, &currentWord,
                            &wordLength);
      }
      if (lastQuote - firstQuote > 1) {
        int tempPosition = 0;
        while (currentPosition < lastQuote) {
          checkChar = currentString[currentPosition];
          if (checkChar != '\"') {
            addCharToString(&currentWord, &tempPosition, checkChar);
          }
          currentPosition++;
        }
        addCharToString(&currentWord, &tempPosition, '\0');
        addWordToStringList(&stringList, &stringListNumber, &currentWord,
                            &tempPosition);
      } else {
        currentPosition++;
      }
    } else if (isDoubleSymbol) {
      if (wordLength != 0) {
        addCharToString(&currentWord, &wordLength, '\0');
        addWordToStringList(&stringList, &stringListNumber, &currentWord,
                            &wordLength);
      }

      int doubleLength = 0;
      if (currentString[currentPosition + 1] == checkChar) {
        addCharToString(&currentWord, &doubleLength, checkChar);
        addCharToString(&currentWord, &doubleLength, checkChar);
        addCharToString(&currentWord, &doubleLength, '\0');
        currentPosition++;
      } else {
        addCharToString(&currentWord, &doubleLength, checkChar);
        addCharToString(&currentWord, &doubleLength, '\0');
      }

      addWordToStringList(&stringList, &stringListNumber, &currentWord,
                          &doubleLength);
    } else if (isOneSymbol) {
      if (wordLength != 0) {
        addCharToString(&currentWord, &wordLength, '\0');
        addWordToStringList(&stringList, &stringListNumber, &currentWord,
                            &wordLength);
      }

      int oneLength = 0;
      addCharToString(&currentWord, &oneLength, checkChar);
      addCharToString(&currentWord, &oneLength, '\0');

      addWordToStringList(&stringList, &stringListNumber, &currentWord,
                          &oneLength);
    } else if (isNewWord) {
      if (wordLength != 0) {
        addCharToString(&currentWord, &wordLength, '\0');
        addWordToStringList(&stringList, &stringListNumber, &currentWord,
                            &wordLength);
      }
    } else {
      addCharToString(&currentWord, &wordLength, checkChar);
    }
    currentPosition++;
  }

  if (wordLength != 0) {
    addCharToString(&currentWord, &wordLength, '\0');
    addWordToStringList(&stringList, &stringListNumber, &currentWord,
                        &wordLength);
  } else {
    free(currentWord);
  }
  *pointerToStringList = stringList;
  *pointerToStringNumber = stringListNumber;
}

bool readLines(char ***pointerToStringList, int *pointerToStringNumber) {
  int firstQuote, lastQuote;

  char *currentString = NULL;
  int currentStringLength = 0;

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
        firstQuote = currentStringLength;
      }
      lastQuote = currentStringLength;
    }

    addCharToString(&currentString, &currentStringLength, inputChar);
    inputChar = getchar();
  }

  addCharToString(&currentString, &currentStringLength, '\0');

  parseLines(currentString, currentStringLength, firstQuote, lastQuote,
             pointerToStringList, pointerToStringNumber);
  free(currentString);

  if (inputChar == EOF) {
    return false;
  }

  return true;
}