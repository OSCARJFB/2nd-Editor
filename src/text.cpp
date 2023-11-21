/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include "text.hpp"

text *text::getNode(text *node, int32_t x, int32_t y)
{
	for (; node != nullptr; node = node->next)
	{
		if (node->x == x && node->y == y)
		{
			node = node->prev != nullptr ? node->prev : node;
			break;
		}

		if (node->next == nullptr)
		{
			break;
		}
	}

	return node;
}

text *text::addNode(text **head, text *newNode, int32_t x, int32_t y)
{
	text *node = getNode(*head, x, y);
	if (*head == newNode) // First node, head node.
	{
		return *head;
	}
	else if (x == 0 && y == 0 && node->prev == nullptr) // At head node.
	{
		int32_t ch = (*head)->ch;
		(*head)->ch = newNode->ch;
		newNode->ch = ch;

		if ((*head)->next != nullptr)
		{
			(*head)->next->prev = newNode;
			newNode->next = (*head)->next;

			(*head)->next = newNode;
			newNode->prev = (*head);
		}
		else
		{
			newNode->prev = *head;
			(*head)->next = newNode;
		}
		return *head;
	}
	else if (node->next == nullptr) // At the end of the list.
	{
		node->next = newNode;
		newNode->prev = node;
	}
	else if (node->next != nullptr) // At the middle of the list.
	{
		node->next->prev = newNode;
		newNode->next = node->next;
		newNode->prev = node;
		node->next = newNode;
	}

	return newNode;
}

text *text::deleteNode(text **head, int32_t x, int32_t y, size_t &id)
{
	if (*head == nullptr || (x == 0 && y == 0)) // Nothing to delete.
	{
		return nullptr;
	}

	text *node = getNode(*head, x, y);
	text *newNode = node->prev;

	if (node->next == nullptr && node->prev != nullptr) // Remove at the end of the list.
	{
		newNode->next = nullptr;
	}
	else if (node->next != nullptr && node->prev != nullptr) // Remove somewhere at the middle of the list.
	{
		newNode->next = node->next;
		node->next->prev = newNode;
	}
	else if (node->prev == nullptr && node == *head) // Remove or swap head.
	{
		if (node->next == nullptr)
		{
			deallocateNodes(head);
			id = 0;
			return nullptr;
		}
		else
		{
			(*head)->ch = node->next->ch;
			node = node->next;
			(*head)->next = (*head)->next->next != nullptr ? (*head)->next->next : nullptr;
		}

		newNode = nullptr;
	}

	// decommission the node and update the current id.
	node->isInUse = false;
	id = node->id;
	return newNode;
}

text *text::findMemorySlot(text *head, size_t id, size_t bufferSize, int32_t ch)
{
	if (bufferSize == 0 || head == nullptr)
	{
		return nullptr;
	}

	// Use id to get quick access to a memory slot else continue looping.
	text *node = head;
	for (size_t i = id; i < bufferSize; ++i)
	{
		if (!node[i].isInUse)
		{
			node[i].next = nullptr;
			node[i].prev = nullptr;
			node[i].ch = ch;
			node[i].isInUse = true;
			return &node[i];
		}
	}

	return nullptr;
}

size_t text::allocateMoreNodes(text **head, size_t bufferSize)
{
	if (*head == nullptr)
	{
		bufferSize = 0;
	}
	// Allocate or reallocate memory of the text list.
	const uint32_t expand = 10;
	*head = *head != nullptr ? (text *)std::realloc(*head, (bufferSize + expand) * sizeof(text))
							 : (text *)std::malloc(expand * sizeof(text));
	if (*head == nullptr)
	{
		exit(EXIT_FAILURE);
	}

	// Set all newly allocated nodes.
	text *node = *head;
	for (size_t i = bufferSize; i < bufferSize + expand; ++i)
	{
		node[i].id = i;
		node[i].next = nullptr;
		node[i].prev = nullptr;
		node[i].isInUse = false;
	}

	return bufferSize + expand;
}

text *text::allocateNodesFromBuffer(std::string &buffer, size_t bufferSize)
{
	if (buffer.empty())
	{
		return nullptr;
	}

	text *node = (text *)malloc(sizeof(text) * bufferSize);

	if (node == nullptr)
	{
		return nullptr;
	}

	// This is the head.
	node[0].id = 0;
	node[0].next = nullptr;
	node[0].prev = nullptr;
	node[0].ch = buffer[0];
	node[0].isInUse = true;

	// Set the rest of the nodes.
	for (size_t i = 1; i < bufferSize; ++i)
	{
		node[i - 1].next = &node[i];
		node[i].id = i;
		node[i].next = nullptr;
		node[i].prev = &node[i - 1];
		node[i].ch = buffer[i];
		node[i].isInUse = true;
	}

	buffer = std::string();
	return node;
}

void text::deallocateNodes(text **head)
{
	free(*head);
	*head = nullptr;
}
