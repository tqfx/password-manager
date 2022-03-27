/*!
 @file m_key.h
 @brief key manager
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#pragma once
#ifndef __M_KEY_H__
#define __M_KEY_H__

#include "liba.h"

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

__BEGIN_DECLS

m_key_s *m_key_new(void);
void m_key_die(m_key_s *ctx);

void m_key_ctor(m_key_s *ctx) __NONNULL_ALL;
void m_key_dtor(m_key_s *ctx) __NONNULL_ALL;

int m_key_set_url(m_key_s *ctx, const void *url) __NONNULL((1));
int m_key_set_blob(m_key_s *ctx, const void *blob) __NONNULL((1));
int m_key_set_text(m_key_s *ctx, const void *text) __NONNULL((1));
void m_key_set_type(m_key_s *ctx, unsigned int type) __NONNULL((1));
void m_key_set_size(m_key_s *ctx, unsigned int size) __NONNULL((1));

int m_key_copy(m_key_s *ctx, const m_key_s *in) __NONNULL_ALL;
void m_key_move(m_key_s *ctx, m_key_s *in) __NONNULL_ALL;

__END_DECLS

#endif /* __M_KEY_H__ */
