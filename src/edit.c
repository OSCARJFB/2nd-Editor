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

static int32_t printText(text *head, int32_t viewStart)
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

int64_t editText(text **head, int32_t ch, int64_t bufferSize, int32_t *id, int32_t x, int32_t y)
{
	if((ch >= ' ' && ch <= '~') || (ch == '\t' || ch == '\n'))
	{
		text *newNode = findMemorySlot(*head, *id, bufferSize, ch);
		if(newNode == NULL)
		{
			bufferSize = allocateMoreNodes(head, bufferSize);
			newNode = findMemorySlot(*head, *id, bufferSize, ch);
		}
		
		addNode(head, newNode, x, y);
	}

	return bufferSize;
}

void edit(text *head, int64_t bufferSize)
{
	curseMode(true); 
	
	int32_t viewStart = 0, view = getmaxy(stdscr); x = 0, y = 0;
	int64_t id = 0; 

	for(int32_t ch = 0; ch != EOF; ch = getch())
	{
		bufferSize = editText(&head, ch, bufferSize, &id, x, y);
		setView(&head, viewStart, view);
		printText(head, view);
	}

	curseMode(false); 
}	
