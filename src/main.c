void _start() {
  asm("movl $1,%eax;"
      "xorl %ebx,%ebx;"
      "int  $0x80"
  );
}

