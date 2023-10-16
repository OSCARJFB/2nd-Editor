/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include <stdlib.h>
#include "text.h"

/**
 * Find the next free memory slot in the array of nodes.
 */
text *findAndSetNode(text *head, uint32_t id, int bufferSize, int32_t ch)
{
	text *node = head;
	for(int32_t i = id; i < bufferSize; ++i)
	{
		if(!node[i].isInUse)
		{	
			node[i].next = NULL; 
			node[i].prev = NULL; 
			node[i].ch = ch; 
			return &node[i]; 
		}
	}

	return NULL; 
}


/** 
 * Add a new node to a doubly linked list. 
 * Check if the list is empty, then create the list. 
 * If not check if the node should be added at the end, middle or head(new head) of the list. 
 */
void add(text **cursor, text *newnode)
{	
	text *node = *cursor; 
	if(node == NULL)
	{
		*cursor = newnode;
	        return;	
	}

     	if(node->next == NULL && node->prev != NULL)
	{
		node->prev->next = newnode; 
	        newnode->prev = node->prev; 	
	}
	else if(node->next != NULL && node->prev != NULL)
	{
		node->next->prev = newnode;
		newnode->prev = node;
		newnode->next = node->next; 
		node->next = newnode;
	}
	else if(node->prev == NULL)
	{
		node->prev = newnode; 
		newnode->next = node; 
	}
	
	*cursor = newnode; 
}

/**
 * Deletes a node at cursor location.
 * First check if cursor is NULL, if that is the case the list is empty.
 * Else check if the node to be deleted is at the end, middle or head(new head) of the list. 
 */
uint32_t del(text **cursor)
{
	text *node = *cursor;
	if(node == NULL)
	{
		return 0; 
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
 * Increase the amount of allocated nodes.
 * Realloc according to the size of expand. 
 * Set the new nodes and return the new bufferSize.
 */
uint32_t allocateMoreNodes(text **head, uint32_t bufferSize)
{
	const uint32_t expand = 100;
	head = realloc(head, expand);
	text *node = *head; 
	for(uint32_t i = bufferSize; i < bufferSize + expand; ++i)
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
text *allocateNodesFromBuffer(int8_t *buffer, uint32_t bufferSize)
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
        for(uint32_t i = 1; i < bufferSize; ++i)
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
