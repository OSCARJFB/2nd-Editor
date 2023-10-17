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
			if(cursor->prev == NULL)
			{
				cursor = cursor->prev; 
			}
			break;
		case KEY_RIGHT:
			if(cursor->next == NULL)
			{
				cursor = cursor->next; 
			}
			break;
	}

	return cursor; 
}

static int32_t printText(text *head, text *cursor, int viewStart)
{
	clear(); 

	int32_t newLines = 0;
	for(text *node = head; node != NULL; node = node->next)
	{
		newLines += node->ch == '\n' ? 1 : 0; 
		if(viewStart < newLines)
		{
			continue; 
		}

		mvwaddch(stdscr, node->y, node->x, node->ch); 
	}
	
	if(head != NULL)
	{
		cursor = cursor == NULL ? head : cursor; 
		move(cursor->y, cursor->x);
	}

	refresh();

	return 1; 	
}

static int32_t setView(text **head, int32_t viewStart, int32_t view)
{
	if(*head == NULL)
	{
		return 1;
	}

	int32_t newLines, newLinesInView, x, y; 
      	newLines = newLinesInView = x = y = 0; 

	for(text *node = *head; node != NULL; node = node->next)
	{
		if(newLines >= viewStart)
		{
			newLinesInView += node->ch == '\n' ? 1 : 0;
			node->x = x;
			node->y = y;

			if(node->ch == '\t')
			{
				x += 8;
			}
			else
			{
				++x;
			}

			if(node->ch == '\n')
			{
				x = 0;
				++y;
			}
		}

		newLines += node->ch == '\n' ? 1 : 0;
		if(newLinesInView == view)
		{
			break;
		}
	}

	return 1;
}

void edit(text *head)
{
	curseMode(true); 
	
	text *cursor = head; 
	
	int32_t viewStart = 0, view = getmaxy(stdscr); 
	for(int32_t ch = 0; setView(&head, viewStart, view), printText(head, cursor, view); ch = getch())
	{
		moveArrowKeys(ch, head, cursor);
	}

	curseMode(false); 
}	
