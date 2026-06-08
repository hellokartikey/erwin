#ifndef ERROR_H
#define ERROR_H

#include <errno.h>
#include <stdnoreturn.h>

typedef enum {
  ERR_OK = 0,
  ERR_UNKNOWN = 0xff,
} hk_error_t;

noreturn void handle_errno(int code);

#endif
