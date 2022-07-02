#include "vector.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int xf_vector_push(xf_vector_t *v, const void *obj, size_t size)
{
  if (xf_vector_resize(v, v->size + size) != 0) { return -1; }
  memcpy(v->data + v->size, obj, size);
  return 0;
}

static int __xf_vector_capacity(xf_vector_t *v, size_t newcap)
{
  unsigned char *ptr = (unsigned char *)malloc(sizeof(unsigned char) * newcap);
  if (ptr == NULL) { return -1; }

  size_t mincopy = (newcap < v->size) ? newcap : v->size;
  memcpy(ptr, v->data, mincopy);

  free(v->data);
  v->data = ptr;
  v->capacity = newcap;
  v->size = mincopy;
  return 0;
}

int xf_vector_resize(xf_vector_t *v, size_t newsize)
{
  if (newsize > v->capacity)
  {
    size_t capacity = 2 * v->capacity;
    if (capacity < newsize)
    {
      capacity = sizeof(char *);
      while (capacity < newsize) { capacity *= 2; }
    }
    if (__xf_vector_capacity(v, capacity) != 0) { return -1; }
  }
  v->size = newsize;
  return 0;
}

size_t xf_vector_pop(xf_vector_t *v, void *obj, size_t size)
{
  /* obj 可以是 NULL 的，目的是从 vector 中直接 pop 废弃。 */
  /* assert(obj); */
  size = (size > v->size) ? v->size : size;
  if (obj) { memcpy(obj, v->data + v->size - size, size); }
  v->size -= size;
  return size;
}
