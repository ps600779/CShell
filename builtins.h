#ifndef BUILTINS_H
#define BUILTINS_H

#include "parser.h"

#include <limits.h>

extern char prev_dir[PATH_MAX];

int cdHandler(Command *cmd);
int pwdHandler(Command *cmd);
int echoHandler(Command *cmd);

#endif
