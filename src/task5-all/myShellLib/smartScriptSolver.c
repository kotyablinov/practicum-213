#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "executeCommand.h"
#include "subScript.h"
#include "subScriptsSpliter.h"

int findAndExecuteChildScripts(char ***pointerToStringList,
                               SUBSCRIPT currentScript, char *fileIn,
                               char *fileOut, bool isAppendOut,
                               char **pointerToErrorMessage);

void freeScriptsAndCommands(SUBSCRIPT *childScripts, char **stackCommands,
                            char **tempFiles, int counterTempFiles);

bool checkIsBackgroundMode(char ***pointerToStringList,
                           SUBSCRIPT currentScript) {
  char **stringList = *pointerToStringList;
  char *lustWord = stringList[currentScript.endPosition];
  if (strcmp(lustWord, "&") == 0) {
    return true;
  } else {
    return false;
  }
}

int pushChildScriptExecutionInNewProcess(char ***pointerToStringList,
                                         SUBSCRIPT currentScript, char *fileIn,
                                         char *fileOut, bool isAppendOut,
                                         char **pointerToErrorMessage) {
  bool isBackgroundMode =
      checkIsBackgroundMode(pointerToStringList, currentScript);
  if (isBackgroundMode) {
    currentScript.endPosition--;
  }

  int errorFileDescriptor[2];
  int currentLocationFileDescriptor[2];

  pipe(errorFileDescriptor);
  pipe(currentLocationFileDescriptor);

  int pidNewProcess =
      fork();
  if (pidNewProcess < 0) {
    *pointerToErrorMessage =
        "Unfortunately, the attempt to create a process for executing the main "
        "script was not crowned with success.";
    return EXIT_FAILURE;
  } else if (pidNewProcess == 0) {
    // сын
    close(errorFileDescriptor[0]);
    close(currentLocationFileDescriptor[0]);

    char *errorMessage = "";
    // выполнить скрипт
    int code =
        findAndExecuteChildScripts(pointerToStringList, currentScript, fileIn,
                                   fileOut, isAppendOut, &errorMessage);
    // записали текущую ошибку
    if (code != 0) {
      write(errorFileDescriptor[1], errorMessage, strlen(errorMessage) * sizeof(char));
    }
    close(errorFileDescriptor[1]);

    // записали текущий путь окружения
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      write(currentLocationFileDescriptor[1], cwd, strlen(cwd) * sizeof(char));
    }
    close(currentLocationFileDescriptor[1]);

    exit(code);
  } else {
    close(errorFileDescriptor[1]);
    close(currentLocationFileDescriptor[1]);

    if (isBackgroundMode) {
      waitpid(-1, NULL, WNOHANG);
    } else {
      int status;
      waitpid(pidNewProcess, &status, 0);
      if (!WIFEXITED(status) || WEXITSTATUS(status)) {
        int bufferSize = 1024;
        char errorMessage[bufferSize];
        memset(errorMessage, 0, sizeof(errorMessage));
        read(errorFileDescriptor[0], errorMessage, sizeof(errorMessage));
        close(errorFileDescriptor[0]);
        *pointerToErrorMessage = errorMessage;
        return EXIT_FAILURE;
      }
    }
    // меняем текущий путь
    char pathOfContext[PATH_MAX];
    memset(pathOfContext, 0, sizeof(pathOfContext));
    read(currentLocationFileDescriptor[0], pathOfContext, sizeof(pathOfContext));
    chdir(pathOfContext);

    close(errorFileDescriptor[0]);
    close(currentLocationFileDescriptor[0]);
  }

  return EXIT_SUCCESS;
}

void freeScriptsAndCommands(SUBSCRIPT *childScripts, char **stackCommands,
                            char **tempFiles, int counterTempFiles) {
  if (childScripts != NULL) free(childScripts);
  if (stackCommands != NULL) free(stackCommands);

  if (tempFiles != NULL) {
    for (int currentFile = 0; currentFile < counterTempFiles; currentFile++) {
      removeFile(tempFiles[currentFile]);
      free(tempFiles[currentFile]);
    }
    free(tempFiles);
  }
}

