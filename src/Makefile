#	Writen by: Oscar Bergström
#  	https://github.com/OSCARJFB
#	
#	MIT License
#	Copyright (c) 2023 Oscar Bergström

cc = g++
cflags = -Wall -Wextra -Werror -pedantic
debugFlag = -g

main:
	$(cc) main.cpp file.cpp edit.cpp text.cpp $(cflags) -lncurses -o main.o
	
debug:
	$(cc) main.cpp file.cpp edit.cpp text.cpp $(debugFlag) -lncurses -o main.o
