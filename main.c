#define _PROGRAM_NAME "whoami"

#include <pwd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"
#include "utils.h"

int cdHandler(Command *cmd) {
  char *path;
  if (cmd->argc == 1) {
    path = ".";
  } else if (cmd->argc == 2) {
    path = cmd->argv[1];
  } else {
    fprintf(stderr, "Too Many Arguments\n");
    return 1;
  }

  if (chdir(path) == 0) {

  } else {
  }

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
