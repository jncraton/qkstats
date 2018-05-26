all: qkstats

qkstats: src/main.c src/stdio.h src/bootstrap.h
	gcc -m64 -o qkstats -Wall -Werror -funsigned-char -static -Os -nostdlib -Wl,--gc-sections -fno-unwind-tables -fno-asynchronous-unwind-tables src/main.c 
	strip qkstats

test: qkstats
	@cat qkstats | wc
	@echo Hello, World! | ./qkstats
	@echo Longer Hello, World! | ./qkstats

clean:
	rm qkstats