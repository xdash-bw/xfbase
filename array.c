#include "array.h"

#include <assert.h>

#include "vector.h"

/* 当插入后，内部的 vector 可能会进行内存分配。而 void *item
 * 可能会失效，所以要进行更新 */
static void __xf_array_update(xf_array_t *a)
{
  a->items = xf_vec_obj_data(&a->vec, void *);
}

void xf_array_init(xf_array_t *a)
{
  xf_vector_init(&a->vec);
  a->size = 0;
  a->items = 0;
}

void xf_array_destory(xf_array_t *a)
{
  xf_vector_destory(&a->vec);
  a->size = 0;
  a->items = NULL;
}

void xf_array_push(xf_array_t *a, void *obj)
{
  if (xf_vec_obj_push(&a->vec, void *, &obj) != 0) { assert(-1); }
  __xf_array_update(a);
  a->size++;
}

void *xf_array_pop(xf_array_t *a)
{
  assert(a->size > 0);
  a->size--;
  void *item = a->items[a->size];

  int hr = xf_vec_obj_resize(&a->vec, void *, a->size);
  if (hr != 0) { assert(hr); }

  __xf_array_update(a);
  return item;
}
