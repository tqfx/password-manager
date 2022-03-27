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

__BEGIN_DECLS

int m_json_load(cJSON **out, const char *in) __NONNULL_ALL;
int m_json_export_info(const cJSON *in, m_info_s *out) __NONNULL_ALL;
int m_json_import_info(cJSON **out, const m_info_s *in) __NONNULL_ALL;

__END_DECLS

#endif /* __M_JSON__ */
