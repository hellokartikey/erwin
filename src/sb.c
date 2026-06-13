#include "sb.h"

#include <stdlib.h>
#include <string.h>

#include "da.h"
#include "sv.h"

sb_t sb0() {
  return (sb_t) { .list = NULL };
}

void sbfree(sb_t* sb) {
  dafree(sb->list);
}

void sbpush(sb_t* sb, const sv_t* sv) {
  sv_t copy = *sv;
  dapush(sb->list, copy);
}

char* sbdup(const sb_t* sb) {
  usize_t buff_sz = sblen(sb);
  char* buff = malloc(buff_sz + 1);
  char* iter = buff;

  for (int i = 0; i < sbcnt(sb); ++i) {
    sv_t sv = sb->list[i];
    iter = stpncpy(iter, svdata(&sv), svlen(&sv));
  }

  buff[buff_sz] = 0;
  return buff;
}

usize_t sbcnt(const sb_t* sb) {
  return dalen(sb->list);
}

usize_t sblen(const sb_t* sb) {
  usize_t len = 0;

  for (int i = 0; i < sbcnt(sb); i++) {
    len += svlen(&sb->list[i]);
  }

  return len;
}
