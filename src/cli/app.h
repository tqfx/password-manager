/*!
 @file app.h
 @brief application
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#pragma once
#ifndef __APP_H__
#define __APP_H__

#include "m_sqlite.h"

#include "console.h"
#include "convert.h"

__BEGIN_DECLS

void app_log(unsigned int n, ...);
int app_exec(const m_key_s *key, const char *word) __NONNULL_ALL;
int app_init(const char *fname) __NONNULL_ALL;
int app_done(void);
void app_show_word(const void *word);
void app_show_info(const void *key);
void app_show_word_idx(const m_word_s *word) __NONNULL_ALL;
void app_show_info_idx(const m_info_s *info) __NONNULL_ALL;
int app_add_word(const m_word_s *word) __NONNULL_ALL;
int app_add_info(const m_info_s *info) __NONNULL_ALL;
int app_del_word(const m_word_s *word) __NONNULL_ALL;
int app_del_info(const m_info_s *info) __NONNULL_ALL;
int app_del_word_idx(const m_word_s *word) __NONNULL_ALL;
int app_del_info_idx(const m_info_s *info) __NONNULL_ALL;
int app_generate_idx(unsigned int word, unsigned int key);
int app_generate_key(const m_key_s *key) __NONNULL_ALL;
int app_convert(const char *in, const char *out) __NONNULL_ALL;
int app_import(const char *fname) __NONNULL_ALL;
int app_export(const char *fname) __NONNULL_ALL;

#if defined(_WIN32)
int app_windows_check(void);
#endif /* _WIN32 */

__BEGIN_DECLS

static const char s_missing[] = "missing";
static const char s_failure[] = "failure";
static const char s_success[] = "success";
static const char s_warning[] = "warning";

#endif /* __APP_H__ */
