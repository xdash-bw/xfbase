#ifndef XF_RBTREE_H
#define XF_RBTREE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rb_node {
  struct rb_node *rb_parent;
  unsigned int rb_color;
#define RB_RED 0
#define RB_BLACK 1
  struct rb_node *rb_right;
  struct rb_node *rb_left;
} xf_rb_node_t;

typedef struct rb_root {
  struct rb_node *rb_node;
} xf_rb_root_t;

#define XF_RB_ROOT_INIT(root) \
  {                           \
    NULL,                     \
  }
#define XF_RB_ROOT(root) xf_root_t root = XF_RB_ROOT_INIT(root)
static inline void xf_rb_root_init(xf_rb_root_t *root) { root->rb_node = 0; }

#define CONTAINER_OF(ptr, type, member) \
  ((type *)(((char *)((type *)(ptr))) - offsetof(type, member)))
#define RB_ENTRY(ptr, type, member) CONTAINER_OF(ptr, type, member)

extern void __xf_rb_insert_color(xf_rb_node_t *, xf_rb_root_t *);
extern void xf_rb_erase(xf_rb_node_t *, xf_rb_root_t *);

static inline void __xf_rb_link_node(xf_rb_node_t *node, xf_rb_node_t *parent,
                                     xf_rb_node_t **rb_link)
{
  node->rb_parent = parent;
  node->rb_color = RB_RED;
  node->rb_left = node->rb_right = 0;

  *rb_link = node;
}

#define xf_rb_node_find(root, what, compare_fn, res_node) \
  do                                                      \
  {                                                       \
    struct rb_node *__n = (root)->rb_node;                \
    (res_node) = NULL;                                    \
    while (__n)                                           \
    {                                                     \
      int __hr = (compare_fn)(what, __n);                 \
      (res_node) = __n;                                   \
      if (__hr == 0)                                      \
      {                                                   \
        (res_node) = __n;                                 \
        break;                                            \
      } else if (__hr < 0)                                \
      {                                                   \
        __n = __n->rb_left;                               \
      } else                                              \
      {                                                   \
        __n = __n->rb_right;                              \
      }                                                   \
    }                                                     \
  } while (0)

#define xf_rb_node_add(root, newnode, compare_fn, duplicate_node) \
  do                                                              \
  {                                                               \
    struct rb_node **__link = &((root)->rb_node);                 \
    struct rb_node *__parent = NULL;                              \
    struct rb_node *__duplicate = NULL;                           \
    int __hr = 1;                                                 \
    while (__link[0])                                             \
    {                                                             \
      __parent = __link[0];                                       \
      __hr = (compare_fn)(newnode, __parent);                     \
      if (__hr == 0)                                              \
      {                                                           \
        __duplicate = __parent;                                   \
        break;                                                    \
      } else if (__hr < 0)                                        \
      {                                                           \
        __link = &(__link[0]->rb_left);                           \
      } else                                                      \
      {                                                           \
        __link = &(__link[0]->rb_right);                          \
      }                                                           \
    }                                                             \
    (duplicate_node) = __duplicate;                               \
    if (__duplicate == NULL)                                      \
    {                                                             \
      __xf_rb_link_node(newnode, __parent, __link);               \
      __xf_rb_insert_color(newnode, root);                        \
    }                                                             \
  } while (0)

/* Find logical next and previous nodes in a tree */
extern xf_rb_node_t *xf_rb_next(xf_rb_node_t *);
extern xf_rb_node_t *xf_rb_prev(xf_rb_node_t *);
extern xf_rb_node_t *xf_rb_first(xf_rb_root_t *);
extern xf_rb_node_t *xf_rb_last(xf_rb_root_t *);

#ifdef __cplusplus
}
#endif

#endif  // XF_RBTREE_H
