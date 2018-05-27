#include "ulibc.h"

int main() {
  assert(strlen("")==0);
  assert(strlen("1234")==4);
  
  assert(strchr("aaa",'b')==0x00);
  assert(strchr("a b c",' ')==(char*)&("a b c")+1);
  assert(strchr(" ",'a')==0x00);
  assert(strchr("ab",'b')==(char*)&("ab")+1);
  assert(strchr("aaaba",'b')==(char*)&("aaaba")+3);

  assert(*strtok("a b c", " ") == 'a');
  //assert(strtok(0x00, " ") != 0x00);
  //assert(*strtok(0x00, " ") == 'c');

  write(stdout,"All tests passed.\n",18);
}
