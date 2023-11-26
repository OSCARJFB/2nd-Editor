#	Writen by: Oscar Bergström
#  	https://github.com/OSCARJFB
#	
#	MIT License
#	Copyright (c) 2023 Oscar Bergström

cc = g++
cflags = -Wall -Wextra -Werror -pedantic
cflags_release = -O3 -march=native -mtune=native -flto -fomit-frame-pointer
debugFlag = -g

main:
	$(cc) main.cpp file.cpp edit.cpp text.cpp padding.cpp $(cflags) -lncurses -o main.o
	
debug:
	$(cc) main.cpp file.cpp edit.cpp text.cpp padding.cpp $(debugFlag) -lncurses -o main.o

release: 
	$(cc) main.cpp file.cpp edit.cpp text.cpp padding.cpp $(cflags_release) -lncurses -o ob