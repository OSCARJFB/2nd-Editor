#ifndef TEXT_H
#define TEXT_H

#include <stdbool.h>

typedef struct text
{
	long id; 
	int ch;
	bool isInUse; 
	struct text *next;
	struct text *prev;	
} text;

text *findMemorySlot(text *head, long id, int bufferSize, int ch);
void add(text **cursor, text *newnode);
long del(text **cursor);
int allocateMoreNodes(text **head, int bufferSize);
text *allocateNodesFromBuffer(char *buffer, int bufferSize);
void deallocateNodes(text *head);

#endif
