#include "page_stream.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

#define PAGE_SIZE 1024
#define NEW_CACHE_NUM 2
#define MAX_CACHE_NUM 8

void xf_page_stream_init(xf_page_stream_t *ps) {
  xf_list_head_init(&ps->used_head);
  xf_list_head_init(&ps->cache_head);
  ps->cache_num = 0;
  ps->pos_read = 0;
  ps->pos_write = 0;
}

static void xf_page_cache_release(xf_page_stream_t *ps, xf_page_t *page) {
  xf_list_add_tail(&ps->cache_head, &page->node);
  ps->cache_num++;

  xf_page_t *cur;
  if (ps->cache_num > MAX_CACHE_NUM) {
    cur = XF_LIST_ENTRY(&ps->cache_head, xf_page_t, node);
    xf_list_del(ps->cache_head.next);
    free(cur);
  }
}

size_t xf_page_stream_read(xf_page_stream_t *ps, void *ptr, size_t size) {
  unsigned char *lptr = (unsigned char *)ptr;
  size_t canread = 0;
  size_t total = 0;
  xf_page_t *current;

  for (; size > 0; size -= canread, total += canread) {
    if (xf_list_is_empty(&ps->used_head)) {
      break;
    }
    current = XF_LIST_ENTRY(&ps->used_head, xf_page_t, node);
    if (current->node.next == &ps->used_head) {
      canread = ps->pos_write - ps->pos_read;
    } else {
      canread = current->size - ps->pos_read;
    }

    canread = (canread < size) ? canread : size;
    memcpy(lptr, current->data + ps->pos_read, canread);

    lptr += canread;
    ps->pos_read += canread;

    if (ps->pos_read >= current->size) {
      xf_list_del(&current->node);
      xf_page_cache_release(ps, current);
    }
  }

  return total;
}

static xf_page_t *xf_page_cache_get(xf_page_stream_t *ps) {
  if (xf_list_is_empty(&ps->cache_head)) {
    for (int i = 0; i < NEW_CACHE_NUM; i++) {
      xf_page_t *newpage = (xf_page_t *)malloc(sizeof(xf_page_t) + PAGE_SIZE);
      xf_list_head_init(&newpage->node);
      newpage->size = PAGE_SIZE;
      xf_list_add_tail(&ps->cache_head, &newpage->node);
      ps->cache_num++;
    }
  }
  xf_page_t *current;
  current = XF_LIST_ENTRY(ps->cache_head.next, xf_page_t, node);
  xf_list_del(&current->node);
  ps->cache_num--;
  return current;
}

size_t xf_page_stream_write(xf_page_stream_t *ps, const void *ptr,
                            size_t size) {
  unsigned char *lptr = (unsigned char *)ptr;
  size_t canwrite = 0;
  size_t total = 0;
  xf_page_t *current = 0;

  for (; size > 0; size -= canwrite, total += canwrite) {
    if (xf_list_is_empty(&ps->used_head)) {
      canwrite = 0;
      current = NULL;
    } else {
      current = XF_LIST_ENTRY(ps->used_head.next, xf_page_t, node);
      canwrite = current->size - ps->pos_write;
    }
    if (canwrite == 0) {
      current = xf_page_cache_get(ps);
      assert(current);
      xf_list_add_tail(&ps->used_head, &current->node);
      canwrite = current->size;
      ps->pos_write = 0;
    }

    canwrite = (canwrite < size) ? canwrite : size;
    memcpy(current->data + ps->pos_write, lptr, canwrite);

    ps->pos_write += canwrite;
    lptr += canwrite;
  }

  return total;
}
