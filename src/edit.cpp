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

void edit::printLines(int32_t newLines, int32_t newLinesInView)
{
	for (int i = 0; i <= newLinesInView; ++i)
	{
		mvwprintw(stdscr, y + i, 0, "%d:", newLines + i + 1);
	}
}

void edit::printText(int32_t viewStart, int32_t view, const termxy &xy)
{
	clear();

	if (m_head == nullptr)
	{
		printw("%d:", 1);
	}

	// Loop each item in the list, start printing to terminal once inside the view range.
	int32_t newLines = 0, newLinesInView = 0;
	for (text *node = m_head; node != nullptr; node = node->next)
	{
		newLines += node->ch == '\n' && viewStart > newLines ? 1 : 0;
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

	printLines(newLines, newLinesInView);
	move(xy.y, xy.x);

	refresh();
}

text *edit::getViewStartNode(const padding &pad)
{
	if (m_cursor == nullptr)
	{
		return m_cursor;
	}

	text *node = m_cursor->prev;
	for (; node != nullptr; node = node->prev)
	{
		if (node->y == 0 && node->x == pad.getPadding())
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
	if (node != nullptr)
	{
		return true;
	}

	if (node->next != nullptr && node->next->ch != '\n')
	{
		return false;
	}

	return true;
}

int32_t edit::setViewStart(int32_t viewStart, int32_t viewEnd, int32_t ch, int32_t d_ch, const padding &pad)
{
	if (m_head == nullptr)
	{
		return viewStart;
	}

	if (ch == KEY_BACKSPACE && viewStart != 0 && d_ch == '\n')
	{
		--viewStart;
	}
	else if (ch == KEY_UP && isNodeAtPrevLine(m_cursor) &&
			 (m_cursor->y == 0 || m_cursor->y == -1) && viewStart != 0)
	{
		--viewStart;
	}
	else if (ch == KEY_DOWN && isNodeAtNextLine(m_cursor) && m_cursor->y == -1)
	{
		++viewStart;
	}
	else if (ch == '\n' && viewEnd == STARTNODE)
	{
		++viewStart;
	}

	return viewStart;
}

void edit::setView(int32_t viewStart, int32_t view, const padding &pad)
{
	bool isViewSet = false;
	int32_t newLines = 0, newLinesInView = 0, x = pad.getPadding(), y = 0;
	for (text *node = m_head; node != nullptr; node = node->next)
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
				x = pad.getPadding();
				++y;
			}
		}

		newLines += node->ch == '\n' ? 1 : 0;
		if (newLinesInView == view)
		{
			isViewSet = true;
			continue;
		}

		if (isViewSet && node->ch == '\n')
		{
			break;
		}
	}
}

text *edit::addText(int32_t ch, uint32_t &bufferSize, uint32_t currentId, const termxy &xy, const padding &pad)
{
	if ((ch >= ' ' && ch <= '~') || (ch == '\t' || ch == '\n'))
	{
		text *newNode = findMemorySlot(m_head, currentId, bufferSize, ch);
		if (newNode == nullptr)
		{
			bufferSize = allocateMoreNodes(&m_head, bufferSize);
			newNode = findMemorySlot(m_head, currentId, bufferSize, ch);
			m_head = m_head == nullptr ? newNode : m_head;
		}

		return addNode(&m_head, newNode, xy.x, xy.y, pad.getPadding());
	}

	return m_cursor;
}

text *edit::deleteText(int32_t ch, int32_t &d_ch, uint32_t &currentId, const termxy &xy, const padding &pad)
{
	if (ch != KEY_BACKSPACE)
	{
		return m_cursor;
	}

	// If head is not NULL return
	if (m_head != nullptr)
	{
		// If the cursor is NULL deleted character is a newline (end of view).
		d_ch = m_cursor != nullptr ? m_cursor->ch : '\n';
	}

	return deleteNode(&m_head, xy.x, xy.y, currentId, pad.getPadding());
}

