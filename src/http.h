#ifndef HTTP_H
#define HTTP_H

#include "sv.h"

typedef enum {
  HTTP_GET,
  HTTP_IDK
} method_t;

typedef struct {
  method_t method;
  sv_t path;
} http_t;

http_t parse(sv_t* req);

int send_200(int conn);
int send_200_dir(int conn, const sv_t* dir);
int send_200_file(int conn, const sv_t* filename);

int send_404(int conn);
int send_405(int conn);

#endif
