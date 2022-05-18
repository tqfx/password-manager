/*!
 @file m_word.c
 @brief password manager
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "m_word.h"
#include "a/oop.h"

#include <assert.h>

A_OOP_NEW(m_word_s, m_word_new, m_word_ctor)
A_OOP_DIE(m_word_s, m_word_die, m_word_dtor)

void m_word_ctor(m_word_s *ctx)
{
    assert(ctx);
    a_vec_ctor(ctx, sizeof(a_str_s));
}

void m_word_dtor(m_word_s *ctx)
{
    assert(ctx);
    a_vec_dtor(ctx, (void (*)(void *))a_str_dtor);
}

a_str_s *m_word_push(m_word_s *ctx)
{
    a_str_s *str = (a_str_s *)a_vec_push(ctx);
    if (str)
    {
        a_str_ctor(str);
    }
    return str;
}

a_str_s *m_word_pop(m_word_s *ctx)
{
    return (a_str_s *)a_vec_pop(ctx);
}

int m_word_add(m_word_s *ctx, a_str_s *str)
{
    a_str_s *p = 0;
    for (size_t i = 0; i != a_vec_len(ctx); ++i)
    {
        p = m_word_at(ctx, i);
        if (a_str_val(p) && strcmp(a_str_val(p), a_str_val(str)) == 0)
        {
            a_str_dtor(p);
            break;
        }
        p = 0;
    }
    if (p == 0)
    {
        p = m_word_push(ctx);
        if (p == 0)
        {
            return ~0;
        }
    }
    return a_str_copy(p, str);
}

int m_word_del(m_word_s *ctx, a_str_s *str)
{
    int ret = ~0;
    for (size_t i = 0; i != a_vec_len(ctx); ++i)
    {
        a_str_s *p = m_word_at(ctx, i);
        if (a_str_val(p) && strcmp(a_str_val(p), a_str_val(str)) == 0)
        {
            a_str_dtor(p);
            ret = 0;
        }
    }
    return ret;
}