text *edit::getKeyUp(void)
{
	// beginning of the list or end of terminal view.
	if (m_cursor == nullptr || (m_cursor->y == 0 && m_cursor->ch != '\n'))
	{
		return m_cursor;
	}

	if (m_cursor->prev == nullptr && m_cursor->ch == '\n')
	{
		return nullptr;
	}

	for (; m_cursor->prev != nullptr; m_cursor = m_cursor->prev)
	{
		if (m_cursor->ch == '\n')
		{
			m_cursor = m_cursor->prev;
			break;
		}
	}

	return m_cursor;
}

text *edit::getKeyDown(void)
{
	if (m_cursor == nullptr)
	{
		m_cursor = m_head;
		if (m_head == nullptr)
		{
			return nullptr;
		}
	}

	if (m_cursor->next != nullptr && m_cursor->next->next != nullptr &&
		m_cursor->next->ch == '\n' && m_cursor->next->next->ch == '\n')
	{
		m_cursor = m_cursor->next;
		return m_cursor;
	}

	for (; m_cursor->next != nullptr; m_cursor = m_cursor->next)
	{
		if (m_cursor->ch == '\n')
		{
			for (m_cursor = m_cursor->next; m_cursor->next != nullptr; m_cursor = m_cursor->next)
			{
				if (m_cursor->next->ch == '\n')
				{
					break;
				}
			}
			break;
		}
	}

	return m_cursor;
}

text *edit::getKeyLeft(void)
{
	if (m_cursor != nullptr && m_cursor->prev != nullptr)
	{
		m_cursor = m_cursor->prev;
	}
	else
	{
		m_cursor = nullptr;
	}

	return m_cursor;
}

text *edit::getKeyRight(void)
{
	if (m_cursor != nullptr && m_cursor->next != nullptr)
	{
		m_cursor = m_cursor->next;
	}
	else if (m_cursor == nullptr)
	{
		m_cursor = m_head;
	}

	return m_cursor;
}

text *edit::readArrowKeys(int32_t ch)
{
	if (m_head == nullptr)
	{
		return nullptr;
	}

	switch (ch)
	{
	case KEY_UP:
		m_cursor = getKeyUp();
		break;
	case KEY_DOWN:
		m_cursor = getKeyDown();
		break;
	case KEY_LEFT:
		m_cursor = getKeyLeft();
		break;
	case KEY_RIGHT:
		m_cursor = getKeyRight();
		break;
	}

	return m_cursor;
}

void edit::updateCursor(termxy &xy, const padding &pad)
{
	if (m_cursor == nullptr)
	{
		xy.x = pad.getPadding();
		xy.y = 0;
	}
	else
	{
		switch (m_cursor->ch)
		{
		case '\n':
			xy.x = pad.getPadding();
			xy.y = m_cursor->y + 1;
			break;
		case '\t':
			xy.x = m_cursor->x + 8;
			xy.y = m_cursor->y;
			break;
		default:
			xy.x = m_cursor->x + 1;
			xy.y = m_cursor->y;
			break;
		}
	}
}

void edit::updatePadding(padding &pad, int32_t ch)
{
	if ((ch >= ' ' && ch <= '~') || (ch == '\t' || ch == '\n'))
	{
		pad.updateLinecount(ch, true);
	}
	else if (ch == KEY_BACKSPACE)
	{
		pad.updateLinecount(ch, false);
	}
}

void edit::run(void)
{
	int32_t viewStart = 0, viewEnd = getmaxy(stdscr), d_ch = 0;
	uint32_t bufferSize = m_bufferSize, currentId = 0;
	padding pad = padding(m_head);
	termxy xy = {pad.getPadding(), 0};

	for (int32_t ch = 0; ch != EOF && ch != KEY_ESCAPE; ch = getch())
	{
		// Set the current padding which forms the left bound border.
		updatePadding(pad, ch);

		// Read user Interaction.
		m_cursor = addText(ch, bufferSize, currentId, xy, pad);
		m_cursor = deleteText(ch, d_ch, currentId, xy, pad);
		m_cursor = readArrowKeys(ch);

		// Update and redraw.
		viewStart = setViewStart(viewStart, viewEnd, ch, d_ch, pad);
		setView(viewStart, viewEnd, pad);
		updateCursor(xy, pad);
		printText(viewStart, viewEnd, xy);
	}
}