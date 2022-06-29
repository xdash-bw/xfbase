#ifndef XF_LIST_H_
#define XF_LIST_H_

#include <stdbool.h>
#include <stddef.h>

typedef struct xf_list_node {
  struct xf_list_node *prev;
  struct xf_list_node *next;
} xf_list_node_t;

#define XF_LIST_HEAD_INIT(name) \
  {                             \
    &(name), &(name)            \
  }
#define XF_LIST_HEAD(head) xf_list_node_t head = XF_LIST_HEAD_INIT(head)

#define CONTAINER_OF(ptr, type, member) \
  ((type *)(((char *)((type *)(ptr))) - offsetof(type, member)))
#define XF_LIST_ENTRY(ptr, type, member) CONTAINER_OF(ptr, type, member)

static inline void xf_list_head_init(xf_list_node_t *node)
{
  node->prev = node;
  node->next = node;
}

static inline bool xf_list_is_empty(xf_list_node_t *head)
{
  return head->next == head;
}

static inline void __xf_list_add(xf_list_node_t *node, xf_list_node_t *prev,
                                 xf_list_node_t *next)
{
  node->prev = prev;
  prev->next = node;
  next->prev = node;
  node->next = next;
}

static inline void xf_list_add(xf_list_node_t *head, xf_list_node_t *node)
{
  __xf_list_add(node, head, head->next);
}

static inline void xf_list_add_tail(xf_list_node_t *head, xf_list_node_t *node)
{
  __xf_list_add(node, head->prev, head);
}

static inline void __xf_list_del(xf_list_node_t *prev, xf_list_node_t *next)
{
  prev->next = next;
  next->prev = prev;
}

static inline void xf_list_del(xf_list_node_t *node)
{
  __xf_list_del(node->prev, node->next);
  node->prev = node->next = 0;
}

#endif  // LIST_H_
