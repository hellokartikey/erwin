#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>

#include "common.h"

typedef struct {
  port_t port;
  char** pages;
  char** routes;
} server_t;

void scan_pages();

void server(const char* host, port_t port);

#endif
