#ifndef SUBSCRIPTSSPLITER_H_
#define SUBSCRIPTSSPLITER_H_

#include "subScript.h"

int getSubScripts(char ***pointerToStringList, SUBSCRIPT currentScript,
                  SUBSCRIPT **pointerToSubScripts,
                  int *pointerToSubScriptsNumber,
                  char ***pointerToStackCommands,
                  int *pointerToStackCommandsNumber,
                  char **pointerToErrorMessage);

#endif