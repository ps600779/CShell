#include <stddef.h>
// #include "parser.h"
typedef struct {
  char **argv;
  size_t argc;
  size_t capacity;
} Command;

int parseCommand(char *input, Command *cmd);

void freeCommand(Command *cmd);
