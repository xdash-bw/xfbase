#ifndef XF_VECTOR_H
#define XF_VECTOR_H

#include <stddef.h>

typedef struct xf_vector {
  unsigned char *data;
  size_t size;
  size_t capacity;
} xf_vector_t;

/*
  access methods
*/

/* 作为 unsigned char 缓存区操作数据 */
#define xf_vector_data(v) ((v)->data)
#define xf_vector_size(v) ((v)->size)
#define xf_vector_capacity(v) ((v)->capacity)

/* 传入 type 作为实际的对象操作数据 */
#define xf_vec_obj_data(v, type) ((type *)xf_vector_data(v))
#define xf_vec_obj_index(v, type, index) (xf_vec_obj_data(v, type)[index])
#define xf_vec_obj_size(v, type) ((v)->size / sizeof(type))
#define xf_vec_obj_capacity(v, type) ((v)->capacity / sizeof(type))

/*
  功能接口
 */

/* 像 STL 一样作为值语义使用，不建议动态分配，所以没有动态分配的接口 */
void xf_vector_init(xf_vector_t *v);
void xf_vector_destory(xf_vector_t *v);

/* 操作 buffer */
int xf_vector_push(xf_vector_t *v, const void *obj, size_t size);
size_t xf_vector_pop(xf_vector_t *v, void *obj, size_t size);
int xf_vector_resize(xf_vector_t *v, size_t newsize);
int xf_vector_reserve(xf_vector_t *v, size_t newcap);

/* 封装为操作 obj 对象的接口 */
#define xf_vec_obj_push(v, type, objptr) xf_vector_push(v, objptr, sizeof(type))
#define xf_vec_obj_pop(v, type, objptr) xf_vector_pop(v, objptr, sizeof(type))

#define xf_vec_obj_resize(v, type, newsize) \
  xf_vector_resize(v, sizeof(type) * newsize)

#define xf_vec_obj_reserve(v, type, newcap) \
  xf_vector_reserve(v, sizeof(type) * newcap)

#endif  // XF_VECTOR_H
