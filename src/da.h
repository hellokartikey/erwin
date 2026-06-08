#ifndef DA_H
#define DA_H

#include <stdlib.h>

#include "common.h"

typedef struct {
  usize_t cap;
  usize_t size;
} da_head_t;

#define da_init(da)                                                            \
do {                                                                           \
  if ((da) == NULL) {                                                          \
    da_head_t* head = (da_head_t*) malloc(sizeof(da_head_t));                  \
    head->cap = 0;                                                             \
    head->size = 0;                                                            \
    (da) = (void*) (head + 1);                                                 \
  }                                                                            \
} while(0)

#define da_head(da) ((da_head_t*) (da) - 1)

#define da_size(da) da_head((da))->size

#define da_capacity(da) da_head((da))->cap

#define da_push(da, item)                                                      \
do {                                                                           \
  if ((da) == NULL) da_init((da));                                             \
  da_head_t* head = da_head((da));                                             \
  if (head->size == head->cap) {                                               \
    if (head->cap == 0) head->cap = 1;                                         \
    else head->cap *= 2;                                                       \
    head = realloc(head, sizeof(da_head_t) + (head->cap * sizeof(item)));      \
    (da) = (void*) (head + 1);                                                 \
  }                                                                            \
  (da)[head->size++] = (item);                                                 \
} while(0)

#define da_free(da) free(da_head((da)))

#define da_free_destroy(da, dtor)                                              \
do {                                                                           \
  for (size_t i = 0; i < da_size((da)); i++) (dtor)((da)[i]);                  \
  da_free((da));                                                               \
} while (0)

#endif
