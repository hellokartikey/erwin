#ifndef SERVER_H
#define SERVER_H

#include <stdnoreturn.h>

#define PORT 8080

typedef struct {
  char** pages;
  char** routes;
} server_t;

void setup_state();
void clean_state();

void scan_pages();

noreturn void server();

void handle_SIGINT(int signal);

#endif
