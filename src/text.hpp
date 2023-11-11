/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#pragma once

#include <cstdlib>
#include <cstdint>

class text
{
public:
	bool isInUse;
	text *next, *prev;
	int32_t ch, x, y;
	int64_t id;

protected:
	bool getNode(text *node);
	text *getNode(text *node, int32_t x, int32_t y);
	text *findMemorySlot(text *head, int64_t id, int64_t bufferSize, int32_t ch);
	text *addNode(text **head, text *newNode, int32_t x, int32_t y);
	text *deleteNode(text **head, int32_t x, int32_t y, int64_t &id);
	text *allocateNodesFromBuffer(int8_t *buffer, int64_t bufferSize);
	int64_t allocateMoreNodes(text **head, int64_t bufferSize);
	void deallocateNodes(text **head);
};