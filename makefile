all: qkstats

qkstats: src/main.c
	gcc src/start.S -o qkstats -Wall -Werror -funsigned-char -static -Os -nostdlib src/main.c 
	strip qkstats