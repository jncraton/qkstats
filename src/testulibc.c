#include "ulibc.h"

int main() {
  assert(strlen("")==0);
  assert(strlen("1234")==4);
  
  assert(strchr("aaa",(int)'b')==(char*)0x00);
  assert(strchr("a b c",' ')==(char*)&("a b c")+1);
  assert(strchr(" ",'a')==0x00);
  assert(strchr("ab",'b')==(char*)&("ab")+1);
  assert(strchr("aaaba",'b')==(char*)&("aaaba")+3);

  char buf[16] = "   a b, c";

  assert(*strtok(buf, " ") == 'a');
  assert(*strtok(0x00, " ") == 'b');
  assert(*strtok(0x00, ", ") == 'c');

  assert(atol("1") == 1);
  assert(atol("10") == 10);
  assert(atol("38592") == 38592);
  assert(atol("1285012080123") == 1285012080123);
  assert(atol("-7245234512") == -7245234512);

  write(stdout,"All tests passed.\n",18);
}
