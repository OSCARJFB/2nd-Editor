#	Writen by: Oscar Bergström
#  	https://github.com/OSCARJFB
#	
#	MIT License
#	Copyright (c) 2023 Oscar Bergström

cc = gcc
cflags = -Wall -Wextra -Werror -pedantic
debugFlag = -g

main:
	$(cc) main.c file.c edit.c text.c $(cflags) -lncurses -o main.o
	
debug:
	$(cc) main.c file.c edit.c text.c $(debugFlag) -lncurses -o main.o
