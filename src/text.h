/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#ifndef TEXT_H
#define TEXT_H

#include <stdbool.h>
#include <stdint.h>

typedef struct text
{
	bool isInUse; 
	uint32_t id; 
	int32_t ch, x, y;
	struct text *next, *prev;	
} text;

text *findMemorySlot(text *head, uint32_t id, uint32_t bufferSize, int32_t ch);
void add(text **cursor, text *newnode);
uint32_t del(text **cursor);
uint32_t allocateMoreNodes(text **head, uint32_t bufferSize);
text *allocateNodesFromBuffer(int8_t *buffer, uint32_t bufferSize);
void deallocateNodes(text *head);

#endif