int findAndExecuteChildScripts(char ***pointerToStringList,
                               SUBSCRIPT currentScript, char *fileIn,
                               char *fileOut, bool isAppendOut,
                               char **pointerToErrorMessage) {
  char *errorMessage = NULL;
  int code;

  // Проверка наличия в текущем скрипте других подскриптов
  SUBSCRIPT *childScripts = NULL;
  int childScriptsNumber = 0;
  // Стек команд
  char **stackCommands = NULL;
  int stackCommandsNumber = 0;

  char **tempFiles = NULL;
  int counterTempFiles = 0;

  code = getSubScripts(pointerToStringList, currentScript, &childScripts,
                       &childScriptsNumber, &stackCommands,
                       &stackCommandsNumber, &errorMessage);
  if (code != EXIT_SUCCESS) {
    *pointerToErrorMessage = errorMessage;
    freeScriptsAndCommands(childScripts, stackCommands, tempFiles,
                           counterTempFiles);
    return EXIT_FAILURE;
  }

  if (childScriptsNumber != 1) {
    char *previousFileName = NULL;

    int statusCodePrevious = EXIT_SUCCESS;
    char *previousErrorMessage = NULL;

    int cursorCommands = 0;

    for (int cursorScripts = 0; cursorScripts < childScriptsNumber;
         cursorScripts++) {
      SUBSCRIPT childScript = childScripts[cursorScripts];

      char *childFileIn = NULL;
      char *childFileOut = NULL;

      bool isChildAppend = false;

      bool isSkipCurrentChildScript = false;
      bool isSkipNextChildScript = false;

      if (cursorScripts == 0) {
        // если первый скрипт
        childFileIn = fileIn;
      } else {
        childFileIn = previousFileName;
      }
      if (cursorCommands == childScriptsNumber - 1) {
        isChildAppend = isAppendOut;
        childFileOut = fileOut;
      }

      // принимаем решение о выполнении текущего дочернего скрипта
      char *commandBeforeChildScript = NULL;
      if ((cursorCommands - 1) < stackCommandsNumber &&
          (cursorCommands - 1) >= 0) {
        commandBeforeChildScript = stackCommands[cursorCommands - 1];
      }
      if (commandBeforeChildScript) {
        if (strcmp(commandBeforeChildScript, "&&") == 0 &&
            statusCodePrevious != EXIT_SUCCESS) {
          isSkipCurrentChildScript = true;
        } else if (strcmp(commandBeforeChildScript, "||") == 0 &&
                   statusCodePrevious == EXIT_SUCCESS) {
          isSkipCurrentChildScript = true;
        } else if (strcmp(commandBeforeChildScript, ";") == 0) {
          isSkipCurrentChildScript = false;
        }
      }

      // решаем проблемы с перенаправленнием потоков
      char *commandAfterChildScript = NULL;
      if (cursorCommands < stackCommandsNumber) {
        commandAfterChildScript = stackCommands[cursorCommands];
        cursorCommands++;
      }

      if (commandAfterChildScript) {
        bool isWrite = (strcmp(commandAfterChildScript, ">") == 0) ||
                       (strcmp(commandAfterChildScript, ">>") == 0);
        bool isRead = (strcmp(commandAfterChildScript, "<") == 0);
        if (isWrite || isRead) {
          if (cursorScripts + 1 >= childScriptsNumber) {
            *pointerToErrorMessage = "File name is missing.";
            freeScriptsAndCommands(childScripts, stackCommands, tempFiles,
                                   counterTempFiles);
            return EXIT_FAILURE;
          }
          SUBSCRIPT fileScript = childScripts[cursorScripts + 1];

          if (fileScript.startPosition != fileScript.endPosition) {
            *pointerToErrorMessage = "File name is missing.";
            freeScriptsAndCommands(childScripts, stackCommands, tempFiles,
                                   counterTempFiles);
            return EXIT_FAILURE;
          }

          if (isWrite) {
            isChildAppend = strcmp(commandAfterChildScript, ">>") == 0;
            childFileOut = (*pointerToStringList)[fileScript.startPosition];
          } else {
            childFileIn = (*pointerToStringList)[fileScript.startPosition];
          }
        }
        if (strcmp(commandAfterChildScript, "|") == 0) {
          childFileOut = NULL;
          int fd = getNewTemporaryFileDescriptor(&childFileOut, &previousErrorMessage);
          if (fd < 0) {
            isSkipCurrentChildScript = true;
            statusCodePrevious = fd;
            freeScriptsAndCommands(childScripts, stackCommands, tempFiles,
                                   counterTempFiles);
          } else {
            counterTempFiles++;
            if (tempFiles == NULL) {
              tempFiles = (char **)malloc(sizeof(char *));
            } else {
              tempFiles = (char **)realloc(tempFiles,
                                           counterTempFiles * sizeof(char *));
            }
            tempFiles[counterTempFiles - 1] = childFileOut;
          }
        }
      }

      if (!isSkipCurrentChildScript) {
        statusCodePrevious = pushChildScriptExecutionInNewProcess(
            pointerToStringList, childScript, childFileIn, childFileOut,
            isChildAppend, &previousErrorMessage);
      }

      if (childFileOut != NULL) {
        previousFileName = childFileOut;
      }

      // если пропустили, то созданная труба для вывода не будет доступна
      if (isSkipNextChildScript) {
        cursorScripts++;
        cursorCommands++;
      }
    }
  } else {
    code = executeCommand(pointerToStringList, childScripts[0].startPosition,
                          childScripts[0].endPosition, fileIn, fileOut,
                          isAppendOut, &errorMessage);
  }

  freeScriptsAndCommands(childScripts, stackCommands, tempFiles,
                         counterTempFiles);

  if (code != EXIT_SUCCESS) {
    *pointerToErrorMessage = errorMessage;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int executeScript(char ***pointerToStringList, SUBSCRIPT currentScript,
                  char **pointerToErrorMessage) {
  char *errorMessage = "";

  int code = pushChildScriptExecutionInNewProcess(
      pointerToStringList, currentScript, NULL, NULL, false, &errorMessage);
  if (code != EXIT_SUCCESS) *pointerToErrorMessage = errorMessage;
  return code;
}