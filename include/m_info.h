/*!
 @file m_info.h
 @brief infomation manager
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#pragma once
#ifndef __M_INFO_H__
#define __M_INFO_H__

#include "m_key.h"
#include "a_vec.h"

typedef a_vec_s m_info_s;

A_INLINE m_key_s *m_info_ptr(const m_info_s *ctx) { return (m_key_s *)a_vec_ptr(ctx); }

A_INLINE m_key_s *m_info_at(const m_info_s *ctx, size_t index) { return m_info_ptr(ctx) + index; }

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

m_info_s *m_info_new(void);
void m_info_die(m_info_s *ctx);

void m_info_ctor(m_info_s *ctx);
void m_info_dtor(m_info_s *ctx);

m_key_s *m_info_push(m_info_s *ctx);
m_key_s *m_info_pop(m_info_s *ctx);

int m_info_add(m_info_s *ctx, m_key_s *key);
int m_info_del(m_info_s *ctx, m_key_s *key);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __M_INFO_H__ */
