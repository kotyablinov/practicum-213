#ifndef EXECUTECOMMAND_H_
#define EXECUTECOMMAND_H_

int getNewTempFileDesc(char **pointerFileName, char **pointerToErrorMessage);
int removeFile(char *filename);

int executeCommand(char ***pointerToStringList, int startCommand,
                   int endCommand, char *fileIn, char *fileOut,
                   bool isAppendOut, char **pointerToErrorMessage);
#endif