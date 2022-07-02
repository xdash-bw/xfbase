#ifndef XF_ARRAY_H
#define XF_ARRAY_H

#include "vector.h"

typedef struct xf_array {
  xf_vector_t vec;
  void **items;
  size_t size;
} xf_array_t;

/* =========================================================
  access methods
============================================================*/

#define xf_array_data(a) ((a)->items)

#define xf_array_index(a, index) \
  do {                           \
    assert((index) < (a)->size); \
    ((a)->item[(index)]);        \
  } while (0)

#define xf_array_size(a) ((a)->size)

/* ===========================================================
  function interface
============================================================== */

/* 像 STL 一样作为值语义使用，不建议动态分配，所以没有动态分配的接口 */
void xf_array_init(xf_array_t *a);
void xf_array_destory(xf_array_t *a);

void xf_array_push(xf_array_t *a, void *obj);
void* xf_array_pop(xf_array_t *a);

#endif  // XF_ARRAY_H
