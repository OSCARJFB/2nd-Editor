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

static void printText(text *head, int32_t viewStart, bool drawArrowKeys, termxy xy)
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

	if(drawArrowKeys)
	{
		move(xy.y, xy.x);
	}

	refresh(); 
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

int64_t modifyText(text **head, int32_t ch, int64_t bufferSize, 
		int64_t *id, termxy xy)
{
	if((ch >= ' ' && ch <= '~') || (ch == '\t' || ch == '\n'))
	{
		text *newNode = findMemorySlot(*head, *id, bufferSize, ch);
		if(newNode == NULL)
		{
			bufferSize = allocateMoreNodes(head, bufferSize);
			newNode = findMemorySlot(*head, *id, bufferSize, ch);
		}
		
		addNode(head, newNode, xy.x, xy.y);
	}
	else if(ch == KEY_BACKSPACE)
	{
		*id = deleteNode(head, xy.x, xy.y); 
	}

	return bufferSize;
}

termxy moveCursor(int32_t ch, termxy xy, bool *drawArrowKeys)
{
	switch(ch)
	{
		case KEY_UP:
			--xy.y;
			break;
		case KEY_DOWN:
			++xy.y;
			break; 
		case KEY_LEFT:
			--xy.x;
			break;
		case KEY_RIGHT:
			++xy.x;
			break; 
		default:
			*drawArrowKeys = false; 
			return xy; 

	}
	
	*drawArrowKeys = true;  
	return xy; 
}

void edit(text *head, int64_t bufferSize)
{
	curseMode(true); 
	
	bool drawArrowKeys = false; 
	int32_t viewStart = 0, view = getmaxy(stdscr);
	int64_t id = 0; 
	termxy xy = {0, 0};

	for(int32_t ch = 0; ch != EOF; ch = getch())
	{
		getyx(stdscr, xy.y, xy.x); 
		bufferSize = modifyText(&head, ch, bufferSize, &id, xy);
		xy = moveCursor(ch, xy, &drawArrowKeys); 
		setView(&head, viewStart, view);
		printText(head, view, drawArrowKeys, xy);
	}

	curseMode(false); 
}	
