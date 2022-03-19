/*!
 @file m_info.c
 @brief infomation manager
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#include "m_info.h"
#include "a_object.h"

#include <string.h>

A_OBJECT_CREATE(m_info_s, m_info_new, m_info_ctor)
A_OBJECT_DELETE(m_info_s, m_info_delete, m_info_dtor)

__STATIC_INLINE
void *m_info_virtual_address(void *obj, size_t index)
{
    m_info_s *ctx = (m_info_s *)obj;
    return ctx->ptr + index;
}

__STATIC_INLINE
int m_info_virtual_realloc(void *obj, size_t capacity)
{
    m_info_s *ctx = (m_info_s *)obj;
    m_key_s *ptr = (m_key_s *)realloc(ctx->ptr, sizeof(m_key_s) * capacity);
    return ptr ? (ctx->ptr = ptr, 0) : ~0;
}

void m_info_ctor(m_info_s *ctx)
{
    AASSERT(ctx);
    static const a_vec_vtbl_s vtbl = {
        m_info_virtual_address,
        m_info_virtual_realloc,
    };
    a_vec_ctor(ctx->vec);
    ctx->vec->vptr = &vtbl;
    ctx->ptr = 0;
}

void m_info_dtor(m_info_s *ctx)
{
    AASSERT(ctx);
    while (a_vec_len(ctx->vec))
    {
        size_t index = a_vec_dec(ctx->vec);
        m_key_s *key = (m_key_s *)a_vec_ptr(ctx, index);
        m_key_dtor(key);
    }
    a_vec_dtor(ctx->vec);
    free(ctx->ptr);
    ctx->ptr = 0;
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
        p = A_VEC_PTR(ctx, i);
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
        m_key_s *p = A_VEC_PTR(ctx, i);
        if (m_key_text(p) && strcmp(m_key_text(p), m_key_text(key)) == 0)
        {
            m_key_dtor(p);
            ret = 0;
        }
    }
    return ret;
}
