#include "ls.h"
#include "parser.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int lsHandler(Command *cmd) {

  struct stat statbuf;
  DIR *dir;
  struct dirent *dp;

  char *path;
  int flags[2] = {0, 0}; // [a-flag, l-flag]
  // handle ls
  if (cmd->argc == 1) {
    path = ".";
  } else if (cmd->argc == 2) { // handle ls <filePath>
    path = cmd->argv[1];
  } else {
    // handle flags here
  }

  if ((dir = opendir(path)) == NULL) {
    fprintf(stderr, "ls: cannot open %s directory\n", path);
    return EXIT_FAILURE;
  }

  while ((dp = readdir(dir)) != NULL) {
    if (dp->d_name[0] == '.') {
      continue;
    }

    printf("%s\n", dp->d_name);
  }

  return 0;
}
