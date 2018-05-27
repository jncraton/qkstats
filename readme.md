qkstats
====

Provides quick stats from the kernel for 64 bit Linux systems.

This is mostly a personal experiment to see what it's like build an application without using libc. This implements system calls and does some basic reading from the /sys and /proc filesystems to return some basic system info. My goal is to make the binary very tiny on the order of a few KiB.