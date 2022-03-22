/*!
 @file m_in.c
 @brief input and output manager
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#include "m_io.h"

long m_io_fsize(FILE *handle)
{
    AASSERT(handle);
    long ret = 0;
    long seek = ftell(handle);
    if (seek < 0)
    {
        return seek;
    }
    if ((ret = fseek(handle, seek, SEEK_END), ret))
    {
        return ret;
    }
    long size = ftell(handle);
    if (size < 0)
    {
        return size;
    }
    if ((ret = fseek(handle, seek, SEEK_SET), ret))
    {
        return ret;
    }
    return size;
}

long m_io_size(const char *fname)
{
    AASSERT(fname);
    FILE *handle = fopen(fname, "rb");
    if (handle == 0)
    {
        return ~0;
    }
    long size = m_io_fsize(handle);
    fclose(handle);
    return size;
}

int m_io_fread(FILE *handle, void **pdata, size_t *nbyte)
{
    AASSERT(pdata);
    AASSERT(nbyte);
    long size = m_io_fsize(handle);
    if (size < 0)
    {
        return ~0;
    }
    *pdata = malloc((size_t)size);
    if (*pdata == 0)
    {
        return ~0;
    }
    *nbyte = fread(*pdata, 1, (size_t)size, handle);
    return 0;
}

int m_io_read(const char *fname, void **pdata, size_t *nbyte)
{
    AASSERT(fname);
    int ret = ~0;
    FILE *handle = fopen(fname, "rb");
    if (handle)
    {
        ret = m_io_fread(handle, pdata, nbyte);
        fclose(handle);
    }
    return ret;
}

int m_io_fwrite(FILE *handle, const void *pdata, size_t nbyte)
{
    AASSERT(handle);
    AASSERT(!nbyte || pdata);
    if (fwrite(pdata, 1, nbyte, handle) == nbyte)
    {
        return 0;
    }
    return ~0;
}

int m_io_write(const char *fname, const void *pdata, size_t nbyte)
{
    AASSERT(fname);
    int ret = ~0;
    FILE *handle = fopen(fname, "wb");
    if (handle)
    {
        ret = m_io_fwrite(handle, pdata, nbyte);
        fclose(handle);
    }
    return ret;
}
