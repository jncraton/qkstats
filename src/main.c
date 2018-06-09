#include "ulibc.h"

int main() {
  char blockdev[255];

  FILE* fp = open("/sys/block/sda/stat",O_RDONLY);
  read(fp, blockdev, 255);
  close(fp);

  write(stdout,blockdev,strlen(blockdev));

  sleep(1);
  
  char buf[255];  
  int field = 0;

  char* tok = strtok(blockdev,"\t ");
  while (tok) {
    memset(buf,0,255);
    
    sprintf(buf, "%d: %d\n", field, (int)atol(tok));
    write(stdout, buf, strlen(buf));
    field++;
    tok = strtok(0x00,"\t ");
  }
}
