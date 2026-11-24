#define _PROGRAM_NAME "whoami"
// this is the comment to chek my github account
#include <pwd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "parser.h"
#include "utils.h"

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
      fprintf(stderr, "Failed to tokenize the input\n");
      continue;
    }

    // No tokens: nothing to run.
    if (cmd.argc == 0) {
      freeCommand(&cmd);
      continue;
    }

    if (strcmp(cmd.argv[0], "cd") == 0) {
      if (cdHandler(&cmd)) {
        fprintf(stderr, "Could Not cd now !!\n");
        freeCommand(&cmd);
        continue;
      }
    } else if (strcmp(cmd.argv[0], "pwd") == 0) {
      if (pwdHandler(&cmd)) {
        fprintf(stderr, "Could Not pwd now !!\n");
        freeCommand(&cmd);
        continue;
      }
    } else if (strcmp(cmd.argv[0], "echo") == 0) {
      if (echoHandler(&cmd)) {
        fprintf(stderr, "could Not Echo\n");
        freeCommand(&cmd);
        continue;
      }
    } else if (strcmp(cmd.argv[0], "exit") == 0) {
      if (cmd.argc > 1) {
        fprintf(stderr, "exit: Too Many Arguments\n");
        freeCommand(&cmd);
        continue;
      }
      printf("Shutting Down XO\n");
      break;
    }

    for (size_t i = 0; i < cmd.argc; i++) {
      printf("argv[%zu] = %s\n", i, cmd.argv[i]);
    }

    freeCommand(&cmd);
  }

  free(input);

  return 0;
}
