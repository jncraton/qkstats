int main();

// stddef.h
#define size_t long
#define ssize_t signed long

// unistd.h
ssize_t syscall(size_t sysno, ...) {
  // The x86_64 calling convention uses rdi, rsi, rdx, rcx, r8 and r9 as args
  // The kernel interface uses rdi, rsi, rdx, r10, r8 and r9 for arg and rax for sysno
  
  asm ("mov %%rdi,%%rax;" // System call number (sysno)
       "mov %%rsi,%%rdi;" // first syscall arg and va_arg
       "mov %%rdx,%%rsi;" // second syscall arg and va_arg
       "mov %%rcx,%%rdx;" // third syscall arg and va_arg
       "mov %%r8,%%r10;"  // fourth syscall arg and va_arg
       "mov %%r9,%%r8;"   // fifth syscall arg and va_arg
       "syscall;"
       :
       :
       :"rax","rdi","rsi","rdx","r8","r9","r10","rcx","r11"); // clobbered registers

  register ssize_t ret asm("rax");
  return ret;
}

// stdio.h
#define stdin 0
#define stdout 1
#define stderr 2

typedef ssize_t FILE;
#define O_RDONLY 00
#define O_WRONLY 01
#define O_RDWR   02
#define read(fd,buf,len) syscall(0,fd,(size_t)buf,len)
#define write(fd,buf,len) syscall(1,fd,(size_t)buf,len)
#define open(filename,flags) (FILE*)syscall(2,(size_t)filename,flags)
#define close(fd) syscall(3,(size_t)fd)
#define exit(error_code) syscall(60,error_code)

// Bootstrap our main function
void _start() {
  exit(main());
}