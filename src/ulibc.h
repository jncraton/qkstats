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

char* strchr(const char *s, int c) {
  // Returns a pointer to the first occurrence of c in s
  // Returns null if c is not in s
  while(*s != 0x00 && (int)(*s) != c) { s++; }
  if (*s == 0x00) { return (char*)0x00; }
  return (char*)s;
}

char* strtok(char* str, const char* delim) {
  static char* next = 0x00;

  if (!str) { str = next; }
  if (!str) { return 0x00; }

  // Skip delimiters
  // not done && the position of the current char in the delimiter isn't null
  while(*str && strchr(delim,*str)) { str++; }
  char* start = str;
  
  // Skip non-delimiters
  while(*str && !strchr(delim,*str)) { str++; }
  if (*str) {
    *(str) = 0x00;
    next = str+1;
  } else {
    next = 0x00;
  }
  
  return start;
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

This currently only supports a maximum of six total arguments.

https://blog.nelhage.com/2010/10/amd64-and-va_arg/
*/

typedef struct {
  void* start;
  unsigned int count;
  size_t register_args[6];
} va_list;

#define va_start(vl,start) \
  register size_t arg1 asm ("rdi");\
  register size_t arg2 asm ("rsi");\
  register size_t arg3 asm ("rdx");\
  register size_t arg4 asm ("rcx");\
  register size_t arg5 asm ("r8");\
  register size_t arg6 asm ("r9");\
  vl = (va_list){0,1};\
  if ((size_t)arg2 == (size_t)start) { vl.count++; }\
  if ((size_t)arg3 == (size_t)start) { vl.count+=2; }\
  if ((size_t)arg4 == (size_t)start) { vl.count+=3; }\
  if ((size_t)arg5 == (size_t)start) { vl.count+=4; }\
  if ((size_t)arg6 == (size_t)start) { vl.count+=5; }\
  vl.register_args[0] = arg1;\
  vl.register_args[1] = arg2;\
  vl.register_args[2] = arg3;\
  vl.register_args[3] = arg4;\
  vl.register_args[4] = arg5;\
  vl.register_args[5] = arg6;

#define va_arg(v1,type) ((vl.count < 6) ? vl.register_args[vl.count] : *((type *)vl.start));\
  vl.count++;\
  if (vl.count >= 6) { vl.start++;  };

int sprintf(char * str, const char* format, ...) {
  va_list vl;
  va_start(vl,format);
  
  while (*format != 0x00) {
    if (*format == '%') {
      format++;
      if (*format == '%') {
        *str = '%'; str++;
      } else if (*format == 'd') {
        size_t arg = va_arg(vl,size_t);      
        char in_leading_zeroes = 1;
        for (size_t i=1e18; i>0; i=i/10) {
          int digit = arg / i;
          if (digit > 0 || (digit==0 && !in_leading_zeroes)) {
            in_leading_zeroes = 0;
            arg -= digit * i;
            *str = '0' + digit; str++;
          }
        }
        if (in_leading_zeroes) { *str = '0'; str++; }
      }
      format++;      
    } else {
      *str = *format; 
      str++; format++;
    }
  }
  return 0;
}

#define assert(expr) if (!(expr)) { write(stderr,"ASSERTION FAILED: " #expr "\n",strlen(#expr) + 19); exit(1); }

// Bootstrap our main function
int main();
void _start() {
  exit(main());
}