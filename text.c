#include <stdlib.h>

typedef struct text
{
	int ch;
	struct text *next;
	struct text *prev;	
} text;

static inline text *createNode(int ch)
{
	// Allocate a new node, set the character value and return the pointer. 
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
 * If not check if the node should be added at the end, middle or head of the list. 
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

text **delete()
{
}

text **deleteAll()
{
}
