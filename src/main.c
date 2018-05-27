#include "ulibc.h"

int main() {
  char buf[65535];

  FILE* fp = open("/sys/block/sda/stat",O_RDONLY);
  read(fp, buf, 65535);
  close(fp);

  int start = 0;
  int end;
  for (start=0;*(buf+start)==' ';start++);
  for (end=start;*(buf+end)!=' ';end++);

  size_t read_reqs = strtol(buf+start,buf+end-1,10);
  
  for (int i = 0; i<65535; i++) {
    buf[i] = 0;
  }
  
  sprintf(buf, "Read requests: %d\n", (int)read_reqs);
  write(stdout, buf, strlen(buf));
}
