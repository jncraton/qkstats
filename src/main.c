#include "ulibc.h"

int main() {
  char buf[65535];

  sprintf(buf, "Some numbers: %d %d %d %d\n", 42, 100, 200, 675);
  write(stdout, buf, strlen(buf));

  FILE* fp = open("/sys/block/sda/stat",O_RDONLY);
  size_t len = read(fp, buf, 65535);
  close(fp);
  write(stdout, buf, len);

  /*
        if stat == 'requests':
          tot[0] += int(stats[0]) # Read requests
          tot[1] += int(stats[4]) # Write requests
        elif stat == 'in_flight':
          tot[0] += int(stats[8])
        elif stat == 'ticks':
          tot[0] += int(stats[9]) / 1000.0
        elif stat == 'wait_time':
          tot[0] += int(stats[10])
        elif stat == 'sectors':
          tot[0] += int(stats[2])
          tot[1] += int(stats[6])
        elif stat == 'megabytes':
          tot[0] += int(stats[2]) * 512 / 1024 / 1024 # Sectors read to megabytes
          tot[1] += int(stats[6]) * 512 / 1024 / 1024 # Sectors written to megabytes
  
  */
}
