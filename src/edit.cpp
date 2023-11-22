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

void edit::printText(text *head, int32_t viewStart, int32_t view, termxy xy)
{
	clear();

	// Loop each item in the list, start printing to terminal once inside the view range.
	int32_t newLines = 0, newLinesInView = 0;
	for (text *node = head; node != nullptr; node = node->next)
	{
		newLines += node->ch == '\n' ? 1 : 0;
		if (viewStart > newLines)
		{
			continue;
		}

		newLinesInView += node->ch == '\n' ? 1 : 0;
		if (newLinesInView > view)
		{
			break;
		}

		mvwaddch(stdscr, node->y, node->x, node->ch);
	}

	move(xy.y, xy.x);

	refresh();
}

text *edit::getViewStartNode(text *cursor)
{
	if (cursor == nullptr)
	{
		return cursor;
	}

	text *node = cursor->prev;
	for (; node != nullptr; node = node->prev)
	{
		if (node->y == 0 && node->x == 0)
		{
			break;
		}
	}

	return node;
}

int32_t edit::getNewLinesInView(text *node, int32_t view)
{
	int32_t lines = 0;
	for (; node != nullptr; node = node->next)
	{
		lines += node->ch == '\n' ? 1 : 0;
		if (lines == view)
		{
			break;
		}
	}

	return lines;
}

bool edit::isNodeAtPrevLine(text *node)
{
	if (node == nullptr)
	{
		return true;
	}

	bool nodeFound = false;
	for (; node != nullptr; node = node->prev)
	{
		if (node->y == -1)
		{
			nodeFound = true;
			break;
		}
	}

	return nodeFound;
}

bool edit::isNodeAtNextLine(text *node)
{
	if (node == nullptr)
	{
		return true;
	}

	if (node->next == nullptr || node->next->ch != '\n')
	{
		return false;
	}

	return true;
}

int32_t edit::setViewStart(int32_t view, int32_t viewStart, text *head,
						   text *cursor, int32_t ch, int32_t delch)
{
	if (head == nullptr)
	{
		return viewStart;
	}

	else if (ch == KEY_BACKSPACE && viewStart != 0 && delch == '\n')
	{
		return --viewStart;
	}

	if (ch == KEY_UP && isNodeAtPrevLine(cursor) && cursor->y == 0)
	{
		return --viewStart;
	}

	if (ch == KEY_DOWN && isNodeAtNextLine(cursor) && cursor->y == -1)
	{
		return ++viewStart;
	}

	text *startNode = cursor != nullptr ? getViewStartNode(cursor) : head;
	if (ch == '\n' && view == getNewLinesInView(startNode, view))
	{
		return ++viewStart;
	}

	return viewStart;
}

void edit::setView(text **head, int32_t viewStart, int32_t view)
{
	bool isViewSet = false;
	int32_t newLines = 0, newLinesInView = 0, x = 0, y = 0;
	for (text *node = *head; node != nullptr; node = node->next)
	{
		node->x = node->y = -1; // outside of view.
		if (newLines >= viewStart && !isViewSet)
		{
			newLinesInView += node->ch == '\n' ? 1 : 0;
			node->x = x;
			node->y = y;

			x += node->ch == '\t' ? 8 : 1;
			if (node->ch == '\n')
			{
				x = 0;
				++y;
			}
		}
		
		newLines += node->ch == '\n' ? 1 : 0;
		if (newLinesInView == view)
		{
			isViewSet = true;
			continue;
		}

		if(isViewSet && node->ch == '\n')
		{
			break; 
		}
	}
}

text *edit::addText(text **head, text *cursor, int32_t ch,
					size_t &bufferSize, size_t id, termxy xy)
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
					   int32_t &delch, size_t &id, termxy xy)
{
	if (ch != KEY_BACKSPACE)
	{
		return cursor;
	}

	// If head is not NULL return
	if (head != nullptr)
	{
		// If the cursor is NULL deleted character is a newline (end of view).
		delch = cursor != nullptr ? cursor->ch : '\n';
	}

	return deleteNode(head, xy.x, xy.y, id);
}

text *edit::getKeyUp(text *cursor)
{
	// beginning of the list or end of terminal view.
	if (cursor == nullptr || (cursor->y == 0 && cursor->ch != '\n'))
	{
		return cursor;
	}

	if (cursor->prev == nullptr && cursor->ch == '\n')
	{
		return nullptr;
	}

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

text *edit::getKeyDown(text *cursor, text *head)
{
	if (cursor == nullptr)
	{
		cursor = head;
		if (head == nullptr)
		{
			return nullptr;
		}
	}

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

text *edit::getKeyRight(text *cursor, text *head)
{
	if (cursor != nullptr && cursor->next != nullptr)
	{
		cursor = cursor->next;
	}
	else if (cursor == nullptr)
	{
		cursor = head;
	}

	return cursor;
}

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
		cursor = getKeyDown(cursor, head);
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

edit::termxy edit::updateCursor(text *cursor, termxy xy)
{
	if (cursor == nullptr)
	{
		xy.x = 0;
		xy.y = 0;
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

edit::edit(std::string &buffer, size_t bufferSize)
{

	curseMode(true);

	text *head = allocateNodesFromBuffer(buffer, bufferSize), *cursor = nullptr;
	int32_t viewStart = 0, view = 3, delch = 0;
	size_t id = 0;
	termxy xy = {0, 0};

	for (int32_t ch = 0; ch != EOF; ch = getch())
	{
		getyx(stdscr, xy.y, xy.x);

		// Read user Interaction.
		cursor = addText(&head, cursor, ch, bufferSize, id, xy);
		cursor = deleteText(&head, cursor, ch, delch, id, xy);
		cursor = readArrowKeys(head, cursor, ch);

		// Update and redraw.
		viewStart = setViewStart(view, viewStart, head, cursor, ch, delch);
		setView(&head, viewStart, view);
		xy = updateCursor(cursor, xy);
		printText(head, viewStart, view, xy);
	}

	curseMode(false);

	deallocateNodes(&head);
}