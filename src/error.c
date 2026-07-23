#include "error.h"

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

void handle_errno(int code) {
  fprintf(stderr, "errno(%d): %s\n", errno, strerror(errno));
  raise(SIGINT);
  while (1);
}
