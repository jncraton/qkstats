#include "ulibc.h"

int main() {
  char blockdev[255];

  FILE* fp = open("/sys/block/sda/stat",O_RDONLY);
  read(fp, blockdev, 255);
  close(fp);

  write(stdout,blockdev,strlen(blockdev));


  char buf[255];  
  int field = 0;

  char* tok = strtok(blockdev,"\t ");
  while (tok) {
    for (int i = 0; i<255; i++) {
      buf[i] = 0;
    }
    
    sprintf(buf, "%d: %d\n", field, (int)atol(tok));
    write(stdout, buf, strlen(buf));
    field++;
    tok = strtok(0x00,"\t ");
  }
}
