#ifndef SV_H
#define SV_H

#include <stdbool.h>

#include "common.h"

#define SV_FMT "%.*s"
#define SV_ARG(sv) (int) (sv).size, (sv).string

#define sv_foreach(iter, sv)                                                   \
  for (                                                                        \
      const char* (iter) = (sv)->string;                                       \
      (iter) < (sv)->string + svlen((sv));                                     \
      (iter)++                                                                 \
  )

#define sv_foreachrev(iter, sv)                                                \
  for (                                                                        \
      const char* (iter) = (sv)->string + svlen((sv)) - 1;                     \
      (iter) >= (sv)->string;                                                  \
      (iter)--                                                                 \
  )

typedef struct {
  const char* string;
  usize_t size;
} sv_t;

sv_t svn(const char* string, usize_t size);
sv_t svlit(const char* literal);
char* svdup(const sv_t* sv);

const char* svdata(const sv_t* sv);
usize_t svlen(const sv_t* sv);

sv_t svtok(sv_t* sv, char c);
sv_t svnextw(sv_t* sv);

usize_t svrtrim(sv_t* sv, usize_t cnt);
usize_t svltrim(sv_t* sv, usize_t cnt);

usize_t svlstrip(sv_t* sv);
usize_t svrstrip(sv_t* sv);
usize_t svstrip(sv_t* sv);

int svcmp(const sv_t* sv1, const sv_t* sv2);

#endif
