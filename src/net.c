#include "net.h"

#include <stdio.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "error.h"

static net_t net;

void setup_net(uint16_t port) {
  int ec;

  printf("[info] starting server on %u...\n", port);
  ec = socket(AF_INET, SOCK_STREAM, 0);
  if (ec < 0) handle_errno(1);
  net.fd = ec;

  printf("[info] binding socket...\n");
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);
  ec = bind(net.fd, (struct sockaddr*) &addr, sizeof(addr));
  if (ec < 0) handle_errno(1);

  printf("[info] listening on socket %u...\n", port);
  ec = listen(net.fd, 10);
  if (ec < 0) handle_errno(1);
}

void clean_net() {
  printf("[info] closing socket %u...\n", net.port);
  close(net.fd);
}

void accept_conn() {
  struct sockaddr_in addr;
  socklen_t len;
  int conn = accept(net.fd, (struct sockaddr*) &addr, &len);

  handle_conn(conn);
}

void handle_conn(int conn) {
  const char msg[] =
    "HTTP/1.1 200 OK\n"
    "Content-Type: text/html, charset=utf-8\n"
    "Content-Length: 22\n"
    "\n"
    "<h1>Hello World!</h1>\n";

  int n = send(conn, msg, sizeof(msg), 0);
  printf("[info] sent %d bytes\n", n);

  close(conn);
}
