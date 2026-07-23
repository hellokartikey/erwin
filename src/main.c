#include <stdlib.h>
#include <stdio.h>

#include "server.h"

int main(int argc, char** argv) {
  port_t port = 8000;

  if (argc == 2) {
    port = atoi(argv[1]);
  } else if (argc > 2) {
    fprintf(stderr, "Usage:\n\t%s [PORT=%u]\n", argv[0], port);
    exit(2);
  }

  server(port);
  return 0;
}
