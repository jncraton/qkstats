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

const char circles[][4] = {"○","◔","◑","◕","●"};
const char squares_growing[][4] = {"·","▪","■"};
const char squares_filling[][4] = {"□","⬓","■"};
const char bars_vert[][4] = {" ","▁","▂","▃","▄","▅","▆","▇","█"};
const char bars_horiz[][4] = {" ","▏","▎","▍","▌","▋","▊","▉","█"};

int main() {
  long io_start = nth_int("/sys/block/sda/stat", 9);

  sleep(1);

  long io_ticks = nth_int("/sys/block/sda/stat", 9) - io_start;

  int circle = min(((io_ticks + 332) / 333), 4);
  
  char buf[64];
  sprintf(buf, "I/O ticks: %s %d\n", circles[circle], (int)io_ticks);
  write(stdout, buf, strlen(buf));

  return 0;
}
