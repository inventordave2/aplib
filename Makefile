# APLIB Makefile

debug=-g -DDEBUG

stringy:
	make -C ./../stringy stringyd
	
aplib: ./aplib.c ./aplib.h stringy
	clang -c $(debug) -O0 -o ./aplib.o ./aplib.c

test:
units: ./units.c ./units.h aplib stringy
	clang $(debug) -o ./test.exe -O0 ./units.c ./aplib.o ./../stringy/stringyd.o

