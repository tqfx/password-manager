/*!
 @file m_sqlite.h
 @brief sqlite manager
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

/* Define to prevent recursive inclusion */
#ifndef __M_SQLITE_H__
#define __M_SQLITE_H__

#include "m_info.h"
#include "m_word.h"

#include "sqlite3.h"

__BEGIN_DECLS

int m_sqlite_init(sqlite3 *db);
int m_sqlite_done(sqlite3 *db);
int m_sqlite_begin(sqlite3 *db);
int m_sqlite_commit(sqlite3 *db);
int m_sqlite_create_word(sqlite3 *db);
int m_sqlite_create_info(sqlite3 *db);
int m_sqlite_drop_word(sqlite3 *db);
int m_sqlite_drop_info(sqlite3 *db);
int m_sqlite_out_word(sqlite3 *db, m_word_s *out);
int m_sqlite_out_info(sqlite3 *db, m_info_s *out);
int m_sqlite_add_word(sqlite3 *db, const m_word_s *in);
int m_sqlite_add_info(sqlite3 *db, const m_info_s *in);
int m_sqlite_del_word(sqlite3 *db, const m_word_s *in);
int m_sqlite_del_info(sqlite3 *db, const m_info_s *in);

__BEGIN_DECLS

/* Enddef to prevent recursive inclusion */
#endif /* __M_SQLITE_H__ */

/* END OF FILE */
