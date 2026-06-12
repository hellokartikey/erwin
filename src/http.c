#include "http.h"

#include <stdlib.h>
#include <stdio.h>

#include <sys/socket.h>
#include <unistd.h>

int send_200_file(int conn, const sv_t* content) {
  char fmt[] =
    "HTTP/1.1 200 OK\n"
    "Content-Type: text/plain, charset=utf-8\n"
    "Content-Length: %d\n"
    "\n"
    SV_FMT
    "\n";

  usize_t sz = svlen(content) + 128;
  char* response = malloc(sz);
  usize_t res_sz = snprintf(response, sz, fmt, svlen(content), SV_ARG(*content));

  int n = send(conn, response, res_sz, 0);
  close(conn);
  free(response);
  return n;
}

int send_404(int conn) {
  const char head[] =
    "HTTP/1.1 404 Not Found\n"
    "Content-Length: 0\n";

  int n = send(conn, head, sizeof(head), 0);
  close(conn);
  return n;
}

int send_405(int conn) {
  const char head[] =
    "HTTP/1.1 405 Method Not Allowed\n"
    "Content-Length: 0\n"
    "Allow: GET\n";

  int n = send(conn, head, sizeof(head), 0);
  close(conn);
  return n;
}
