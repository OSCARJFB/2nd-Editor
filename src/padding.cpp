#include "padding.hpp"

int32_t padding::countLines(text *head)
{
    if(head == nullptr)
    {
        return 0; 
    }

    for(text *node = head; node->next != nullptr; node = node->next)
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
        m_padding = 3; 
    }
    else if(lines < 1000 && lines >= 100)  
    {
        m_padding = 4; 
    }
    else if(lines < 10000 && lines >= 1000)  
    {
        m_padding = 5;     
    }
    else if(lines < 100000 && lines >= 10000)  
    {
        m_padding = 6; 
    }
}