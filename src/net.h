#ifndef NET_H
#define NET_H

#include <stdint.h>

typedef struct {
  int fd;
  uint16_t port;
} net_t;

void setup_net(uint16_t port);
void clean_net();

void accept_conn();
int handle_conn(int conn);

#endif
