/*!
 @file m_key.h
 @brief key manager
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#pragma once
#ifndef __M_KEY_H__
#define __M_KEY_H__

#include "a/def.h"

#define M_KEY_OUTSIZ 0x20

typedef enum m_key_e
{
    M_KEY_EMAIL,
    M_KEY_DIGIT,
    M_KEY_CUSTOM,
} m_key_e;

typedef struct m_key_s
{
    char *url;
    char *blob;
    char *text;
    unsigned int type;
    unsigned int size;
} m_key_s;

#define m_key_url(ctx)  (ctx)->url
#define m_key_text(ctx) (ctx)->text
#define m_key_blob(ctx) (ctx)->blob
#define m_key_type(ctx) (ctx)->type
#define m_key_size(ctx) (ctx)->size

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

m_key_s *m_key_new(void);
void m_key_die(m_key_s *ctx);

void m_key_ctor(m_key_s *ctx);
void m_key_dtor(m_key_s *ctx);

int m_key_set_url(m_key_s *ctx, const void *url);
int m_key_set_blob(m_key_s *ctx, const void *blob);
int m_key_set_text(m_key_s *ctx, const void *text);
void m_key_set_type(m_key_s *ctx, unsigned int type);
void m_key_set_size(m_key_s *ctx, unsigned int size);

int m_key_copy(m_key_s *ctx, const m_key_s *in);
void m_key_move(m_key_s *ctx, m_key_s *in);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __M_KEY_H__ */
