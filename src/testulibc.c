#include "ulibc.h"

int main() {
  assert(strlen("")==0);
  assert(strlen("1234")==4);

  char buf[16] = "   a b, c";
  
  assert(strchr(buf,'d')==0);
  assert(strchr(buf,' ')==buf);
  assert(strchr(buf,'a')==buf+3);
  assert(strchr(buf,'b')==buf+5);

  assert(*strtok(buf, " ") == 'a');
  assert(*strtok(0x00, " ") == 'b');
  assert(*strtok(0x00, ", ") == 'c');

  assert(atol("1") == 1);
  assert(atol("10") == 10);
  assert(atol("38592") == 38592);
  assert(atol("1285012080123") == 1285012080123);
  assert(atol("-7245234512") == -7245234512);
  assert(atol(" 20") == 20);
  assert(atol(" 55 ") == 55);

  write(stdout,"All tests passed.\n",18);
}
