/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <ncurses.h>
#include "text.h"

int main(int32_t argc, int8_t **argv)
{	
	int8_t *buffer = (int8_t *)"oscar";
	uint32_t bufferSize = 5; 


	text *head = allocateNodesFromBuffer(buffer, bufferSize);
	
	for(text *node = head; node != NULL; node = node->next)
	{
		printf("%c", node->ch);
	}

	return 0;
}
