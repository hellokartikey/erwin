#include "server.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>

#include <dirent.h>
#include <netdb.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include "da.h"
#include "error.h"

server_t state;

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

void init_state() {
  state.pages = NULL;
  state.routes = NULL;
  state.fd = -1;

  signal(SIGINT, &handle_SIGINT);
}

void clean_state() {
  printf("[info] shutting down...\n");

  close(state.fd);

  da_free_destroy(state.pages, &free);
  da_free_destroy(state.routes, &free);
}

void setup_socket(const char* host, const char* port) {
  struct addrinfo* addr;
  int ec = getaddrinfo(host, port, NULL, &addr);
  if (ec) {
    fprintf(stderr, "getaddrinfo(%d): %s\n", ec, gai_strerror(ec));
    exit(1);
  }

  printf("[info] starting socket %s\n", port);
  state.fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
  if (state.fd < 0) handle_errno(1);

  printf("[info] binding socket...\n");
  ec = bind(state.fd, addr->ai_addr, addr->ai_addrlen);
  if (ec < 0) handle_errno(1);

  printf("[info] listening...\n");
  listen(state.fd, 10);

  freeaddrinfo(addr);
}

void handle_connection(int conn) {
    const char msg[] =
      "HTTP/1.1 200 OK\n"
      "Content-Type: text/html, charset=utf-8\n"
      "Content-Length: 22\n"
      "\n"
      "<h1>Hello World!</h1>\n";

    int n = send(conn, msg, sizeof(msg), 0);
    printf("[info] sent %d bytes\n", n);

    close(conn);
}

void server(const char* host, const char* port) {
  init_state();

  scan_pages();
  setup_socket(host, port);

  while (true) {
    struct sockaddr_in addr;
    socklen_t addrlen;
    int conn = accept(state.fd, (struct sockaddr*) &addr, &addrlen);
    handle_connection(conn);
  }
}
