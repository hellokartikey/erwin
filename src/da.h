#ifndef DA_H
#define DA_H

#include <stdlib.h>

#include "common.h"

typedef struct {
  usize_t cap;
  usize_t size;
} da_t;

#define da(d)                                                                  \
do {                                                                           \
  if ((d) == NULL) {                                                           \
    da_t* head = (da_t*) malloc(sizeof(da_t));                                 \
    head->cap = 0;                                                             \
    head->size = 0;                                                            \
    (d) = (void*) (head + 1);                                                  \
  }                                                                            \
} while(0)

#define dahead(da) ((da_t*) (da) - 1)

#define dalen(da) dahead((da))->size

#define dacap(da) dahead((da))->cap

#define dapush(d, item)                                                        \
do {                                                                           \
  if ((d) == NULL) da((d));                                                    \
  da_t* head = dahead((d));                                                    \
  if (head->size == head->cap) {                                               \
    if (head->cap == 0) head->cap = 1;                                         \
    else head->cap *= 2;                                                       \
    head = realloc(head, sizeof(da_t) + (head->cap * sizeof(item)));           \
    (d) = (void*) (head + 1);                                                  \
  }                                                                            \
  (d)[head->size++] = (item);                                                  \
} while(0)

#define dafree(da)                                                             \
do {                                                                           \
  free(dahead((da)));                                                          \
  (da) = NULL;                                                                 \
} while(0)

#define dafreedtor(da, dtor)                                                   \
do {                                                                           \
  for (size_t i = 0; i < dalen((da)); i++) (dtor)((da)[i]);                    \
  dafree((da));                                                                \
} while (0)

#endif
