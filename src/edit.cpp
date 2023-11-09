/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include "edit.hpp"

void edit::curseMode(bool isCurse)
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
void edit::printText(text *head, int32_t viewStart, termxy xy)
{
	clear();

	if (head == nullptr)
	{
		return;
	}

	// Loop each item in the list, start printing to terminal once inside the view range.
	int32_t newLines = 0;
	for (text *node = head; node != nullptr; node = node->next)
	{
		newLines += node->ch == '\n' ? 1 : 0;
		if (viewStart < newLines)
		{
			continue;
		}

		mvwaddch(stdscr, node->y, node->x, node->ch);
	}

	move(xy.y, xy.x);

	refresh();
}

int32_t edit::getViewBounderies(void)
{
	return 0;
}

void edit::setView(text **head, int32_t viewStart, int32_t view)
{
	if (*head == nullptr)
	{
		return;
	}

	int32_t newLines, newLinesInView, x, y;
	newLines = newLinesInView = x = y = 0;

	for (text *node = *head; node != nullptr; node = node->next)
	{
		if (newLines >= viewStart)
		{
			newLinesInView += node->ch == '\n' ? 1 : 0;
			node->x = x;
			node->y = y;

			if (node->ch == '\t')
			{
				x += 8;
			}
			else
			{
				++x;
			}

			if (node->ch == '\n')
			{
				x = 0;
				++y;
			}
		}

		newLines += node->ch == '\n' ? 1 : 0;
		if (newLinesInView == view)
		{
			break;
		}
	}
}

text *edit::addText(text **head, text *cursor, int32_t ch,
					int64_t &bufferSize, int64_t id, termxy xy)
{
	if ((ch >= ' ' && ch <= '~') || (ch == '\t' || ch == '\n'))
	{
		text *newNode = findMemorySlot(*head, id, bufferSize, ch);
		if (newNode == nullptr)
		{
			bufferSize = allocateMoreNodes(head, bufferSize);
			newNode = findMemorySlot(*head, id, bufferSize, ch);
		}

		cursor = addNode(head, newNode, xy.x, xy.y);
	}

	return cursor;
}

text *edit::deleteText(text **head, text *cursor, int32_t ch,
					   int64_t &id, termxy xy)
{
	if (ch != KEY_BACKSPACE)
	{
		return cursor;
	}
	return deleteNode(head, xy.x, xy.y, id);
}

/**
 * Will set the new cursor position by updating the cursor pointer.
 */
text *edit::getKeyUp(text *cursor)
{
	for (; cursor->prev != nullptr; cursor = cursor->prev)
	{
		if (cursor->ch == '\n')
		{
			cursor = cursor->prev;
			break;
		}
	}

	return cursor;
}

/**
 * Will set the new cursor position by updating the cursor pointer.
 */
text *edit::getKeyDown(text *cursor)
{
	if (cursor->next != nullptr && cursor->next->next != nullptr &&
		cursor->next->ch == '\n' && cursor->next->next->ch == '\n')
	{
		cursor = cursor->next;
		return cursor;
	}

	for (; cursor->next != nullptr; cursor = cursor->next)
	{
		if (cursor->ch == '\n')
		{
			for (cursor = cursor->next; cursor->next != nullptr; cursor = cursor->next)
			{
				if (cursor->next->ch == '\n')
				{
					break;
				}
			}
			break;
		}
	}

	return cursor;
}

/**
 * Will set the new cursor position by updating the cursor pointer.
 */
text *edit::getKeyLeft(text *cursor)
{
	if (cursor != nullptr && cursor->prev != nullptr)
	{
		cursor = cursor->prev;
	}
	else
	{
		cursor = nullptr;
	}

	return cursor;
}

/**
 * Will set the new cursor position by updating the cursor pointer.
 */
text *edit::getKeyRight(text *cursor, text *head)
{
	if (cursor != nullptr && cursor->next != nullptr)
	{
		cursor = cursor->next;
	}
	else
	{
		cursor = head;
	}

	return cursor;
}

/**
 * Read the arrow key and set cursor.
 * If key is up or down we iterate until we find a newline.
 * else if key left, right, we take one step prev or next.
 */
text *edit::readArrowKeys(text *head, text *cursor, int32_t ch)
{
	if (head == nullptr)
	{
		return nullptr;
	}

	switch (ch)
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
		cursor = getKeyRight(cursor, head);
		break;
	}

	return cursor;
}

/**
 * Read the cursor node and update the cursor coordinates accordingly.
 */
edit::termxy edit::updateCursor(text *cursor, termxy xy, int32_t ch)
{
	if (cursor == nullptr)
	{
		xy.x = 0;
		xy.y = 0;
	}
	else if (ch == KEY_BACKSPACE)
	{
		switch (cursor->ch)
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
		switch (cursor->ch)
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

edit::edit(int8_t *buffer, int64_t bufferSize)
{

	curseMode(true);

	text *head = allocateNodesFromBuffer(buffer, bufferSize), *cursor = nullptr;
	int32_t viewStart = 0, view = getmaxy(stdscr);
	int64_t id = 0;
	termxy xy = {0, 0};

	for (int32_t ch = 0; ch != EOF; ch = getch())
	{
		getyx(stdscr, xy.y, xy.x);

		// Read user Interaction.
		cursor = addText(&head, cursor, ch, bufferSize, id, xy);
		cursor = deleteText(&head, cursor, ch, id, xy);
		cursor = readArrowKeys(head, cursor, ch);

		// Update and redraw.
		getViewBounderies();
		setView(&head, viewStart, view);
		xy = updateCursor(cursor, xy, ch);
		printText(head, view, xy);
	}

	curseMode(false);

	deallocateNodes(&head);
}