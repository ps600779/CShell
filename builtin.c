#include "builtins.h"
#include "parser.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char prev_dir[PATH_MAX] = "";

int cdHandler(Command *cmd) {
  if (cmd == NULL || cmd->argv == NULL) {
    return 1;
  }

  char *path;
  const char *home = "/Users/priyanshusharma/Documents/linux/lab/shell";

  if (cmd->argc > 2) {
    fprintf(stderr, "cd: too many arguments\n");
    return 1;
  }

  // cd -> home
  if (cmd->argc <= 1) {
    if (home == NULL) {
      fprintf(stderr, "cd: HOME not set\n");
      return 1;
    }
    path = (char *)home;
  } else {
    path = cmd->argv[1];
  }

  // cd ~
  if (strcmp(path, "~") == 0) {
    if (home == NULL) {
      fprintf(stderr, "cd: HOME not set\n");
      return 1;
    }
    path = (char *)home;
  }
  // cd -
  if (strcmp(path, "-") == 0) {
    if (prev_dir[0] == '\0') {
      fprintf(stderr, "cd: OLDPWD not set\n");
      return 1;
    }
    path = prev_dir;
  }

  // save the current directory for previous directory
  char old_dir[PATH_MAX];

  if (getcwd(old_dir, sizeof old_dir) == NULL) {
    perror("cd: getcwd");
    return 1;
  }

  // change directories
  if (chdir(path) == -1) {
    perror("cd");
    return 1;
  }

  // update prev_dir
  strcpy(prev_dir, old_dir);

  return 0;
}

int pwdHandler(Command *cmd) {
  if (cmd == NULL) {
    return 1;
  }

  if (cmd->argc > 1) {
    fprintf(stderr, "pwd: Too Many Arguments\n");
    return 1;
  }

  char pwd[PATH_MAX];
  if (getcwd(pwd, sizeof(pwd)) == NULL) {
    perror("pwd");
    return 1;
  }

  printf("%s\n", pwd);

  return 0;
}

int echoHandler(Command *cmd) {
  char *output;

  for (size_t i = 1; i < cmd->argc; i++) {
    printf("%s ", cmd->argv[i]);
  }

  printf("\n");

  return 0;
}
