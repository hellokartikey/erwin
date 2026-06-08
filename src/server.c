#include "server.h"

#include <stdio.h>
#include <string.h>

#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "da.h"
#include "error.h"

server_t server_state;

void scan_pages() {
  DIR* fd = opendir(".");
  if (fd == NULL) handle_errno(1);

  struct dirent* file;
  while ( (file = readdir(fd)) ) {
    struct stat info;
    if (stat(file->d_name, &info)) handle_errno(1);

    if (S_ISREG(info.st_mode)) {
      da_push(server_state.pages, strdup(file->d_name));
    }

    if (S_ISDIR(info.st_mode)) {
      da_push(server_state.routes, strdup(file->d_name));
    }
  }

  for (size_t i = 0; i < da_size(server_state.routes); i++) {
    printf("[info] scanned route %s/\n", server_state.routes[i]);
  }

  for (size_t i = 0; i < da_size(server_state.pages); i++) {
    printf("[info] scanned file %s\n", server_state.pages[i]);
  }

  closedir(fd);
}

void server(const char* host, port_t port) {
  scan_pages();

  da_free_destroy(server_state.pages, &free);
  da_free_destroy(server_state.routes, &free);
}
