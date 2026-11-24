#include "parser.h"

#include <ctype.h>
#include <stdbool.h>
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

    // Skip whitespace between tokens
    while (isspace((unsigned char)*ptr)) {
      ptr++;
    }

    if (*ptr == '\0') {
      break;
    }

    // Allocate temporary buffer for the token
    size_t maxLength = strlen(ptr);
    char *token = malloc(maxLength + 1);

    if (token == NULL) {
      freeCommand(cmd);
      return 0;
    }

    size_t j = 0;
    char quote_char = '\0';

    while (*ptr != '\0') {

      // Handle opening and closing quotes
      if (*ptr == '"' || *ptr == '\'') {

        if (quote_char == '\0') {
          quote_char = *ptr;
          ptr++;
          continue;
        } else if (quote_char == *ptr) {
          quote_char = '\0';
          ptr++;
          continue;
        }
      }

      // Whitespace ends token only outside quotes
      if (isspace((unsigned char)*ptr) && quote_char == '\0') {
        break;
      }

      token[j++] = *ptr;
      ptr++;
    }

    // Unmatched quote detection
    if (quote_char != '\0') {
      free(token);
      freeCommand(cmd);
      return 0;
    }

    token[j] = '\0';

    if (!addArgument(cmd, token, j)) {
      free(token);
      freeCommand(cmd);
      return 0;
    }

    free(token);
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
