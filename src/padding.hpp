#pragma once

#include <cstdint>
#include "text.hpp"

class padding : private text
{
public:
    explicit padding(text *head)
        : m_lines(countLines(head)) {}

    void updateLinecount(int32_t ch, bool isAdd);
    int32_t getPadding(void) const { return m_padding; }

private:
    int32_t countLines(text *head);
    void setPadding(int32_t lines);

private:
    int32_t m_padding = 0, m_lines = 0;
};