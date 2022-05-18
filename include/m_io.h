/*!
 @file m_in.h
 @brief input and output manager
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#pragma once
#ifndef __M_IO__
#define __M_IO__

#include "a/def.h"

#include <stdlib.h>
#include <stdio.h>

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

long m_io_fsize(FILE *handle);
int m_io_fread(FILE *handle, void **pdata, size_t *nbyte);
int m_io_fwrite(FILE *handle, const void *pdata, size_t nbyte);

long m_io_size(const char *fname);
int m_io_read(const char *fname, void **pdata, size_t *nbyte);
int m_io_write(const char *fname, const void *pdata, size_t nbyte);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __M_IO__ */
