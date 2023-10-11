/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include <stdlib.h>
#include "text.h"

/**
 * Find next free memory slot in the array of nodes.
 */
static inline text *findMemorySlot(text *head, int index, int bufferSize)
{
	text *node = head; 
	for(int i = index; bufferSize; ++i)
	{
		if(node[i].next == NULL && node[i].prev == NULL)
		{
			return &node; 
		}
	}

	return NULL; 
}

/** 
 * Add a new node to a doubly linked list. 
 * Check if the list is empty, then create the list. 
 * If not check if the node should be added at the end, middle or head(new head) of the list. 
 */
void add(text **cursor, text *newnode, int ch)
{	
	text *node = cursor; 
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
void del(text **cursor)
{
	text *node = *cursor;
	if(node == NULL)
	{
		return NULL; 
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

	free(node);
	node = NULL;
}

/**
 * Increase the size of our node pool
 */
text *allocMoreNodes(text *head)
{
	const int size = 100;
	head = realloc(head, size);
	return head;
}

text *allocateNodesFromBuffer(char *buffer, int bufferSize)
{
        text *node = malloc(sizeof(text) * bufferSize);
        if(node == NULL)
        {
                return NULL;
        }

        node[0].next = NULL;
        node[0].prev = NULL;
        node[0].ch = buffer[0];


        for(int i = 1; i < bufferSize; ++i)
        {
                node[i - 1].next = &node[i];
                node[i].next = NULL;
                node[i].prev = &node[i - 1];
                node[i].ch = buffer[i];
        }

        return node;
}
