CC=gcc

all: qkstats

qkstats: src/main.c src/ulibc.h makefile
	$(CC) -m64 -mavx2 -o qkstats -Wall -Werror -funsigned-char -static -Os -nostdlib -ffunction-sections -fdata-sections -fno-math-errno -fno-unroll-loops -fmerge-all-constants -fno-ident -mfpmath=387 -mfancy-math-387 -ffast-math -Wl,--hash-style=gnu -Wl,--build-id=none -Wl,-z,norelro -Wl,--gc-sections -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-stack-protector src/main.c 
	strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt qkstats

testulibc: src/testulibc.c src/ulibc.h
	$(CC) -m64 -mavx2 -Wall -Werror -static -nostdlib -o testulibc src/testulibc.c
	@./testulibc

test: testulibc qkstats
	@cat qkstats | wc
	@./testulibc
	@./qkstats

clean:
	rm qkstats