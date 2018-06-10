#include "ulibc.h"

long nth_int(char* filename, int n) {
  char content[255];

  FILE* fp = open(filename, O_RDONLY);
  if (fp == -1) { return -1 };
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

int get_cpu_stats(long* idle) {
  /** Gets CPU total idle time

  Returns CPU count
  
  This will cause an overflow if `idle` doesn't have enough memory 
  allocated for all cpus.
  **/
  char buf[4096];

  FILE* fp = open("/proc/stat", O_RDONLY);
  read(fp, buf, 4096);
  close(fp);

  char* tok = strtok(buf,"\n\t ");
  int cpu = -1;
  while (tok) {
    if (tok[0] == 'c' && tok[1] == 'p' && tok[2] == 'u') {
      cpu++;
      for (int i = 0; i < 3; i++) { strtok(0,"\n\t "); }
      idle[cpu] = atol(strtok(0,"\n\t "));
    }
    tok = strtok(0,"\n\t ");
  }

  return cpu;
}

const char circles[][4] = {"○","◔","◑","◕","●"};
const char squares_growing[][8] = {"·","▪","■"};
const char squares_filling[][4] = {"□","⬓","■"};
const char bars_vert[][4] = {" ","▁","▂","▃","▄","▅","▆","▇","█"};

int main() {
  int bat = nth_int("/sys/class/power_supply/BAT0/capacity",0);
  long io_start = nth_int("/sys/block/sda/stat", 9);

  long cpu_idle_start[9];
  get_cpu_stats(cpu_idle_start);

  sleep(1);

  long io_ticks = nth_int("/sys/block/sda/stat", 9) - io_start;

  long cpu_idle_end[9];
  int n_cpus = get_cpu_stats(cpu_idle_end);

  char cpus[128]; // Needs to allocate at least 3n+1 where n is CPU count
  int pos = 0;

  for (int i = 1; i <= n_cpus; i++) {
    int idle = cpu_idle_end[i] - cpu_idle_start[i];

    const char* state = squares_growing[0];
    if (idle < 10) {
      state = squares_growing[2];
    } else if (idle < 97) {
      state = squares_growing[1];
    }

    for (int j = 0; j < 4; j++) {
      if (!state[j]) { break; }
      cpus[pos] = state[j]; pos++;
    }
  }

  int io_status = min(((io_ticks + 332) / 333), 4);
  
  char buf[64];
  sprintf(buf, "%s %s %d%% \n", cpus, circles[io_status], (int)bat);
  write(stdout, buf, strlen(buf));

  return 0;
}
