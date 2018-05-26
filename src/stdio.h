#define stdin (void*)0
#define stdout (void*)1
#define stderr (void*)2

void write(void* fd, char* buf, long len) {
  asm ("mov $1,%%rax;" // Write is syscall 1
       "mov $1,%%rdi;" // stdout is file descriptor 1
       "mov %0,%%rsi;" // memory location to read
       "mov %1,%%rdx;" // length
       "mov %%r8,%%r10;"
       "mov %%r9,%%r8;"
       "syscall;"
       :
       :"r"(buf),"r"(len)  /* 11 is input operand and it's related to %0 */
       :"rax","rdi","rsi","rdx","rcx","r8","r9","r10","r11"); /* clobbered registers */  
}

long read(void* fd, char* buf, long len) {
  asm ("mov $0,%%rax;" // Write is syscall 0
       "mov $0,%%rdi;" // stdin is file descriptor 0
       "mov %0,%%rsi;" // memory location to write
       "mov %1,%%rdx;" // length
       "mov %%r8,%%r10;"
       "mov %%r9,%%r8;"
       "syscall;"
       :
       :"r"(buf),"r"(len)  /* 11 is input operand and it's related to %0 */
       :"rax","rdi","rsi","rdx","rcx","r8","r9","r10","r11"); /* clobbered registers */

  return 14;
}
