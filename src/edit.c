/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include "edit.h"

static void curseMode(bool isCurse)
{
	if (isCurse)
	{
		initscr();
		cbreak();
		noecho();
		curs_set(1);
		keypad(stdscr, TRUE);
		return; 
	}
	endwin();
}

static text *moveArrowKeys(int32_t ch, text *head, text *cursor)
{
	if(head == NULL)
	{
		return NULL; 
	}

	if(cursor == NULL)
	{
		cursor = head; 
	}

	
	switch(ch)
	{
		case KEY_UP:
			while(cursor->prev != NULL)
			{
				cursor = cursor->next; 
			}
			break;
		case KEY_DOWN:
			while(cursor->next != NULL)
			{
				cursor = cursor->next; 
			}			
			break;
		case KEY_LEFT:
			cursor = cursor->prev == NULL ? cursor->prev : cursor; 
			break;
		case KEY_RIGHT:
			cursor = cursor->next == NULL ? cursor->next : cursor; 
			break;
	}

	return cursor; 
}

void edit(text *head)
{
	curseMode(true); 
	
	bool isRunning = true; 
	for(int32_t ch = 0; isRunning; ch = getch())
	{
		moveArrowKeys(head, cursor); 
	}

	curseMode(false); 
}	
