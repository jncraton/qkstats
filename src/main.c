void _start() {
    const char* msg = "Hello, World!\n";
    long len = 14;

    asm ("mov $1,%%rax;" // Write is syscall 1
         "mov $1,%%rdi;" // stdout is file descriptor 1
         "mov %0,%%rsi;" // memory location to write
         "mov %1,%%rdx;" // length
         "mov %%r8,%%r10;"
         "mov %%r9,%%r8;"
         "syscall;"
         :
         :"r"(msg),"r"(len)  /* 11 is input operand and it's related to %0 */
         :"rax","rdi","rsi","rdx"); /* %eax is clobbered register */  

  asm("movl $1,%eax;"
      "xorl %ebx,%ebx;"
      "int  $0x80"
  );
}