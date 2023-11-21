/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#pragma once

#include <string>
#include <cstdlib>
#include <cstdint>

class text
{
public:
	bool isInUse;
	text *next, *prev;
	int32_t ch, x, y;
	size_t id;

protected:
	text *getNode(text *node, int32_t x, int32_t y);
	text *findMemorySlot(text *head, size_t id, size_t bufferSize, int32_t ch);
	text *addNode(text **head, text *newNode, int32_t x, int32_t y);
	text *deleteNode(text **head, int32_t x, int32_t y, size_t &id);
	text *allocateNodesFromBuffer(std::string &buffer, size_t bufferSize);
	size_t allocateMoreNodes(text **head, size_t bufferSize);
	void deallocateNodes(text **head);
};