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
#define stdin (FILE*)0
#define stdout (FILE*)1
#define stderr (FILE*)2

typedef ssize_t FILE;
#define O_RDONLY 00
#define O_WRONLY 01
#define O_RDWR   02
#define read(fd,buf,len) syscall(0,fd,buf,len)
#define write(fd,buf,len) syscall(1,fd,buf,len)
#define open(filename,flags) (FILE*)syscall(2,filename,flags)
#define close(fd) syscall(3,fd)
#define exit(error_code) syscall(60,error_code)

// stdlib.h

size_t strlen(const char* str) {
  int i = 0;
  while(str[i] != 0x00) { i++; }
  return i;
}

long int strtol(const char *nptr, const char* endptr, int base) {
  // https://codereview.stackexchange.com/a/45769
  long int value = 0;
  int sign = 1;
  if( *nptr == '+' || *nptr == '-' ) {
    if( *nptr  == '-' ) sign = -1;
    nptr++;
  }
  while (nptr <= endptr) {
    value *= base;
    value += (int) (*nptr-'0');
    nptr++;
  }
  return (value * sign);
}

/*
Variadic function support

This isn't quite right. This isn't as simple as incrementing
a pointer on the stack in x86_64. If you need to use this,
read the source carefully for the implementation details.
*/

typedef struct {
  void* start;
  unsigned int count;
  size_t register_args[4];
} va_list;

inline void __attribute__((always_inline)) va_start(va_list* vl, void* start) {
  *vl = (va_list){start, 0};

  asm("mov %%rdx,%0" : "=r"(vl->register_args[0]));
  asm("mov %%rcx,%0" : "=r"(vl->register_args[1]));
  asm("mov %%r8,%0" : "=r"(vl->register_args[2]));
  asm("mov %%r9,%0" : "=r"(vl->register_args[3]));
}

inline size_t __attribute__((always_inline)) _va_arg(va_list* vl) {
  size_t ret;
  if (vl->count < 4) {
    ret = vl->register_args[vl->count];
  } else {
    ret = 0; //TODO: handle stack args
  }

  vl->count++;

  return ret;
}
#define va_arg(vl, type) _va_arg(vl)

int sprintf(char * str, const char* format, ...) {
  va_list vl = {&format,0};
  va_start(&vl, &format);
  
  while (*format != 0x00) {
    if (*format == '%') {
      format++;
      if (*format == '%') {
        *str = '%'; str++;
      } else if (*format == 'd') {
        size_t arg = va_arg(&vl,size_t);      
        char in_leading_zeroes = 1;
        for (size_t i=1e18; i>0; i=i/10) {
          int digit = arg / i;
          if (digit > 0 || (digit==0 && !in_leading_zeroes)) {
            in_leading_zeroes = 0;
            arg -= digit * i;
            *str = '0' + digit; str++;
          }
        }
      }
      format++;      
    } else {
      *str = *format; 
      str++; format++;
    }
  }
  return 0;
}

// Bootstrap our main function
int main();
void _start() {
  exit(main());
}