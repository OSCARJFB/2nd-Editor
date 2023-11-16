/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#pragma once

#include <ncurses.h>
#include <cstdint>
#include <cstdlib>
#include "text.hpp"

class edit : text
{
private:
	typedef struct termxy
	{
		int32_t x, y;
	} termxy;

private:
	void curseMode(bool isCurse);
	void printText(text *head, int32_t viewStart, int32_t view, termxy xy);
	void setView(text **head, int32_t viewStart, int32_t view);
	bool getNode(text *node);
	int32_t setViewStart(int32_t view, int32_t viewStart, text *head, text *cursor, int32_t ch, int32_t delch);
	int32_t getNewLinesInView(text *node, int32_t view);
	text *addText(text **head, text *cursor, int32_t ch, int64_t &bufferSize, int64_t id, termxy xy);
	text *deleteText(text **head, text *cursor, int32_t ch, int32_t &delch, int64_t &id, termxy xy);
	text *getKeyUp(text *cursor);
	text *getKeyDown(text *cursor, text *head);
	text *getKeyLeft(text *cursor);
	text *getKeyRight(text *cursor, text *head);
	text *readArrowKeys(text *head, text *cursor, int32_t ch);
	text *getViewStartNode(text *cursor);
	termxy updateCursor(text *cursor, termxy xy);

public:
	edit(int8_t *buffer, int64_t bufferSize);
};