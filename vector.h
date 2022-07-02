#ifndef XF_VECTOR_H
#define XF_VECTOR_H

#include <stddef.h>

typedef struct xf_vector{
  unsigned char *data;
  size_t size;
  size_t capacity;
} xf_vector_t;

#define xf_vector_data(v) ((v)->data)
#define xf_vector_size(v) ((v)->size)
#define xf_vector_capacity(v) ((v)->capacity)

#define xf_vector_entry(v, type) ((type*)xf_vector_data(v))

int xf_vector_push(xf_vector_t *v, const void *obj, size_t size);
size_t xf_vector_pop(xf_vector_t *v, void *obj, size_t size);

int xf_vector_resize(xf_vector_t *v, size_t newsize);

#endif  // XF_VECTOR_H
