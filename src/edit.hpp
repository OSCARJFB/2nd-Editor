/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#pragma once

extern "C"
{
#include <ncurses.h>
}

#include <string>
#include <cstdint>
#include "text.hpp"
#include "padding.hpp"

#define KEY_ESCAPE 27

class edit : private text
{
public:
	explicit edit(const std::string &buffer, uint32_t bufferSize)
		: m_bufferSize(bufferSize), m_head(allocateNodesFromBuffer(buffer, bufferSize))
	{
		curseMode(true);
	}

	~edit()
	{
		deallocateNodes(m_head);
		curseMode(false);
	}

	void run(void);

private:
	typedef struct termxy
	{
		int32_t x, y;
	} termxy;

private:
	const uint32_t m_bufferSize;
	text *m_head;

private:
	void curseMode(bool isCurse);
	void printText(text *head, int32_t viewStart, int32_t view, termxy xy);
	void setView(text **head, int32_t viewStart, int32_t view);
	bool isNodeAtPrevLine(text *node);
	bool isNodeAtNextLine(text *node);
	int32_t setViewStart(int32_t view, int32_t viewStart, text *head, text *cursor, int32_t ch, int32_t dch);
	int32_t getNewLinesInView(text *node, int32_t view);
	inline int32_t updatePadding(padding &p, int32_t ch);
	text *addText(text **head, text *cursor, int32_t ch, uint32_t &bufferSize, uint32_t currentId, termxy xy);
	text *deleteText(text **head, text *cursor, int32_t ch, int32_t &dch, uint32_t &currentId, termxy xy);
	text *getKeyUp(text *cursor);
	text *getKeyDown(text *cursor, text *head);
	text *getKeyLeft(text *cursor);
	text *getKeyRight(text *cursor, text *head);
	text *readArrowKeys(text *head, text *cursor, int32_t ch);
	text *getViewStartNode(text *cursor);
	termxy updateCursor(text *cursor, termxy xy);
};