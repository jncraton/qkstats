#include "stdio.h"

int main() {
  char buf[1024];

  size_t len = read(stdin, buf, 1024);
  write(stdout, buf, len);
}

#include "bootstrap.h"
