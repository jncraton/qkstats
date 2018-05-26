#include "stdio.h"

int main() {
  write((void*)1, "Hello, World!\n", 14);
}

#include "bootstrap.h"
