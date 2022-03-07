#include <fcntl.h>
#include <malloc.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "executeCommand.h"

void error(int *processes, int num_of_pr) {
  long long j;
  for (j = 0; j < num_of_pr; ++j) {
    kill(processes[j], SIGKILL);
  }
  while (1) {
    if (wait(NULL) == -1) {
      break;
    }
  }
}

int executePiping(char ****pointerToProcessList,
                  int *pointerToProcessListNumber,
                  int **pointerToProcessLineLength) {
  char ***processList = *pointerToProcessList;
  int processListNumber = *pointerToProcessListNumber;
  int *processLineLength = *pointerToProcessLineLength;

  if (processListNumber == 0) {
    return 0;
  }

  char *lastCommand = processList[processListNumber - 1][processLineLength[processListNumber - 1] - 1];
  bool inBackground = strcmp(lastCommand, "&") == 0;
  if (inBackground) {
    free(lastCommand);
    processLineLength[processListNumber - 1]--;
  }

  int processesNumber = 0, *processes;
  int prevReadFileDesc = -1, pid;
  int errorFile;
  long long currentProcess;
  errorFile = open("/dev/null", O_WRONLY);
  if (dup2(errorFile, 2) == -1) {
    // переводим поток ошибок в вечно пустой файл
    perror("dup2: ");
    _exit(0);
  }
  close(errorFile);

  processes = NULL;

  int fileDesc[2];
  for (currentProcess = 0; currentProcess < processListNumber;
       currentProcess++) {
    if (pipe(fileDesc) == -1) {
      // ошибка pipe
      error(processes, processesNumber);
      perror("pipe: ");
      _exit(0);
    }

    pid = fork();  // создаём процесс для новой команды
    if (pid < 0) {
      // ошибка создания процесса
      error(processes, processesNumber);
      perror("fork: ");
      free(processes);
      _exit(0);
    }

    if (pid == 0) {
      // сын выполняет команду
      if (currentProcess != 0) {
        if (dup2(prevReadFileDesc, STDIN_FILENO) == -1) {
          perror("dup2: ");
          _exit(0);
        }
        close(prevReadFileDesc);
      }
      if (currentProcess != processListNumber - 1) {
        if (dup2(fileDesc[1], STDOUT_FILENO) == -1) {
          perror("dup2: ");
          _exit(0);
        }
      }
      close(fileDesc[0]);
      close(fileDesc[1]);

      executeList(&processList[currentProcess],
                  &processLineLength[currentProcess]);
    }

    if (pid != 0) {
      // отец
      if (processes == NULL) {
        processes = (int *)malloc(sizeof(int));
      } else {
        processes =
            (int *)realloc(processes, (processesNumber + 1) * sizeof(int));
      }

      processes[processesNumber] = pid;
      processesNumber++;

      // открываем и закрываем нужные дескрипторы на чтение и на запись
      if (currentProcess == 0) {
        // первая команда
        prevReadFileDesc = fileDesc[0];
      } else if (currentProcess == processListNumber - 1) {
        // последняя команда
        close(prevReadFileDesc);
        close(fileDesc[0]);
      } else {
        // команда в середине
        close(prevReadFileDesc);
        prevReadFileDesc = fileDesc[0];
      }
      close(fileDesc[1]);
    }
  }
  for (int currentPid = 0; currentPid < processesNumber; currentPid++) {
    if (inBackground) {
      waitpid(-1, NULL, WNOHANG);

    } else {
      waitpid(processes[currentPid], NULL, 0);
    }
  }
  free(processes);
  return 0;
}