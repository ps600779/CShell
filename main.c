#define _PROGRAM_NAME "whoami"
// this is the comment to chek my github account
#include <pwd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"
#include "utils.h"

static char prev_dir[PATH_MAX] = "";

static char home_dir[PATH_MAX] =
    "/Users/priyanshusharma/Documents/linux/lab/shell";

int cdHandler(Command *cmd) {
  char *path;
  char *home = home_dir;

  if (cmd->argc > 2) {
    fprintf(stderr, "cd: too many arguments\n");
    return 1;
  }

  // cd -> home
  if (cmd->argc == 1) {
    if (home == NULL) {
      fprintf(stderr, "cd: HOME not set\n");
      return 1;
    }
    path = home;
  } else {
    path = cmd->argv[1];
  }

  // cd ~
  if (strcmp(path, "~") == 0) {
    if (home == NULL) {
      fprintf(stderr, "cd: Home Not Set\n");
      return 1;
    }
    path = home;
  }
  // cd -
  if (strcmp(path, "-") == 0) {
    if (prev_dir[0] == '\0') {
      fprintf(stderr, "cd: OLDPWD not ser\n");
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

  // change ditectories
  if (chdir(path) == -1) {
    perror("cd");
    return 1;
  }

  // update prev_dir
  strcpy(prev_dir, old_dir);

  return 0;
}

int main(void) {
  char userName[256];
  char systemName[256];
  char currentWorkingDir[1024];

  if (getUserName(userName, sizeof(userName))) {
    fprintf(stderr, "Unable to get username\n");
    return 1;
  }

  if (getSystemName(systemName, sizeof(systemName))) {
    fprintf(stderr, "Unable to get system name\n");
    return 1;
  }

  if (getCurrentWorkingDirectory(currentWorkingDir,
                                 sizeof(currentWorkingDir))) {
    fprintf(stderr, "Unable to get CWD\n");
    return 1;
  }

  char *input = NULL;
  size_t inputCapacity = 0;

  while (1) {

    if (getCurrentWorkingDirectory(currentWorkingDir,
                                   sizeof currentWorkingDir)) {
      strcpy(currentWorkingDir, "?");
    }

    // Display shell prompt
    printf("%s@%s:%s> ", userName, systemName, currentWorkingDir);

    fflush(stdout);

    // Read user input dynamically
    ssize_t bytesRead = getline(&input, &inputCapacity, stdin);

    // Handle EOF (Ctrl+D) or input error
    if (bytesRead == -1) {
      printf("\n");
      break;
    }

    // Remove trailing newline
    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) == 0) {
      continue;
    }

    Command cmd;

    if (!parseCommand(input, &cmd)) {
      fprintf(stderr, "Failed to Tokenize the input");
      continue;
    }

    if (strcmp(cmd.argv[0], "cd") == 0) {
      if (cdHandler(&cmd)) {
        fprintf(stderr, "Could Not cd now !!");
      }
    }

    for (size_t i = 0; i < cmd.argc; i++) {
      printf("argv[%zu] = %s\n", i, cmd.argv[i]);
    }

    freeCommand(&cmd);
  }

  free(input);

  return 0;
}
