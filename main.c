#include <stddef.h>
#define _PROGRAM_NAME "whoami"
#include "utils.h"
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  char userName[256];
  char systemName[256];
  char currentWorkingDir[1024];

  if (getUserName(userName, sizeof(userName))) {
    fprintf(stderr, "Unable to get username\n");
    return 1;
  }

  if (getSystemName(systemName, sizeof(systemName))) {
    fprintf(stderr, "Unable to get systemName\n");
    return 1;
  }

  if (getCurrentWorkingDirectory(currentWorkingDir,
                                 sizeof(currentWorkingDir))) {
    fprintf(stderr, "Unable to get CWD\n");
    return 1;
  }

  while (1) {
    // implement the status message here
    printf("%s@%s:%s>", userName, systemName, currentWorkingDir);
    // implement user input
    char input[100];
    if (fgets(input, sizeof input, stdin) != NULL) {
      input[strcspn(input, "\n")] = '\0';

      printf("You entered : %s\n", input);
      break;
    }
  }
  return 0;
}
