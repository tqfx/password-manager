/*!
 @file m_info.c
 @brief infomation manager
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "m_info.h"
#include "a/oop.h"

#include <assert.h>
#include <string.h>

A_OOP_NEW(m_info_s, m_info_new, m_info_ctor)
A_OOP_DIE(m_info_s, m_info_die, m_info_dtor)

void m_info_ctor(m_info_s *ctx)
{
    assert(ctx);
    a_vec_ctor(ctx, sizeof(m_key_s));
}

void m_info_dtor(m_info_s *ctx)
{
    assert(ctx);
    a_vec_dtor(ctx, (void (*)(void *))m_key_dtor);
}

m_key_s *m_info_push(m_info_s *ctx)
{
    m_key_s *key = (m_key_s *)a_vec_push(ctx);
    if (key)
    {
        m_key_ctor(key);
    }
    return key;
}

m_key_s *m_info_pop(m_info_s *ctx)
{
    return (m_key_s *)a_vec_pop(ctx);
}

int m_info_add(m_info_s *ctx, m_key_s *key)
{
    m_key_s *p = 0;
    for (size_t i = 0; i != a_vec_len(ctx); ++i)
    {
        p = m_info_at(ctx, i);
        if (m_key_text(p) && strcmp(m_key_text(p), m_key_text(key)) == 0)
        {
            m_key_dtor(p);
            break;
        }
        p = 0;
    }
    if (p == 0)
    {
        p = m_info_push(ctx);
        if (p == 0)
        {
            return ~0;
        }
    }
    return m_key_copy(p, key);
}

int m_info_del(m_info_s *ctx, m_key_s *key)
{
    int ret = ~0;
    for (size_t i = 0; i != a_vec_len(ctx); ++i)
    {
        m_key_s *p = m_info_at(ctx, i);
        if (m_key_text(p) && strcmp(m_key_text(p), m_key_text(key)) == 0)
        {
            m_key_dtor(p);
            ret = 0;
        }
    }
    return ret;
}
