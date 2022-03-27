/*!
 @file word.c
 @brief password manager testing
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#include "m_word.h"

#include <stdio.h>

static void test(size_t n)
{
    m_word_s *ctx = m_word_new();

    for (size_t i = 0; i != n; ++i)
    {
        a_str_s *str = m_word_push(ctx);
        a_str_printf(str, "%zu", i);
    }

    m_word_die(ctx);
}

int main(void)
{
    test(0xFF);
    return 0;
}
