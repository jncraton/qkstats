#include "stdio.h"

int main() {
  char buf[65535];

  FILE* fp = open("src/main.c",O_RDONLY);
  size_t len = read(fp, buf, 65535);
  write(stdout, buf, len);
}
