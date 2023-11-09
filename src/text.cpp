/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include "text.hpp"

text *text::getNode(text *node, int32_t x, int32_t y)
{
	for(; node != nullptr; node = node->next)
	{
		if(node->x == x && node->y == y)
		{
			node = node->prev != nullptr ? node->prev : node; 
			break;
		}

		if(node->next == nullptr)
		{
			break;
		}
	}

	return node; 
}

/** 
 * Add a new node to a doubly linked list. 
 * Check if the list is empty, then create the list. 
 * If not check if the node should be added at the end, middle or head(new head) of the list. 
 */
text *text::addNode(text **head, text *newNode, int32_t x, int32_t y)
{	
	text *node = getNode(*head, x, y); 

	if(*head == newNode) 			// First node, head node.
	{
		return *head; 
	}
	else if(x == 0 && y == 0)		// At head node.
	{
		int32_t ch = (*head)->ch;
		(*head)->ch = newNode->ch; 
		newNode->ch = ch;
		
		if((*head)->next != nullptr)
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
	else if(node->next == nullptr) 		// At the end of the list.
	{
		node->next = newNode;
		newNode->prev = node;
	}
	else if(node->next != nullptr)		// At the middle of the list.
	{
		node->next->prev = newNode; 
		newNode->next = node->next; 
		newNode->prev = node;
		node->next = newNode; 
	}

	return newNode; 
}

/**
 * Unlinks a node in the list and return the id of the node.
 * This Id can be used to access the node if it should be reused. 
 */
text *text::deleteNode(text **head, int32_t x, int32_t y, int64_t &id)
{
	if(*head == nullptr || (x == 0 && y == 0))				// Nothing to delete. 
	{
		return nullptr; 
	}

	text *node = getNode(*head, x, y);
	text *newNode = node->prev;
	
	if(node->next == nullptr && node->prev != nullptr)
	{
	 	newNode->next = nullptr; 
	}
	else if(node->next != nullptr && node->prev != nullptr)
	{
		newNode->next = node->next;
	        node->next->prev = newNode; 	
	}
	else if(node->prev == nullptr && node == *head)
	{
		if(node->next == nullptr)
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

	node->isInUse = false; 
	id = node->id; 
	return newNode; 
}

/**
 * Find the next free memory slot in the array of nodes.
 */
text *text::findMemorySlot(text *head, int64_t id, int64_t bufferSize, int32_t ch)
{
	if(bufferSize == 0 || head == nullptr)
	{
		return nullptr; 
	}

	text *node = head;
	for(int64_t i = id; i < bufferSize; ++i)
	{
		if(!node[i].isInUse)
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

/**
 * Increase the amount of allocated nodes.
 * Realloc according to the size of expand. 
 * Set the new nodes and return the new bufferSize.
 */
int64_t text::allocateMoreNodes(text **head, int64_t bufferSize)
{
	if(*head == nullptr)
	{
		bufferSize = 0;  
	}
	const uint32_t expand = 10;
	*head = *head != nullptr ? (text*)std::realloc(*head, (bufferSize + expand) * sizeof(text)) 
			      : (text*)std::malloc(expand * sizeof(text));
	if(*head == nullptr)
	{
		exit(EXIT_FAILURE);
	}

	text *node = *head; 
	for(int64_t i = bufferSize; i < bufferSize + expand; ++i)
	{
		node[i].id = i; 
		node[i].next = nullptr; 
		node[i].prev = nullptr; 
		node[i].isInUse = false; 
	}

	return bufferSize + expand;
}

/**
 * Allocate and set nodes from a chunk of memory.
 */
text *text::allocateNodesFromBuffer(int8_t *buffer, int64_t bufferSize)
{
	if(buffer == nullptr || bufferSize == 0)
	{
		return nullptr; 
	}

        text *node = (text*)malloc(sizeof(text) * bufferSize);
        
	if(node == nullptr)
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
        for(int64_t i = 1; i < bufferSize; ++i)
        {
                node[i - 1].next = &node[i];
		node[i].id = i; 
                node[i].next = nullptr;
                node[i].prev = &node[i - 1];
                node[i].ch = buffer[i];
		node[i].isInUse = true; 
        }

        return node;
}

/**
 * This function will release the entire block of memory,
 * which in turn completely free all nodes, since they are bound to the same allocation.
 */
void text::deallocateNodes(text **head)
{
	free(*head); 
	*head = nullptr; 
}
