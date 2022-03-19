/*!
 @file m_key.c
 @brief key manager
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#include "m_key.h"

#include "a_object.h"

#include <string.h>

A_OBJECT_CREATE(m_key_s, m_key_new, m_key_ctor)
A_OBJECT_DELETE(m_key_s, m_key_delete, m_key_dtor)

void m_key_ctor(m_key_s *ctx)
{
    AASSERT(ctx);
    ctx->url = 0;
    ctx->blob = 0;
    ctx->text = 0;
    ctx->type = M_KEY_EMAIL;
    ctx->size = M_KEY_OUTSIZ >> 1;
}

void m_key_dtor(m_key_s *ctx)
{
    AASSERT(ctx);
    ctx->url ? (free(ctx->url), ctx->url = 0) : 0;
    ctx->blob ? (free(ctx->blob), ctx->blob = 0) : 0;
    ctx->text ? (free(ctx->text), ctx->text = 0) : 0;
    ctx->type = M_KEY_EMAIL;
    ctx->size = M_KEY_OUTSIZ >> 1;
}

#undef M_KEY_SET_STR
#define M_KEY_SET_STR(field)                                       \
    int m_key_set_##field(m_key_s *ctx, const void *field)         \
    {                                                              \
        AASSERT(ctx);                                              \
        char *str = 0;                                             \
        if (field)                                                 \
        {                                                          \
            str = (char *)malloc(strlen((const char *)field) + 1); \
            if (str == 0)                                          \
            {                                                      \
                return ~0;                                         \
            }                                                      \
            strcpy(str, (const char *)field);                      \
        }                                                          \
        if (ctx->field)                                            \
        {                                                          \
            free(ctx->field);                                      \
        }                                                          \
        ctx->field = str;                                          \
        return 0;                                                  \
    }
M_KEY_SET_STR(url)
M_KEY_SET_STR(blob)
M_KEY_SET_STR(text)
#undef M_KEY_SET_STR

void m_key_set_type(m_key_s *ctx, unsigned int type)
{
    AASSERT(ctx);
    ctx->type = type;
}

void m_key_set_size(m_key_s *ctx, unsigned int size)
{
    AASSERT(ctx);
    ctx->size = size < M_KEY_OUTSIZ ? size : M_KEY_OUTSIZ;
}

int m_key_copy(m_key_s *ctx, const m_key_s *in)
{
    AASSERT(in);
    AASSERT(ctx);
    if (m_key_set_url(ctx, m_key_url(in)))
    {
        return ~0;
    }
    if (m_key_set_text(ctx, m_key_text(in)))
    {
        return ~0;
    }
    if (m_key_set_blob(ctx, m_key_blob(in)))
    {
        return ~0;
    }
    m_key_set_type(ctx, m_key_type(in));
    m_key_set_size(ctx, m_key_size(in));
    return 0;
}

void m_key_move(m_key_s *ctx, m_key_s *in)
{
    AASSERT(in);
    AASSERT(ctx);
    memcpy(ctx, in, sizeof(m_key_s));
    memset(in, 0, sizeof(m_key_s));
}
