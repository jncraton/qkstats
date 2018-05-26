all: qkstats

qkstats: src/main.c
	gcc -m64 -o qkstats -Wall -Werror -funsigned-char -static -Os -nostdlib -Wl,--gc-sections -fno-unwind-tables -fno-asynchronous-unwind-tables src/main.c 
	strip qkstats
	cat qkstats | wc

test: qkstats
	./qkstats

clean:
	rm qkstats