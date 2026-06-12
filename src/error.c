#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void handle_errno(int code) {
  fprintf(stderr, "errno(%d): %s\n", errno, strerror(errno));
  exit(code);
}
