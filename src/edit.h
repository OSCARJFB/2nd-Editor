/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#ifndef EDIT_H
#define EDIT_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdint.h>
#include "text.h"

void edit(text *head, int64_t bufferSize); 

#endif
