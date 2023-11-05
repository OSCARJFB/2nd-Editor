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


/**
 * This function prints all characters, once within terminal view range. 
 */
static void printText(text *head, int32_t viewStart, termxy xy)
{
	clear();

	if(head == NULL)
	{
		return;
	}
	
	// Loop each item in the list, start printing to terminal once inside the view range.
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

static void setView(text **head, int32_t viewStart, int32_t view)
{
	if(*head == NULL)
	{
		return;
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
}

static text *addText(text **head, text *cursor, int32_t ch, 
		int64_t *bufferSize, int64_t id, termxy xy)
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

static text *deleteText(text **head, text* cursor, int32_t ch, 
		int64_t *id, termxy xy)
{
	if(ch != KEY_BACKSPACE)
	{
		return cursor; 
	}
	return deleteNode(head, xy.x, xy.y, id); 
}

static text *getKeyUp(text *cursor)
{
	for(; cursor->prev != NULL; cursor = cursor->prev)
	{
		if(cursor->ch == '\n')
		{
			cursor = cursor->prev;
			break;
		}
	}

	return cursor; 
}

static text *getKeyDown(text *cursor)
{
	if(cursor->next != NULL && cursor->next->next != NULL 
			&& cursor->next->ch == '\n' && cursor->next->next->ch == '\n')
	{
		cursor = cursor->next;
		return cursor; 
	}

	for(; cursor->next != NULL; cursor = cursor->next)
	{
		if(cursor->ch == '\n')
		{
			for(cursor = cursor->next; cursor->next != NULL; cursor = cursor->next)
			{	
				if(cursor->next->ch == '\n')
				{
					break; 
				}
			}
			break; 
		}
	}

	return cursor; 
}

static text *getKeyLeft(text *cursor)
{
	if(cursor != NULL && cursor->prev != NULL)
	{
		cursor = cursor->prev; 
	}
	return cursor; 
}

static text *getKeyRight(text *cursor)
{
	if(cursor != NULL && cursor->next != NULL)
	{
		cursor = cursor->next; 
	}
	

	return cursor; 
}

/**
 * Read the arrow key and set cursor.
 * If key is up or down we iterate until we find a newline.
 * else if key left, right, we take one step prev or next.
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
			cursor = getKeyUp(cursor); 		
			break;
		case KEY_DOWN:
			cursor = getKeyDown(cursor);
			break; 
		case KEY_LEFT:
			cursor = getKeyLeft(cursor); 
			break;
		case KEY_RIGHT:
			cursor = getKeyRight(cursor);
			break; 
	}

	return cursor; 
}


/**
 * Read the cursor node and update the cursor coordinates accordingly.
 */
static termxy updateCursor(text *cursor, text *head, termxy xy, int32_t ch)
{
	if(cursor == NULL || (cursor == head && head->next != NULL))
	{
		xy.x = 0;
		xy.y = 0; 
	}
	else if(ch == KEY_BACKSPACE)
	{
		switch(cursor->ch)
		{
			case '\n':
				xy.x = 0; 
				xy.y = cursor->y + 1; 
				break;
			case '\t':
				xy.x = cursor->x + 8; 
				xy.y = cursor->y;
				break;
			default:
				xy.x = cursor->x + 1; 
				xy.y = cursor->y;
				break;

		}
	}
	else
	{
		switch(cursor->ch)
		{
			case '\n':
				xy.x = 0; 
				xy.y = cursor->y + 1; 
				break;
			case '\t':
				xy.x = cursor->x + 8;
			        xy.y = cursor->y; 	
				break;
			default:
				xy.x = cursor->x + 1;
			        xy.y = cursor->y;
				break;

		}
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
		xy = updateCursor(cursor, head, xy, ch); 
		printText(head, view, xy);
	}

	curseMode(false);

	deallocateNodes(&head);
}	
