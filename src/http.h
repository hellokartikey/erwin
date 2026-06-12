#ifndef HTTP_H
#define HTTP_H

#include "sv.h"

int send_200(int conn);
int send_200_file(int conn, const sv_t* content);

int send_404(int conn);
int send_405(int conn);

#endif
