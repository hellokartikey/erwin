#include "server.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>

#include <dirent.h>
#include <netdb.h>
#include <unistd.h>

#include <sys/stat.h>

#include "net.h"

route_t type(const sv_t* route) {
  struct stat info;

  char* name = svdup(route);
  if (stat(name, &info)) return TYPE_IDK;
  free(name);

  if (S_ISREG(info.st_mode)) { return TYPE_REG; }
  if (S_ISDIR(info.st_mode)) { return TYPE_DIR; }

  return TYPE_IDK;
}

void handler(int signal) {
  printf("[info] shutting down...\n");
  clean_net();
  exit(0);
}

void server() {
  signal(SIGINT, &handler);
  setup_net(PORT);
  while (true) accept_conn();
}
