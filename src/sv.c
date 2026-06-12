#include "sv.h"

#include <string.h>
#include <ctype.h>

sv_t svn(const char* string, usize_t size) {
  return (sv_t) { .string = string, .size = size };
}

sv_t svlit(const char* literal) {
  return svn(literal, strlen(literal));
}

char* svdup(const sv_t* sv) {
  return strndup(svdata(sv), svlen(sv));
}

const char* svdata(const sv_t* sv) {
  return sv->string;
}

usize_t svlen(const sv_t* sv) {
  return sv->size;
}

sv_t svtok(sv_t* sv, char c) {
  usize_t cnt = 0;
  sv_foreach(iter, sv) {
    cnt++;
    if (*iter == c) break;
  }

  sv_t tok = *sv;
  svrtrim(&tok, svlen(&tok) - cnt);

  svltrim(sv, cnt);

  return tok;
}

sv_t svnextw(sv_t* sv) {
  svlstrip(sv);

  usize_t cnt = 0;
  sv_foreach(iter, sv) {
    if (isspace(*iter)) break;
    cnt++;
  }

  sv_t word = *sv;
  svrtrim(&word, svlen(&word) - cnt);

  svltrim(sv, cnt);
  svlstrip(sv);

  return word;
}

usize_t svrtrim(sv_t* sv, usize_t cnt) {
  if (svlen(sv) < cnt) cnt = svlen(sv);
  sv->size -= cnt;
  return cnt;
}

usize_t svltrim(sv_t* sv, usize_t cnt) {
  if (svlen(sv) < cnt) cnt = svlen(sv);
  sv->string += cnt;
  sv->size -= cnt;
  return cnt;
}

usize_t svlstrip(sv_t* sv) {
  usize_t cnt = 0;
  sv_foreach(iter, sv) {
    if (! isspace(*iter)) break;
    cnt++;
  }

  return svltrim(sv, cnt);
}

usize_t svrstrip(sv_t* sv) {
  usize_t cnt = 0;
  sv_foreachrev(iter, sv) {
    if (! isspace(*iter)) break;
    cnt++;
  }

  return svrtrim(sv, cnt);
}

usize_t svstrip(sv_t* sv) {
  return svlstrip(sv) + svrstrip(sv);
}

int svcmp(const sv_t* sv1, const sv_t* sv2) {
  usize_t len1 = svlen(sv1);
  usize_t len2 = svlen(sv2);

  usize_t min = len1 < len2 ? len1 : len2;
  return strncmp(svdata(sv1), svdata(sv2), min);
}
