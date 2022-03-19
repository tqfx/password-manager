/*!
 @file m_word.h
 @brief password manager
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

/* Define to prevent recursive inclusion */
#ifndef __M_WORD_H__
#define __M_WORD_H__

#include "liba.h"
#include "a_vec.h"
#include "a_str.h"

A_VEC_S(m_word_s, a_str_s);

A_VEC_AT(m_word_s, m_word_at, a_str_s)

__BEGIN_DECLS

m_word_s *m_word_new(void);
void m_word_delete(m_word_s *ctx);

void m_word_ctor(m_word_s *ctx) __NONNULL_ALL;
void m_word_dtor(m_word_s *ctx) __NONNULL_ALL;

a_str_s *m_word_push(m_word_s *ctx) __NONNULL_ALL;
a_str_s *m_word_pop(m_word_s *ctx) __NONNULL_ALL;

int m_word_add(m_word_s *ctx, a_str_s *str) __NONNULL_ALL;
int m_word_del(m_word_s *ctx, a_str_s *str) __NONNULL_ALL;

__END_DECLS

/* Enddef to prevent recursive inclusion */
#endif /* __M_WORD_H__ */
