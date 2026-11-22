#include <stddef.h>
#define _PROGRAM_NAME "whoami"
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int getUserName(char *username, const size_t size) {
  struct passwd *pw = getpwuid(getuid());

  if (!pw)
    return 1;

  strncpy(username, pw->pw_name, size - 1);
  username[size - 1] = '\0';

  return 0;
}

int getSystemName(char *systemNamef, const size_t size) {
  if (gethostname(systemNamef, size) == -1) {
    perror("gethostname");
    return 1;
  }

  systemNamef[size - 1] = '\0';

  return 0;
}

int getCurrentWorkingDirectory(char *currentWorkingDir, const size_t size) {
  if (getcwd(currentWorkingDir, size) == NULL) {
    perror("cannot get CWD");
    return 1;
  }

  currentWorkingDir[size - 1] = '\0';

  return 0;
}

int main() {
  char userName[256];
  char systemName[256];
  char currentWorkingDir[1024];

  if (getUserName(userName, sizeof(userName))) {
    printf("Unable to get username");
    return 1;
  }

  if (getSystemName(systemName, sizeof(systemName))) {
    printf("Unable to get systemName");
    return 1;
  }

  if (getCurrentWorkingDirectory(currentWorkingDir, sizeof currentWorkingDir)) {
    printf("Unable to get CWD");
    return 1;
  }

  printf("UserName: %s\n", userName);
  printf("SystemName: %s\n", systemName);
  printf("currentWorkingDir: %s\n", currentWorkingDir);

  // systemName = getSystemName();
  // currentWorkingDir = getCurrentWorkingDirectory();

  return 0;
}
