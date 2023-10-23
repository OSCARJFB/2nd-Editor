/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include <stdlib.h>
#include "text.h"

/** 
 * Add a new node to a doubly linked list. 
 * Check if the list is empty, then create the list. 
 * If not check if the node should be added at the end, middle or head(new head) of the list. 
 */
void addNode(text **head, text *newNode, int32_t x, int32_t y)
{	
	text *node = *head;
	for(; node->next != NULL; node = node->next)
	{
		if(node->x == x && node->y == y)
		{
			break;
		}
	}
	
	if(*head == newNode) // is head node.
	{
		return; 
	}
	else
	{
		node->next = newNode;
		newNode->prev = node;
	}
}

int64_t deleteNode(text **head, int32_t x, int32_t y)
{
	text *node = *head;
	if(node == NULL)
	{
		return 0; 
	}
	
	for(; node->next != NULL; node = node->next)
	{
		if(node->x == x && node->y == y)
		{
			break;
		}
	}


	if(node->next == NULL && node->prev != NULL)
	{
	 	node->prev->next = NULL; 
	}
	else if(node->next != NULL && node->prev != NULL)
	{
		node->prev->next = node->next;
		node->next->prev = node->prev->next;	
	}
	else if(node->prev == NULL)
	{
		node->next->prev = NULL; 
	}

	node->isInUse = false; 
	return node->id; 
}

/**
 * Find the next free memory slot in the array of nodes.
 */
text *findMemorySlot(text *head, int64_t id, int64_t bufferSize, int32_t ch)
{
	if(bufferSize == 0 || head == NULL)
	{
		return NULL; 
	}

	text *node = head;
	for(int64_t i = id; i < bufferSize; ++i)
	{
		if(!node[i].isInUse)
		{	
			node[i].next = NULL; 
			node[i].prev = NULL; 
			node[i].ch = ch;
		       	node[i].isInUse = true;	
			return &node[i]; 
		}
	}

	return NULL; 
}

/**
 * Increase the amount of allocated nodes.
 * Realloc according to the size of expand. 
 * Set the new nodes and return the new bufferSize.
 */
int64_t allocateMoreNodes(text **head, int64_t bufferSize)
{
	const uint32_t expand = 10;
	*head = *head != NULL ? realloc(*head, (bufferSize + expand) * sizeof(text)) : malloc(expand * sizeof(text));
	if(head == NULL)
	{
		exit(EXIT_FAILURE);
	}

	text *node = *head; 
	for(int64_t i = bufferSize; i < bufferSize + expand; ++i)
	{
		node[i].id = i; 
		node[i].next = NULL; 
		node[i].prev = NULL; 
		node[i].isInUse = false; 
	}

	return bufferSize + expand;
}

/**
 * Allocate and set nodes from a chunk of memory.
 */
text *allocateNodesFromBuffer(int8_t *buffer, int64_t bufferSize)
{
	if(buffer == NULL || bufferSize == 0)
	{
		return NULL; 
	}

        text *node = malloc(sizeof(text) * bufferSize);
        
	if(node == NULL)
        {
                return NULL;
        }
	
	// This is the head. 
	node[0].id = 0; 
        node[0].next = NULL;
        node[0].prev = NULL;
        node[0].ch = buffer[0];
	node[0].isInUse = true; 
	
	// Set the rest of the nodes.
        for(int64_t i = 1; i < bufferSize; ++i)
        {
                node[i - 1].next = &node[i];
		node[i].id = i; 
                node[i].next = NULL;
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
void deallocateNodes(text *head)
{
	free(head); 
	head = NULL; 
}
