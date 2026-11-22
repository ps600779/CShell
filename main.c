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

int main() {
  char userName[256];
  char systemName[256];
  char *currentWorkingDir;

  if (getUserName(userName, sizeof(userName))) {
    printf("Unable to get username");
    return 1;
  }

  if (getSystemName(systemName, sizeof(systemName))) {
    printf("Unable to get systemName");
    return 1;
  }

  printf("%s\n", userName);
  printf("%s\n", systemName);

  // systemName = getSystemName();
  // currentWorkingDir = getCurrentWorkingDirectory();

  return 0;
}
