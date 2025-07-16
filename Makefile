# APLIB Makefile

debug=-g -DDEBUG
compiler=clang

stringy:
	$(compiler) -c -O0 c:\dev\repos\stringy\stringy.c $(debug)  -o stringyd.o
	
aplib: ./aplib.c ./aplib.h stringy
	$(compiler) -c $(debug) -O0 -o ./aplib.o ./aplib.c

units: ./unittests.c ./unittests.h aplib stringy
	$(compiler) $(debug) -o ./test.exe -O0 ./units.c ./aplib.o c:\dev\repos\stringy\stringyd.o
	
all: units
	

