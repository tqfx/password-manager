/*!
 @file m_json.h
 @brief json manager
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#pragma once
#ifndef __M_JSON__
#define __M_JSON__

#include "m_info.h"

#include "cjson/cJSON.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

int m_json_load(cJSON **out, const char *in);
int m_json_export_info(const cJSON *in, m_info_s *out);
int m_json_import_info(cJSON **out, const m_info_s *in);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __M_JSON__ */
