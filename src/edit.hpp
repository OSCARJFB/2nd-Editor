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
	void printText(text *head, int32_t viewStart, termxy xy);
	void setView(text **head, int32_t viewStart, int32_t view);
	void setViewBounderies(int32_t &view, int32_t &viewStart, text *cursor, int32_t ch);
	text *addText(text **head, text *cursor, int32_t ch, int64_t &bufferSize, int64_t id, termxy xy);
	text *deleteText(text **head, text *cursor, int32_t ch, int64_t &id, termxy xy);
	text *getKeyUp(text *cursor);
	text *getKeyDown(text *cursor);
	text *getKeyLeft(text *cursor);
	text *getKeyRight(text *cursor, text *head);
	text *readArrowKeys(text *head, text *cursor, int32_t ch);
	termxy updateCursor(text *cursor, termxy xy, int32_t ch);

public:
	edit(int8_t *buffer, int64_t bufferSize);
};
