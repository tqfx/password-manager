/*!
 @file m_generate.h
 @brief generate manager
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#pragma once
#ifndef __M_GENERATE_H__
#define __M_GENERATE_H__

#include "m_key.h"

#include <stdlib.h>

__BEGIN_DECLS

void m_generate_setrule(const void *s0, const void *s1, const void *s2, const void *s3);

int m_generate1(const m_key_s *key, const char *word, char **out) __NONNULL_ALL;
int m_generate2(const m_key_s *key, const char *word, char **out) __NONNULL_ALL;

__END_DECLS

__NONNULL_ALL
__STATIC_INLINE
int m_generate(const m_key_s *key, const char *word, char **out)
{
    return m_generate1(key, word, out);
}

#endif /* __M_GENERATE_H__ */
