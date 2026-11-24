#include "parser.h"

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static int addArgument(Command *cmd, char *start, size_t length) {
  //
  // grow argv when full(reserve space for null)

  if (cmd->argc + 1 >= cmd->capacity) {
    size_t newCapacity = cmd->capacity * 2;

    char **temp = realloc(cmd->argv, newCapacity * sizeof(char *));

    if (temp == NULL) {
      return 0;
    }

    cmd->argv = temp;
    cmd->capacity = newCapacity;
  }

  // Allocate memory for this token;
  char *token = malloc(length + 1);

  if (token == NULL) {
    return 0;
  }

  memcpy(token, start, length);

  token[length] = '\0';

  cmd->argv[cmd->argc++] = token;
  cmd->argv[cmd->argc] = NULL;

  return 1;
}

int parseCommand(char *input, Command *cmd) {
  cmd->argc = 0;
  cmd->capacity = 4;
  cmd->argv = malloc(cmd->capacity * sizeof(char *));

  if (cmd->argv == NULL) {
    cmd->capacity = 0;
    return 0;
  }

  cmd->argv[0] = NULL;

  char *ptr = input;

  while (*ptr != '\0') {
    // skip whitespaces
    while (isspace((unsigned char)*ptr)) {
      ptr++;
    }

    if (*ptr == '\0') {
      break;
    }

    // mark the start of the token
    char *start = ptr;

    // find token end
    while (*ptr != '\0' && !isspace((unsigned char)*ptr)) {
      ptr++;
    }

    size_t length = ptr - start;

    if (!addArgument(cmd, start, length)) {
      freeCommand(cmd);
      return 0;
    }
  }

  return 1;
}

// free the allocated memory
void freeCommand(Command *cmd) {
  if (cmd == NULL) {
    return;
  }

  if (cmd->argv != NULL) {
    for (size_t i = 0; i < cmd->argc; i++) {
      free(cmd->argv[i]);
    }
  }

  free(cmd->argv);

  cmd->argv = NULL;
  cmd->argc = 0;
  cmd->capacity = 0;
}
