/*!
 @file m_word.c
 @brief password manager
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#include "m_word.h"
#include "a_object.h"

A_OBJECT_CREATE(m_word_s, m_word_new, m_word_ctor)
A_OBJECT_DELETE(m_word_s, m_word_delete, m_word_dtor)

__STATIC_INLINE
void *m_word_virtual_address(void *obj, size_t index)
{
    m_word_s *ctx = (m_word_s *)obj;
    return ctx->ptr + index;
}

__STATIC_INLINE
int m_word_virtual_realloc(void *obj, size_t capacity)
{
    m_word_s *ctx = (m_word_s *)obj;
    a_str_s *ptr = (a_str_s *)realloc(ctx->ptr, sizeof(a_str_s) * capacity);
    return ptr ? (ctx->ptr = ptr, 0) : ~0;
}

void m_word_ctor(m_word_s *ctx)
{
    AASSERT(ctx);
    static const a_vec_vtbl_s vtbl = {
        m_word_virtual_address,
        m_word_virtual_realloc,
    };
    a_vec_ctor(ctx->vec);
    ctx->vec->vptr = &vtbl;
    ctx->ptr = 0;
}

void m_word_dtor(m_word_s *ctx)
{
    AASSERT(ctx);
    while (a_vec_len(ctx->vec))
    {
        size_t index = a_vec_dec(ctx->vec);
        a_str_s *str = (a_str_s *)a_vec_ptr(ctx, index);
        a_str_dtor(str);
    }
    a_vec_dtor(ctx->vec);
    free(ctx->ptr);
    ctx->ptr = 0;
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
        p = A_VEC_PTR(ctx, i);
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
        a_str_s *p = A_VEC_PTR(ctx, i);
        if (a_str_val(p) && strcmp(a_str_val(p), a_str_val(str)) == 0)
        {
            a_str_dtor(p);
            ret = 0;
        }
    }
    return ret;
}
