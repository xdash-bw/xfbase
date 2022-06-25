#ifndef XF_PAGE_STREAM_H
#define XF_PAGE_STREAM_H

#include "list.h"

typedef struct xf_page_stream {
  xf_list_node_t used_head;
  xf_list_node_t cache_head;
  size_t cache_num;
  size_t pos_read;
  size_t pos_write;
} xf_page_stream_t;

typedef struct xf_page {
  xf_list_node_t node;
  size_t size;
  unsigned char data[2];
} xf_page_t;

extern void xf_page_stream_init(xf_page_stream_t *ps);
extern size_t xf_page_stream_read(xf_page_stream_t *ps, void *ptr, size_t size);
extern size_t xf_page_stream_write(xf_page_stream_t *ps, const void *ptr,
                                   size_t size);

#endif  // XF_PAGE_STREAM_H
