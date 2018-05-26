// This should be in stddef.h...
#define size_t long
#define ssize_t signed long

#define stdin 0
#define stdout 1
#define stderr 2

ssize_t syscall3(size_t syscall, size_t a, size_t b, size_t c) {
  // https://filippo.io/linux-syscall-table/
  asm ("mov %0,%%rax;" // System call number
       "mov %1,%%rdi;" // arg1
       "mov %2,%%rsi;" // arg2
       "mov %3,%%rdx;" // arg3
       "mov %%r8,%%r10;"
       "mov %%r9,%%r8;"
       "syscall;"
       :
       :"r"(syscall),"r"(a),"r"(b),"r"(c)
       :"rax","rdi","rsi","rdx","r8","r9","r10","rcx","r11"); /* clobbered registers */

  register int ret asm("rax");
  return ret;
}

ssize_t write(size_t fd, char* buf, size_t len) {
  return syscall3(1, fd, (size_t)buf, len);
}

ssize_t read(size_t fd, char* buf, size_t len) {
  return syscall3(0, fd, (size_t)buf, len);
}
