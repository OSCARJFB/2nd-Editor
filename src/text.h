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
	int64_t id; 
	int32_t ch, x, y;
	struct text *next, *prev;	
} text;

text *findMemorySlot(text *head, uint32_t id, int64_t bufferSize, int32_t ch);
void addNode(text **head, text *newNode, int32_t x, int32_t y);
int64_t allocateMoreNodes(text **head, int64_t bufferSize);
text *allocateNodesFromBuffer(int8_t *buffer, int64_t bufferSize);
void deallocateNodes(text *head);

#endif
