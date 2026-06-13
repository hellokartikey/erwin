#include "net.h"

#include <stdio.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "error.h"
#include "sv.h"
#include "server.h"
#include "http.h"

static net_t net;

void setup_net(uint16_t port) {
  int ec;

  net.port = port;

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

  printf("[info] listening on socket %u...\n", net.port);
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

  int n = handle_conn(conn);
  printf("[info] sent %d bytes\n", n);
}

int handle_conn(int conn) {
  char reqbuff[1024];
  int n = recv(conn, reqbuff, 1024, 0);
  sv_t sv = svn(reqbuff, n);

  http_t req = parse(&sv);
  const char* METHOD = req.method == HTTP_GET ? "GET" : "IDK";
  printf("[info] %s "SV_FMT"\n", METHOD, SV_ARG(req.path));
  if (req.method != HTTP_GET) return send_405(conn);

  switch (type(&req.path)) {
  case TYPE_REG:
    return send_200_file(conn, &req.path);
  case TYPE_DIR:
    return send_200_dir(conn, &req.path);
  default:
    return send_404(conn);
  }
}
