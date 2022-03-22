/*!
 @file info.c
 @brief infomation manager testing
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#include "m_info.h"

#include <stdio.h>

static void test(size_t n)
{
    m_info_s *ctx = m_info_new();

    for (size_t i = 0; i != n; ++i)
    {
        m_key_s *key = (m_key_s *)a_vec_push(ctx);
        if (key == 0)
        {
            continue;
        }
        m_key_ctor(key);
        char buf[0x20];
        sprintf(buf, "%zu", i);
        m_key_set_url(key, buf);
        m_key_set_text(key, buf);
        m_key_set_blob(key, buf);
    }

    m_info_delete(ctx);
}

int main(void)
{
    test(0xFF);
    return 0;
}
