#include <stdio.h>

#include "server.h"

#define PORT 8000
#define HOST "localhost"

int main()
{
  printf("[info] starting server on port %d...\n", PORT);
  server(HOST, PORT);

  return 0;
}
