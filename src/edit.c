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

static void printText(text *head, int32_t viewStart, termxy xy)
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

	move(xy.y, xy.x);

	refresh(); 
}

static int32_t getViewBounderies(void)
{
	return 0;
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

static text *addText(text **head, text *cursor, int32_t ch, int64_t *bufferSize, int64_t id, termxy xy)
{
	if((ch >= ' ' && ch <= '~') || (ch == '\t' || ch == '\n'))
	{
		text *newNode = findMemorySlot(*head, id, *bufferSize, ch);
		if(newNode == NULL)
		{
			*bufferSize = allocateMoreNodes(head, *bufferSize);
			newNode = findMemorySlot(*head, id, *bufferSize, ch);
		}

		cursor = addNode(head, newNode, xy.x, xy.y);
	}

	return cursor;
}

static text *deleteText(text **head, text* cursor, int32_t ch, int64_t *id, termxy xy)
{
	if(ch != KEY_BACKSPACE)
	{
		return cursor; 
	}
	return deleteNode(head, xy.x, xy.y, id); 
}

/**
 * Read the arrow key and set cursor.
 * If key is up or down we iterate until we find a newline.
 * else f key left, right, we take one step prev or next.
 */
static text *readArrowKeys(text *head, text *cursor, int32_t ch)
{
	if(head == NULL)
	{
		return NULL; 
	}
		
	switch(ch)
	{
		case KEY_UP:
			 
			for(bool isOnNewLine = false; cursor->prev != NULL && !isOnNewLine; cursor = cursor->prev)
			{
				if(cursor->ch == '\n')
				{
					isOnNewLine = true; 
				}
			}
			break;
		case KEY_DOWN:
			for(bool isOnNewLine = false; cursor->next != NULL && !isOnNewLine; cursor = cursor->next)
			{
				if(cursor->ch == '\n')
				{
					isOnNewLine = true;
				}
			}
			break; 
		case KEY_LEFT:
			if(cursor->next != NULL)
			{
				cursor = cursor->prev->ch != '\n' ? cursor->prev : cursor;
		       	}
			break;
		case KEY_RIGHT:
			if(cursor->next != NULL)
			{
				cursor = cursor->next->ch != '\n' ? cursor->next : cursor;
		       	}	
			break; 
	}

	return cursor; 
}


/**
 * Read the cursor node and update the cursor coordinates accordingly.
 */
static termxy updateCursor(text *cursor, termxy xy, int32_t ch)
{
	if(cursor == NULL)
	{
		xy.x = 0;
		xy.y = 0; 
	}
	else
	{
		xy.x = ch == '\n' ? 0 : cursor->x + 1;  
		xy.y = ch == '\n' ? cursor->y + 1 : cursor->y;  
	}

	return xy; 
}

void edit(text *head, int64_t bufferSize)
{
	curseMode(true); 

	text *cursor = NULL; 
	int32_t viewStart = 0, view = getmaxy(stdscr);
	int64_t id = 0; 
	termxy xy = {0, 0};

	for(int32_t ch = 0; ch != EOF; ch = getch())
	{
		getyx(stdscr, xy.y, xy.x); 

		// Read user Interaction.  
		cursor = addText(&head, cursor, ch, &bufferSize, id, xy);
		cursor = deleteText(&head, cursor, ch, &id, xy); 
		cursor = readArrowKeys(head, cursor, ch); 

		// Update and redraw.
		getViewBounderies();
		setView(&head, viewStart, view);
		xy = updateCursor(cursor, xy, ch); 
		printText(head, view, xy);
	}

	curseMode(false); 
}	
