/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include <stdlib.h>

/**
 * Allocate a new node, set the character value and return the pointer. 
 */
static inline text *createNode(int ch)
{
	text node = *malloc(sizeof(text));
	if(node == NULL)
	{
		exit(EXIT_FAILURE); 
	}
	node->ch = ch;
	node->next = NULL; 
	node->prev = NULL; 
	return node; 
}

/** 
 * Add a new node to a doubly linked list. 
 * Check if the list is empty, then create the list. 
 * If not check if the node should be added at the end, middle or head(new head) of the list. 
 */
void add(text **cursor, int ch)
{	
	text *node = cursor, *newnode = createNode(ch); 
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

void delAll(text *head)
{
	text *node = *head, *nextNode = NULL; 
	while (node != NULL)
	{
		nextNode = node->next == NULL ? NULL : node->next; 
		free(node);
		node = NULL; 
		node = nextNode; 
	}
}
