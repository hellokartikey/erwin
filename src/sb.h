#ifndef SB_H
#define SB_H

#include "sv.h"

typedef struct {
  sv_t* list;
} sb_t;

sb_t sb0();
void sbfree(sb_t* sb);

void sbpush(sb_t* sb, const sv_t* sv);
char* sbdup(const sb_t* sb);

usize_t sbcnt(const sb_t* sb);
usize_t sblen(const sb_t* sb);

#endif
