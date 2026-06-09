#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>
#include <stdbool.h>
#include <stdnoreturn.h>

#include "common.h"

typedef struct {
  char** pages;
  char** routes;
  int fd;
} server_t;

void init_state();
void clean_state();

noreturn void server(const char* host, const char* port);

void scan_pages();
void setup_socket(const char* host, const char* port);

void handle_SIGINT(int signal);
void handle_connection(int conn);

#endif
