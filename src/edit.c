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

void moveArrowKeys(int32_t ch, text **head, text **cursor)
{
	if(head == NULL)
	{
		return; 
	}

	if(cursor == NULL)
	{
		*cursor = *head; 
	}

	text *node = *cursor; 
	switch(ch)
	{
		case KEY_UP:
			while(node->prev != NULL)
			{
				node = node->next; 
			}
			break;
		case KEY_DOWN:
			while(node->next != NULL)
			{
				node = node->next; 
			}			
			break;
		case KEY_LEFT:
			if(node->prev == NULL)
			{
				node = node->prev; 
			}
			break;
		case KEY_RIGHT:
			if(node->next == NULL)
			{
				node = node->next; 
			}
			break;
	}
}

int64_t editText(text **head, text **cursor, 
		int32_t ch, int64_t bufferSize, int32_t *id)
{
	if((ch >= ' ' && ch <= '~') || (ch == '\t' || ch == '\n'))
	{
		text *newNode = findMemorySlot(*head, *id, bufferSize, ch);
		if(newNode == NULL)
		{
			bufferSize = allocateMoreNodes(head, bufferSize);
		}
		
		addNode(cursor, newNode);
	}
	else if(ch == KEY_BACKSPACE)
	{
		*id = delNode(cursor); 
	}

	return bufferSize;
}

void edit(text *head, int64_t bufferSize)
{
	curseMode(true); 
	
	text *cursor = head; 
	
	int32_t viewStart = 0, view = getmaxy(stdscr), id = 0;  
	for(int32_t ch = 0; setView(&head, viewStart, view), printText(head, cursor, view); ch = getch())
	{
		moveArrowKeys(ch, &head, &cursor);
		bufferSize = editText(&head, &cursor, ch, bufferSize, &id);
	}

	curseMode(false); 
}	
