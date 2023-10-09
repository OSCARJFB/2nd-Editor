#ifndef TEXT_H
#define TEXT_H

typedef struct text
{
	int ch;
	struct text *next;
	struct text *prev;	
} text;

void add(text **cursor, int ch);
void del(text **cursor); 
void delAll(text *head);

#endif
