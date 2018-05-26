#include "ulibc.h"

int main() {
  char buf[65535];

  FILE* fp = open("src/main.c",O_RDONLY);
  size_t len = read(fp, buf, 65535);
  close(fp);
  write(stdout, buf, len);
}
