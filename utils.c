#include "utils.h"

#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int getUserName(char *username, size_t size) {
  if (username == NULL || size == 0)
    return 1;

  struct passwd *pw = getpwuid(getuid());

  if (pw == NULL) {
    perror("getpwuid");
    return 1;
  }

  strncpy(username, pw->pw_name, size - 1);
  username[size - 1] = '\0';

  return 0;
}

int getSystemName(char *systemName, size_t size) {
  if (systemName == NULL || size == 0)
    return 1;

  if (gethostname(systemName, size) == -1) {
    perror("gethostname");
    return 1;
  }

  systemName[size - 1] = '\0';

  return 0;
}

int getCurrentWorkingDirectory(char *currentWorkingDir, size_t size) {
  if (currentWorkingDir == NULL || size == 0)
    return 1;

  if (getcwd(currentWorkingDir, size) == NULL) {
    perror("getcwd");
    return 1;
  }

  return 0;
}
