#include "ls.h"
#include "parser.h"

void getPermissions(mode_t mode, char *permissions) {

  // File type
  if (S_ISDIR(mode))
    permissions[0] = 'd';
  else if (S_ISLNK(mode))
    permissions[0] = 'l';
  else if (S_ISCHR(mode))
    permissions[0] = 'c';
  else if (S_ISBLK(mode))
    permissions[0] = 'b';
  else if (S_ISFIFO(mode))
    permissions[0] = 'p';
  else if (S_ISSOCK(mode))
    permissions[0] = 's';
  else
    permissions[0] = '-';

  // Owner permissions
  permissions[1] = (mode & S_IRUSR) ? 'r' : '-';
  permissions[2] = (mode & S_IWUSR) ? 'w' : '-';
  permissions[3] = (mode & S_IXUSR) ? 'x' : '-';

  // Group permissions
  permissions[4] = (mode & S_IRGRP) ? 'r' : '-';
  permissions[5] = (mode & S_IWGRP) ? 'w' : '-';
  permissions[6] = (mode & S_IXGRP) ? 'x' : '-';

  // Others permissions
  permissions[7] = (mode & S_IROTH) ? 'r' : '-';
  permissions[8] = (mode & S_IWOTH) ? 'w' : '-';
  permissions[9] = (mode & S_IXOTH) ? 'x' : '-';

  permissions[10] = '\0';
}

void printLongFormat(char *dName, struct stat *statbuf) {

  char permissions[11];
  char timeBuffer[64];

  getPermissions(statbuf->st_mode, permissions);

  struct passwd *pw = getpwuid(statbuf->st_uid);
  struct group *gr = getgrgid(statbuf->st_gid);

  char *owner = pw ? pw->pw_name : "?";
  char *group = gr ? gr->gr_name : "?";

  struct tm *timeInfo = localtime(&statbuf->st_mtime);

  if (timeInfo == NULL ||
      strftime(timeBuffer, sizeof(timeBuffer), "%b %e %H:%M", timeInfo) == 0) {
    snprintf(timeBuffer, sizeof(timeBuffer), "unknown");
  }

  printf("%s %3llu %-8s %-8s %8lld %s %s\n", permissions,
         (unsigned long long)statbuf->st_nlink, owner, group,
         (long long)statbuf->st_size, timeBuffer, dName);
}

int lsHandler(Command *cmd) {

  struct stat statbuf;
  DIR *dir;
  struct dirent *dp;

  char *path = ".";
  int flags[2] = {0, 0}; // [a-flag, l-flag]

  if (cmd->argc > 4) {
    fprintf(stderr, "ls: Too many arguments\n");
    return EXIT_FAILURE;
  }

  // Process arguments
  for (int i = 1; i < cmd->argc; i++) {

    if (cmd->argv[i][0] == '-') {

      char *ptr = cmd->argv[i] + 1;

      while (*ptr != '\0') {

        if (*ptr == 'a')
          flags[0] = 1;
        else if (*ptr == 'l')
          flags[1] = 1;
        else {
          fprintf(stderr, "ls: illegal option -- %c\n", *ptr);
          return EXIT_FAILURE;
        }

        ptr++;
      }
    } else {
      path = cmd->argv[i];
    }
  }

  dir = opendir(path);

  if (dir == NULL) {
    perror("ls: opendir");
    return EXIT_FAILURE;
  }

  while ((dp = readdir(dir)) != NULL) {

    if (!flags[0] && dp->d_name[0] == '.') {
      continue;
    }

    if (fstatat(dirfd(dir), dp->d_name, &statbuf, AT_SYMLINK_NOFOLLOW) == -1) {
      perror(dp->d_name);
      continue;
    }

    if (flags[1]) {
      printLongFormat(dp->d_name, &statbuf);
    } else {
      printf("%s\n", dp->d_name);
    }
  }

  closedir(dir);

  return EXIT_SUCCESS;
}
