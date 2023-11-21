/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#pragma once

#include <string>
#include <cstdint>
#include <ncurses.h>
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
	bool isNodeAtPrevLine(text *node);
	bool isNodeAtNextLine(text *node);
	int32_t setViewStart(int32_t view, int32_t viewStart, text *head, text *cursor, int32_t ch, int32_t delch);
	int32_t getNewLinesInView(text *node, int32_t view);
	text *addText(text **head, text *cursor, int32_t ch, size_t &bufferSize, size_t id, termxy xy);
	text *deleteText(text **head, text *cursor, int32_t ch, int32_t &delch, size_t &id, termxy xy);
	text *getKeyUp(text *cursor);
	text *getKeyDown(text *cursor, text *head);
	text *getKeyLeft(text *cursor);
	text *getKeyRight(text *cursor, text *head);
	text *readArrowKeys(text *head, text *cursor, int32_t ch);
	text *getViewStartNode(text *cursor);
	termxy updateCursor(text *cursor, termxy xy);

public:
	edit(std::string &buffer, size_t bufferSize);
};