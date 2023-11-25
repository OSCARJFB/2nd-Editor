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
	int16_t x, y;
	int32_t ch;
	uint32_t id;
	text *next, *prev;

protected:
	text *getNode(text *node, int32_t x, int32_t y);
	text *findMemorySlot(text *head, uint32_t currentId, uint32_t bufferSize, int32_t ch);
	text *addNode(text **head, text *newNode, int32_t x, int32_t y);
	text *deleteNode(text **head, int32_t x, int32_t y, uint32_t &currentId);
	text *allocateNodesFromBuffer(const std::string &buffer, uint32_t bufferSize);
	uint32_t allocateMoreNodes(text **head, uint32_t bufferSize);
	void deallocateNodes(text **head);
};