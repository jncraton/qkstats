// This should be in stddef.h...
#define size_t long
#define ssize_t signed long

#define stdin (void*)0
#define stdout (void*)1
#define stderr (void*)2

ssize_t write(void* fd, char* buf, size_t len) {
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

  return len;
}

ssize_t read(void* fd, char* buf, size_t len) {
  ssize_t bytes_read;

  asm ("mov $0,%%rax;" // Write is syscall 0
       "mov $0,%%rdi;" // stdin is file descriptor 0
       "mov %1,%%rsi;" // memory location to write
       "mov %2,%%rdx;" // length
       "mov %%r8,%%r10;"
       "mov %%r9,%%r8;"
       "syscall;"
       "mov %%rax,%0;"
       :"=r"(bytes_read)
       :"r"(buf),"r"(len)  /* 11 is input operand and it's related to %0 */
       :"rax","rdi","rsi","rdx","rcx","r8","r9","r10","r11"); /* clobbered registers */

  return bytes_read;
}
