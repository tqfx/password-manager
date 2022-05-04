/*!
 @file m_word.h
 @brief password manager
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#pragma once
#ifndef __M_WORD_H__
#define __M_WORD_H__

#include "liba.h"
#include "a_vec.h"
#include "a_str.h"

typedef a_vec_s m_word_s;

A_INLINE a_str_s *m_word_ptr(const m_word_s *ctx) { return (a_str_s *)a_vec_ptr(ctx); }

A_INLINE a_str_s *m_word_at(const m_word_s *ctx, size_t index) { return m_word_ptr(ctx) + index; }

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

m_word_s *m_word_new(void);
void m_word_die(m_word_s *ctx);

void m_word_ctor(m_word_s *ctx);
void m_word_dtor(m_word_s *ctx);

a_str_s *m_word_push(m_word_s *ctx);
a_str_s *m_word_pop(m_word_s *ctx);

int m_word_add(m_word_s *ctx, a_str_s *str);
int m_word_del(m_word_s *ctx, a_str_s *str);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __M_WORD_H__ */
