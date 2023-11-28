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
#define STARTNODE getNewLinesInView((m_cursor != nullptr ? getViewStartNode(pad) : m_head), viewEnd)

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
		deallocateNodes(&m_head);
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
	text *m_head, *m_cursor = nullptr;

private:
	void curseMode(bool isCurse);
	void printText(int32_t viewStart, int32_t view, const termxy &xy);
	void setView(int32_t viewStart, int32_t view, const padding& pad);
	void updateCursor(termxy &xy, const padding &pad);
	void updatePadding(padding &p, int32_t ch);
	void printLines(int32_t newLines, int32_t newLinesInView);
	bool isNodeAtPrevLine(text *node);
	bool isNodeAtNextLine(text *node);
	int32_t setViewStart(int32_t viewStart, int32_t viewEnd, int32_t ch, int32_t d_ch, const padding &pad);
	int32_t getNewLinesInView(text *node, int32_t view);
	text *addText(int32_t ch, uint32_t &bufferSize, uint32_t currentId, const termxy& xy, const padding& pad);
	text *deleteText(int32_t ch, int32_t &d_ch, uint32_t &currentId, const termxy& xy, const padding& pad);
	text *getKeyUp(void);
	text *getKeyDown(void);
	text *getKeyLeft(void);
	text *getKeyRight(void);
	text *readArrowKeys(int32_t ch);
	text *getViewStartNode(const padding& pad);
};