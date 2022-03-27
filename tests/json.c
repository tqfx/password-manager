/*!
 @file json.c
 @brief json manager testing
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#include "m_json.h"

#include <stdio.h>

int main(void)
{
    m_info_s *info = m_info_new();
    m_key_s *key = m_info_push(info);
    m_key_set_text(key, "0");
    key = m_info_push(info);
    m_key_set_text(key, "1");
    m_key_set_size(key, 6);
    m_key_set_type(key, M_KEY_DIGIT);
    key = m_info_push(info);
    m_key_set_text(key, "2");
    m_key_set_type(key, M_KEY_CUSTOM);
    m_key_set_blob(key, "blob");
    m_key_set_url(key, "url");
    cJSON *json = 0;
    m_json_import_info(&json, info);
    m_info_dtor(info);
    m_info_ctor(info);
    m_json_export_info(json, info);
    cJSON_Delete(json);
    m_json_import_info(&json, info);
    m_info_die(info);
    cJSON_Delete(json);
    return 0;
}
