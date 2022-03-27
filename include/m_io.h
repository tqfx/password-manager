/*!
 @file m_in.h
 @brief input and output manager
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#pragma once
#ifndef __M_IO__
#define __M_IO__

#include "liba.h"

#include <stdlib.h>
#include <stdio.h>

__BEGIN_DECLS

long m_io_fsize(FILE *handle) __NONNULL_ALL;
int m_io_fread(FILE *handle, void **pdata, size_t *nbyte) __NONNULL((1));
int m_io_fwrite(FILE *handle, const void *pdata, size_t nbyte) __NONNULL((1));

long m_io_size(const char *fname) __NONNULL_ALL;
int m_io_read(const char *fname, void **pdata, size_t *nbyte) __NONNULL((1));
int m_io_write(const char *fname, const void *pdata, size_t nbyte) __NONNULL((1));

__END_DECLS

#endif /* __M_IO__ */
