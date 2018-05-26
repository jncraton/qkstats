void _start() {
  main();

  asm("movl $1,%eax;"
      "xorl %ebx,%ebx;"
      "int  $0x80"
  );
}
