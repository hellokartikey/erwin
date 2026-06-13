#ifndef SERVER_H
#define SERVER_H

#include <stdnoreturn.h>

#include "sv.h"

#define PORT 6971

typedef enum {
  TYPE_REG,
  TYPE_DIR,
  TYPE_IDK
} route_t;

route_t type(const sv_t* route);
noreturn void handler(int signal);
noreturn void server();

#endif
