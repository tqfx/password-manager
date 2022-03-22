/*!
 @file m_json.c
 @brief json manager
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#include "m_json.h"

#include "m_io.h"

int m_json_load(cJSON **out, const char *in)
{
    AASSERT(in);
    AASSERT(out);
    int ret = ~0;
    void *pdata = 0;
    size_t nbyte = 0;
    ret = m_io_read(in, &pdata, &nbyte);
    if (ret == 0)
    {
        *out = cJSON_ParseWithLength((char *)pdata, nbyte);
        free(pdata);
    }
    return ret;
}

int m_json_export_info(const cJSON *in, m_info_s *out)
{
    AASSERT(in);
    AASSERT(out);
    m_key_s key[1];
    cJSON *object;
    int n = cJSON_GetArraySize(in);
    for (int i = 0; i != n; ++i)
    {
        m_key_ctor(key);
        cJSON *item = cJSON_GetArrayItem(in, i);

        object = cJSON_GetObjectItem(item, "text");
        if (object == 0)
        {
            continue;
        }
        m_key_text(key) = cJSON_GetStringValue(object);

#ifndef _MSC_VER
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbad-function-cast"
#endif /* _MSC_VER */

        object = cJSON_GetObjectItem(item, "size");
        if (object == 0)
        {
            continue;
        }
        m_key_size(key) = (unsigned int)cJSON_GetNumberValue(object);

        object = cJSON_GetObjectItem(item, "type");
        if (object == 0)
        {
            continue;
        }
        m_key_type(key) = (unsigned int)cJSON_GetNumberValue(object);

#ifndef _MSC_VER
#pragma GCC diagnostic pop
#endif /* _MSC_VER */

        if (m_key_type(key) == M_KEY_CUSTOM)
        {
            object = cJSON_GetObjectItem(item, "blob");
            if (object == 0)
            {
                continue;
            }
            m_key_blob(key) = cJSON_GetStringValue(object);
        }

        object = cJSON_GetObjectItem(item, "url");
        if (object)
        {
        }
        m_key_url(key) = cJSON_GetStringValue(object);

        m_info_add(out, key);
    }
    return 0;
}

int m_json_import_info(cJSON **out, const m_info_s *in)
{
    AASSERT(in);
    AASSERT(out);
    *out = cJSON_CreateArray();
    for (size_t i = 0; i != a_vec_len(in); ++i)
    {
        m_key_s *key = A_VEC_PTR(in, i);
        if (m_key_text(key) == 0)
        {
            continue;
        }
        cJSON *item = cJSON_CreateObject();
        cJSON_AddStringToObject(item, "text", m_key_text(key));
        cJSON_AddNumberToObject(item, "size", m_key_size(key));
        cJSON_AddNumberToObject(item, "type", m_key_type(key));
        if (m_key_type(key) == M_KEY_CUSTOM && m_key_blob(key))
        {
            cJSON_AddStringToObject(item, "blob", m_key_blob(key));
        }
        if (m_key_url(key))
        {
            cJSON_AddStringToObject(item, "url", m_key_url(key));
        }
        cJSON_AddItemToArray(*out, item);
    }
    return 0;
}
