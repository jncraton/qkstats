#include "stdio.h"

int main() {
  char buf[14];

  read(stdin, buf, 14);
  write(stdout, buf, 14);
}

#include "bootstrap.h"
