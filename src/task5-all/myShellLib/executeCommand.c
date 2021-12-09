#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1

int getNewTempFileDesc(char **pointerFileName, char **pointerToErrorMessage) {
  char filename[] = "tempfile-XXXXXX";
  int fd;
  if ((fd = mkstemp(filename)) == -1) {
    *pointerToErrorMessage = "Failed to create a temporary file.";
    return EXIT_FAILURE;
  }
  *pointerFileName = (char *)malloc(strlen(filename) + 1);
  strcpy(*pointerFileName, filename);
  return EXIT_SUCCESS;
}

int removeFile(char *filename) { return remove(filename); }

int getWriteFileDesc(char *filename, bool isAppend,
                     char **pointerToErrorMessage) {
  int fd = -1;
  if (filename) {
    if (isAppend) {
      fd = open(filename, O_CREAT | O_APPEND, 0666);
    } else {
      fd = open(filename, O_CREAT | O_WRONLY, 0666);
    }
  }
  if (filename == NULL || fd < 0) {
    *pointerToErrorMessage = "Could not open the file for writing";
    return EXIT_FAILURE;
  }
  return fd;
}

int getReadFileDesc(char *filename, char **pointerToErrorMessage) {
  int fd = EXIT_FAILURE;
  if (filename) {
    fd = open(filename, O_RDONLY, 0666);
  }
  if (filename == NULL || fd < 0) {
    *pointerToErrorMessage = "Could not open the file for writing";
    return EXIT_FAILURE;
  }
  return fd;
}

int duplicateFileDescriptors(char *fileIn, char *fileOut, bool isAppendOut,
                             char **pointerToErrorMessage) {
  if (fileIn != NULL) {
    int fileDescIn = getReadFileDesc(fileIn, pointerToErrorMessage);
    if (dup2(fileDescIn, STDIN_FILENO) == -1) {
      *pointerToErrorMessage =
          "Something went wrong when creating a duplicate file descriptor "
          "(STDIN).";
      return EXIT_FAILURE;
    }
  }
  if (fileOut) {
    int fileDescOut =
        getWriteFileDesc(fileOut, isAppendOut, pointerToErrorMessage);
    if (dup2(fileDescOut, STDOUT_FILENO) == -1) {
      *pointerToErrorMessage =
          "Something went wrong when creating a duplicate file descriptor "
          "(STDOUT).";
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

int executeChangeDirectory(char ***pointerToStringList, int startCommand,
                           int endCommand, char **pointerToErrorMessage) {
  int numberArguments = endCommand - startCommand;
  bool isChangeToHome = (numberArguments == 0) ||
                        (strcmp((*pointerToStringList)[endCommand], "~") == 0);
  if (isChangeToHome) {
    chdir(getenv("HOME"));
  } else if (numberArguments == 1) {
    int result = chdir((*pointerToStringList)[endCommand]);
    if (result != 0) {
      *pointerToErrorMessage = "Wrong path.";
      return EXIT_FAILURE;
    }
  } else {
    *pointerToErrorMessage =
        "The number of arguments of the cd command is greater than 1.";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int executeCommand(char ***pointerToStringList, int startCommand,
                   int endCommand, char *fileIn, char *fileOut,
                   bool isAppendOut, char **pointerToErrorMessage) {
  if (strcmp((*pointerToStringList)[startCommand], "cd") == 0) {
    char *errorMessage = NULL;

    int code = executeChangeDirectory(pointerToStringList, startCommand,
                                      endCommand, &errorMessage);
    if (code != EXIT_SUCCESS) {
      *pointerToErrorMessage = errorMessage;
      return EXIT_FAILURE;
    }

  } else {
    int status;
    int pid = fork();

    if (pid < 0) {
      *pointerToErrorMessage =
          "Failed to create a process to execute the command. It worked "
          "before.";
      return EXIT_FAILURE;
    } else if (pid == 0) {
      int code = duplicateFileDescriptors(fileIn, fileOut, isAppendOut,
                                          pointerToErrorMessage);
      if (code != EXIT_SUCCESS) {
        exit(code);
      }
      int sizeOfCommand = endCommand - startCommand + 2;
      char **commandForExecute =
          (char **)malloc(sizeOfCommand * sizeof(char *));
      for (int currentWord = startCommand; currentWord <= endCommand;
           currentWord++) {
        commandForExecute[currentWord - startCommand] =
            (*pointerToStringList)[currentWord];
      }
      commandForExecute[sizeOfCommand - 1] = NULL;
      int processStatus = execvp(commandForExecute[0], commandForExecute);
      free(commandForExecute);
      exit(processStatus);
    } else {
      if (waitpid(pid, &status, 0) > 0) {
        // closeTube(tubeIn);
        // closeTube(tubeOut);
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
          return EXIT_SUCCESS;
        } else if (WIFEXITED(status) && WEXITSTATUS(status)) {
          if (WEXITSTATUS(status) == 127) {
            *pointerToErrorMessage = "Execv failed.";
            return EXIT_FAILURE;
          } else {
            *pointerToErrorMessage =
                "Program terminated normally, but returned a non-zero status.";
            return EXIT_FAILURE;
          }
        } else {
          *pointerToErrorMessage = "Program didn't terminate normally.";
          return EXIT_FAILURE;
        }
      } else {
        *pointerToErrorMessage = "Waitpid() failed.";
        return EXIT_FAILURE;
      }
    }
  }
  return EXIT_SUCCESS;
}