# APLIB Makefile

debug=-g -DDEBUG
compiler=gcc
compiler_cpp=g++
appname=m5
optim=-O0

all: units ap_app

ap_app: aplibcpp stringy colour ./m5.cpp ./m5.hpp
	$(compiler_cpp) $(optim) -o $(appname).exe -O0 $(debug) aplib_cpp.o ./../colour/colour_d.o ./../stringy/stringyd.o

aplibcpp: ./aplibcpp.cpp ./aplibcpp.hpp aplib 
	$(compiler_cpp) -O0 $(debug) aplib.o -c -o aplib_cpp.o

aplib: ./aplib.c ./aplib.h stringy
	$(compiler) -c $(debug) -O0 -o ./aplib.o ./aplib.c

units: ./unittests.c ./unittests.h aplib colour
	$(compiler) $(debug) -o ./test.exe -O0 ./unittests.c ./aplib.o ./../colour/colour_d.o

## Dependencies

stringy:
	gnumake -C ./../stringy/ stringyd

colour:
	gnumake -C ./../colour/ colour_d


## Manage
	
clean:
	del *.o
	del *.exe
	gnumake -C ./../stringy/ clean
	gnumake -C ./../colour/ clean

