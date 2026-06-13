#include "http.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <unistd.h>
#include <dirent.h>

#include "da.h"
#include "sb.h"

http_t parse(sv_t* req) {
  sv_t method = svnextw(req);
  sv_t route = svnextw(req);
  sv_t path = svtok(&route, '?');

  http_t ret;

  const sv_t GET = svlit("GET");
  switch (svcmp(&method, &GET)) {
  case 0:
    ret.method = HTTP_GET;
    break;
  default:
    ret.method = HTTP_IDK;
    break;
  }

  svtok(&path, '/');
  if (svlen(&path) == 0) path = svlit(".");
  ret.path = path;

  return ret;
}

int send_200_file(int conn, const sv_t* filename) {
  const char fmt[] =
    "HTTP/1.1 200 OK\n"
    "Content-Type: text/plain, charset=utf-8\n"
    "Content-Length: %lu\n"
    "\n"
    ;

  char* filename_str = svdup(filename);
  FILE* file = fopen(filename_str, "r");
  free(filename_str);

  fseek(file, 0, SEEK_END);
  usize_t file_sz = ftell(file);
  fseek(file, 0, SEEK_SET);

  char* buff = malloc(file_sz);
  usize_t read = fread(buff, 1, file_sz, file);
  sv_t content = svn(buff, read);

  char* head = malloc(256);
  usize_t head_sz = snprintf(head, 256, fmt, svlen(&content));
  sv_t head_sv = svn(head, head_sz);

  sb_t sb = sb0();
  sbpush(&sb, &head_sv);
  sbpush(&sb, &content);

  usize_t res_sz = sblen(&sb);
  char* response = sbdup(&sb);

  free(head);
  free(buff);

  int n = send(conn, response, res_sz, 0);

  free(response);
  sbfree(&sb);
  close(conn);

  return n;
}

int send_200_dir(int conn, const sv_t* dir) {
  sb_t sb_html = sb0();

  const char html_head_fmt[] =
    "<!DOCTYPE HTML>\n"
    "<html lang=\"en\">\n"
    "<head>\n"
    "<title>Erwin | A Simple C Server</title>\n"
    "</head>\n"
    "<body>\n"
    "<h1>Lising "SV_FMT"</h1>\n"
    "<hr>\n"
    "<ul>\n"
    ;

  char* html_head = malloc(256);
  int html_head_sz = snprintf(html_head, 256, html_head_fmt, SV_ARG(*dir));
  sv_t head_sv = svn(html_head, html_head_sz);
  sbpush(&sb_html, &head_sv);

  const char li_fmt[] = "<li><a href=\""SV_FMT"/"SV_FMT"\">"SV_FMT"</a></li>\n";
  char* dir_path = svdup(dir);
  DIR* dirent = opendir(dir_path);
  free(dir_path);

  char** strings = NULL;

  struct dirent* ent = NULL;
  while ((ent = readdir(dirent)) != NULL) {
    int li_sz = strlen(ent->d_name) + 64;
    sv_t li_sv = svn(ent->d_name, li_sz);
    char* li_buf = malloc(li_sz);
    usize_t sz = snprintf(
        li_buf,
        li_sz,
        li_fmt,
        SV_ARG(*dir),
        SV_ARG(li_sv),
        SV_ARG(li_sv)
    );
    dapush(strings, li_buf);

    sv_t li = svn(li_buf, sz);
    sbpush(&sb_html, &li);
  }

  const char html_tail[] =
    "</ul>\n"
    "<hr>\n"
    "</body>\n"
    "</html>\n"
    ;
  sv_t tail_sv = svlit(html_tail);
  sbpush(&sb_html, &tail_sv);

  char* html = sbdup(&sb_html);
  sbfree(&sb_html);
  free(html_head);
  dafreedtor(strings, &free);
  usize_t html_sz = strlen(html);
  sv_t html_sv = svn(html, html_sz);

  const char http_head_fmt[] =
    "HTTP/1.1 200 OK\n"
    "Content-Type: text/html, charset=utf-8\n"
    "Content-Length: %lu\n"
    "\n"
    ;

  char* http = malloc(128);
  int http_sz = snprintf(http, 128, http_head_fmt, html_sz);
  sv_t http_sv = svn(http, http_sz);

  sb_t sb = sb0();
  sbpush(&sb, &http_sv);
  sbpush(&sb, &html_sv);
  char* payload = sbdup(&sb);
  sbfree(&sb);
  free(http);
  free(html);
  usize_t sz = strlen(payload);

  int n = send(conn, payload, sz, 0);
  free(payload);
  close(conn);

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
