#define _PROGRAM_NAME "whoami"

#include <pwd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

    for (size_t i = 0; i < cmd.argc; i++) {
      printf("argv[%zu] = %s\n", i, cmd.argv[i]);
    }

    freeCommand(&cmd);
  }

  free(input);

  return 0;
}
