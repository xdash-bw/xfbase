#include <stddef.h>
#include <stdio.h>

#include "../page_stream.h"

int main() {
  char fuckyou[] = "fuckyouskljdflskjfldjskdjf";
  xf_page_stream_t ps;
  xf_page_stream_init(&ps);

  xf_page_stream_write(&ps, fuckyou, sizeof(fuckyou));

  char fuck[100];
  size_t n = xf_page_stream_read(&ps, fuck, 1000);
  printf("%s \nnum:%d", fuck, n);
}
