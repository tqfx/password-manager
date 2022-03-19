/*!
 @file m_info.h
 @brief infomation manager
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

/* Define to prevent recursive inclusion */
#ifndef __M_INFO_H__
#define __M_INFO_H__

#include "m_key.h"
#include "a_vec.h"

A_VEC_S(m_info_s, m_key_s);

A_VEC_AT(m_info_s, m_info_at, m_key_s)

__BEGIN_DECLS

m_info_s *m_info_new(void);
void m_info_delete(m_info_s *ctx);

void m_info_ctor(m_info_s *ctx) __NONNULL_ALL;
void m_info_dtor(m_info_s *ctx) __NONNULL_ALL;

m_key_s *m_info_push(m_info_s *ctx) __NONNULL_ALL;
m_key_s *m_info_pop(m_info_s *ctx) __NONNULL_ALL;

int m_info_add(m_info_s *ctx, m_key_s *key) __NONNULL_ALL;
int m_info_del(m_info_s *ctx, m_key_s *key) __NONNULL_ALL;

__END_DECLS

/* Enddef to prevent recursive inclusion */
#endif /* __M_INFO_H__ */
