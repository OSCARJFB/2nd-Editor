/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#include "padding.hpp"

int32_t padding::countLines(text *head)
{
    m_lines = 0; 
    for(text *node = head; node != nullptr && node->next != nullptr; node = node->next)
    {
        m_lines += node->ch == '\n' ? 1 : 0; 
    }
    setPadding(m_lines); 
    return m_lines;
}

void padding::updateLinecount(int32_t ch, bool isAdd)
{   
    m_lines += ch == '\n' && isAdd ? 1 : -1; 
    setPadding(m_lines); 
}

void padding::setPadding(int32_t lines)
{
    if(lines <= 100)
    {
        m_padding = 4; 
    }
    else if(lines < 1000 && lines >= 100)  
    {
        m_padding = 6; 
    }
    else if(lines < 10000 && lines >= 1000)  
    {
        m_padding = 8;     
    }
    else if(lines < 100000 && lines >= 10000)  
    {
        m_padding = 10; 
    }
}