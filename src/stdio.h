// This should be in stddef.h...
#define size_t long
#define ssize_t signed long

#define stdin 0
#define stdout 1
#define stderr 2

ssize_t syscall3(size_t sysno, size_t a, size_t b, size_t c) {
  /*
  size_t* arg = &sysno;

  size_t args[5];
  for (int i = 0; i < 5; i++) {
    arg++;
    args[i] = *arg;
  }
  */
  
  // implements a 3 parameter system call
  // https://filippo.io/linux-syscall-table/
  asm ("mov %0,%%rax;" // System call number
       "mov %1,%%rdi;" // arg1
       "mov %2,%%rsi;" // arg2
       "mov %3,%%rdx;" // arg3
       "mov %%r8,%%r10;"
       "mov %%r9,%%r8;"
       "syscall;"
       :
       :"r"(sysno),"r"(a),"r"(b),"r"(c)
       :"rax","rdi","rsi","rdx","r8","r9","r10","rcx","r11"); /* clobbered registers */

  register int ret asm("rax");
  return ret;
}

#define read(fd,buf,len) syscall3(0,fd,(size_t)buf,len)
#define write(fd,buf,len) syscall3(1,fd,(size_t)buf,len)
