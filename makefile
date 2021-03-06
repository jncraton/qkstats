CC=gcc
CCFLAGS=-m64 -mavx2 -Wall -Werror -funsigned-char -static -Os -nostdlib -ffunction-sections -fdata-sections -fno-math-errno -fno-unroll-loops -fmerge-all-constants -fno-ident -mfpmath=387 -mfancy-math-387 -ffast-math -Wl,--hash-style=gnu -Wl,--build-id=none -Wl,-z,norelro -Wl,--gc-sections -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-stack-protector
STRIPFLAGS=-S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt

all: qkstats

qkstats: src/main.c src/ulibc.h
	$(CC) $(CCFLAGS) -o qkstats src/main.c 
	strip $(STRIPFLAGS) qkstats

testulibc: src/testulibc.c src/ulibc.h
	$(CC) $(CCFLAGS) -o testulibc src/testulibc.c
	strip $(STRIPFLAGS) testulibc

testulibc-passed: testulibc
	./testulibc
	touch testulibc-passed

test: testulibc-passed qkstats
	@cat qkstats | wc
	time ./qkstats

clean:
	rm -f qkstats
	rm -f testulibc
	rm -f testulibc-passed