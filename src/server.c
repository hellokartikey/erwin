#include "server.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>

#include <dirent.h>
#include <netdb.h>
#include <unistd.h>

#include <sys/stat.h>

#include "da.h"
#include "error.h"
#include "net.h"

static server_t state;

void scan_pages() {
  DIR* fd = opendir(".");
  if (fd == NULL) handle_errno(1);

  struct dirent* file;
  while ( (file = readdir(fd)) ) {
    if (file->d_name[0] == '.') continue;

    struct stat info;
    if (stat(file->d_name, &info)) handle_errno(1);

    if (S_ISREG(info.st_mode)) { da_push(state.pages, strdup(file->d_name)); }
    if (S_ISDIR(info.st_mode)) { da_push(state.routes, strdup(file->d_name)); }
  }

  for (size_t i = 0; i < da_size(state.routes); i++) {
    printf("[info] scanned route %s/\n", state.routes[i]);
  }

  for (size_t i = 0; i < da_size(state.pages); i++) {
    printf("[info] scanned file %s\n", state.pages[i]);
  }

  closedir(fd);
}

void handle_SIGINT(int signal) {
  clean_state();
  exit(0);
}

void setup_state() {
  state.pages = NULL;
  state.routes = NULL;

  signal(SIGINT, &handle_SIGINT);

  scan_pages();

  setup_net(PORT);
}

void clean_state() {
  printf("[info] shutting down...\n");

  da_free_destroy(state.pages, &free);
  da_free_destroy(state.routes, &free);

  clean_net();
}

void server() {
  setup_state();
  while (true) accept_conn();
}
