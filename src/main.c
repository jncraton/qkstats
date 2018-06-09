#include "ulibc.h"

long nth_int(char* filename, int n) {
  char content[255];

  FILE* fp = open(filename, O_RDONLY);
  read(fp, content, 255);
  close(fp);

  char* tok = strtok(content,"\t ");
  while (tok) {
    if (n == 0) { return atol(tok); }
    n--;
    tok = strtok(0x00,"\t ");
  }

  return -1;
}

int main() {
  sleep(1);
  
  char buf[16];
  sprintf(buf, "I/O ticks: %d\n", (int)nth_int("/sys/block/sda/stat", 9));
  write(stdout, buf, strlen(buf));
}
