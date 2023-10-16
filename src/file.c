/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include "file.h"

static FILE *getFileFromArg(int32_t argc, int8_t **argv)
{
	if(argc < 2)
	{
		return NULL;
	}

	FILE *fp = fopen((char*)argv[1], "r");
	if(fp == NULL)
	{
		return NULL; 
	}

	return fp;
}

static int64_t getFileSize(FILE *fp)
{
	if(fp == NULL)
	{
		return 0;
	}

	int64_t size = 0;
        if(fseek(fp, 0, SEEK_END) == -1)
	{
		return -1; 
	}	

	size = ftell(fp);
	if(size == -1)
	{
		return -1;
	}

	return size; 
}

static int8_t *createBuffer(int64_t bufferSize, FILE *fp)
{
	if(bufferSize == 0)
	{
		return NULL; 
	}

	int8_t *buffer = malloc(bufferSize);
	
	if(buffer == NULL)
	{
		exit(EXIT_FAILURE);
	}

	rewind(fp);
	while(fread(buffer, bufferSize, 1, fp) > 0){}; 
	return buffer;	
}

void startApp(int32_t argc, int8_t **argv)
{
	FILE *fp = getFileFromArg((int32_t)argc, (int8_t **)argv);
	int64_t bufferSize = getFileSize(fp);
	int8_t *buffer = createBuffer(bufferSize, fp);
	text *head = allocateNodesFromBuffer(buffer, bufferSize);
	edit(head);
}

