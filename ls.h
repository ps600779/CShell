#ifndef LS_H
#define LS_H

#include "parser.h"
#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

void getPermissions(mode_t mode, char *permissions);

void printLongFormat(char *dName, struct stat *statbuf);

int lsHandler(Command *cmd);

#endif
