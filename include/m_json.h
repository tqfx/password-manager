/*!
 @file m_json.h
 @brief json manager
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#pragma once
#ifndef __M_JSON__
#define __M_JSON__

#include "m_info.h"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif /* __GNUC__ || __clang__ */
#include "cjson/cJSON.h"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif /* __GNUC__ || __clang__ */

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
