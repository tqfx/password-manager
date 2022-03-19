/*!
 @file m_sqlite.c
 @brief sqlite for flower password
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#include "m_sqlite.h"

#include <string.h>

static struct
{
    const char *sql;
    const char *word;
    const char *word_text;
    const char *info;
    const char *info_url;
    const char *info_text;
    const char *info_blob;
    const char *info_size;
    const char *info_type;
} state[1] = {
    {
        .sql = 0,
        .word = "WORD",
        .word_text = "TEXT",
        .info = "INFO",
        .info_url = "URL",
        .info_text = "TEXT",
        .info_blob = "BLOB",
        .info_size = "SIZE",
        .info_type = "TYPE",
    },
};

int m_sqlite_begin(sqlite3 *db)
{
    AASSERT(db);
    sqlite3_stmt *stmt = 0;
    sqlite3_prepare(db, "BEGIN;", -1, &stmt, 0);
    sqlite3_step(stmt);
    return sqlite3_finalize(stmt);
}

int m_sqlite_commit(sqlite3 *db)
{
    AASSERT(db);
    sqlite3_stmt *stmt = 0;
    sqlite3_prepare(db, "COMMIT;", -1, &stmt, 0);
    sqlite3_step(stmt);
    return sqlite3_finalize(stmt);
}

int m_sqlite_create_word(sqlite3 *db)
{
    AASSERT(db);
    sqlite3_stmt *stmt = 0;

    state->sql = "CREATE TABLE IF NOT EXISTS %s(%s TEXT NOT NULL PRIMARY KEY);";

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, state->sql, state->word, state->word_text);
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);
    sqlite3_step(stmt);

    return sqlite3_finalize(stmt);
}

int m_sqlite_create_info(sqlite3 *db)
{
    AASSERT(db);
    sqlite3_stmt *stmt = 0;

    state->sql = "CREATE TABLE IF NOT EXISTS %s("
                 "%s TEXT NOT NULL PRIMARY KEY,"
                 "%s INTEGER NOT NULL,"
                 "%s INTEGER NOT NULL,"
                 "%s TEXT,%s TEXT);";

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, state->sql, state->info, state->info_text,
                        state->info_size, state->info_type,
                        state->info_blob, state->info_url);
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);
    sqlite3_step(stmt);

    return sqlite3_finalize(stmt);
}

int m_sqlite_drop_word(sqlite3 *db)
{
    AASSERT(db);
    sqlite3_stmt *stmt = 0;

    state->sql = "DROP TABLE IF EXISTS %s;";

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, state->sql, state->word);
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);
    sqlite3_step(stmt);

    return sqlite3_finalize(stmt);
}

int m_sqlite_drop_info(sqlite3 *db)
{
    AASSERT(db);
    sqlite3_stmt *stmt = 0;

    state->sql = "DROP TABLE IF EXISTS %s;";

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, state->sql, state->info);
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);
    sqlite3_step(stmt);

    return sqlite3_finalize(stmt);
}

int m_sqlite_init(sqlite3 *db)
{
    AASSERT(db);
    m_sqlite_create_word(db);
    m_sqlite_create_info(db);
    return m_sqlite_begin(db);
}

int m_sqlite_done(sqlite3 *db)
{
    AASSERT(db);
    return m_sqlite_commit(db);
}

int m_sqlite_out_word(sqlite3 *db, m_word_s *out)
{
    AASSERT(db);
    AASSERT(out);
    sqlite3_stmt *stmt = 0;

    state->sql = "SELECT * FROM %s;";

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, state->sql, state->word);
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        if (text == 0)
        {
            continue;
        }
        a_str_s *s = m_word_push(out);
        a_str_puts(s, text);
    }

    return sqlite3_finalize(stmt);
}

int m_sqlite_out_info(sqlite3 *db, m_info_s *out)
{
    AASSERT(db);
    AASSERT(out);
    sqlite3_stmt *stmt = 0;

    state->sql = "SELECT * FROM %s ORDER BY %s ASC;";

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, state->sql, state->info, state->info_text);
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        if (text == 0)
        {
            continue;
        }
        m_key_s *key = m_info_push(out);
        m_key_set_text(key, text);
        m_key_set_size(key, (unsigned int)sqlite3_column_int(stmt, 1));
        m_key_set_type(key, (unsigned int)sqlite3_column_int(stmt, 2));
        text = sqlite3_column_text(stmt, 3);
        if (m_key_type(key) == M_KEY_CUSTOM && text)
        {
            m_key_set_blob(key, text);
        }
        if ((text = sqlite3_column_text(stmt, 4), text))
        {
            m_key_set_url(key, text);
        }
    }

    return sqlite3_finalize(stmt);
}

int m_sqlite_add_word(sqlite3 *db, const m_word_s *in)
{
    AASSERT(db);
    AASSERT(in);
    sqlite3_stmt *stmt = 0;

    state->sql = "INSERT INTO %s VALUES(?);";

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, state->sql, state->word);
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);

    for (size_t i = 0; i != a_vec_len(in); ++i)
    {
        const a_str_s *s = A_VEC_PTR(in, i);
        if (a_str_len(s))
        {
            sqlite3_reset(stmt);
            sqlite3_bind_text(stmt, 1, a_str_val(s), (int)a_str_len(s), SQLITE_STATIC);
            sqlite3_step(stmt);
        }
    }

    return sqlite3_finalize(stmt);
}

int m_sqlite_add_info(sqlite3 *db, const m_info_s *in)
{
    AASSERT(db);
    AASSERT(in);
    sqlite3_stmt *stmt = 0;

    state->sql = "INSERT INTO %s VALUES(?,?,?,?,?);";

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, state->sql, state->info);
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);

    for (size_t i = 0; i != a_vec_len(in); ++i)
    {
        const m_key_s *key = A_VEC_PTR(in, i);
        if (m_key_text(key))
        {
            sqlite3_reset(stmt);
            sqlite3_bind_int(stmt, 2, (int)m_key_size(key));
            sqlite3_bind_int(stmt, 3, (int)m_key_type(key));
            {
                int size = (int)strlen(m_key_text(key));
                sqlite3_bind_text(stmt, 1, m_key_text(key), size, SQLITE_STATIC);
            }
            {
                int size = ((m_key_type(key) == M_KEY_CUSTOM) && m_key_blob(key))
                             ? (int)strlen(m_key_blob(key))
                             : 0;
                sqlite3_bind_text(stmt, 4, m_key_blob(key), size, SQLITE_STATIC);
            }
            {
                int size = m_key_url(key)
                             ? (int)strlen(m_key_url(key))
                             : 0;
                sqlite3_bind_text(stmt, 5, m_key_url(key), size, SQLITE_STATIC);
            }
            sqlite3_step(stmt);
        }
    }

    return sqlite3_finalize(stmt);
}

int m_sqlite_del_word(sqlite3 *db, const m_word_s *in)
{
    AASSERT(db);
    AASSERT(in);
    sqlite3_stmt *stmt = 0;

    state->sql = "DELETE FROM %s WHERE %s = ?;";

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, state->sql, state->word, state->word_text);
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);

    for (size_t i = 0; i != a_vec_len(in); ++i)
    {
        const a_str_s *s = A_VEC_PTR(in, i);
        if (a_str_len(s))
        {
            sqlite3_reset(stmt);
            sqlite3_bind_text(stmt, 1, a_str_val(s), (int)a_str_len(s), SQLITE_STATIC);
            sqlite3_step(stmt);
        }
    }

    return sqlite3_finalize(stmt);
}

int m_sqlite_del_info(sqlite3 *db, const m_info_s *in)
{
    AASSERT(db);
    AASSERT(in);
    sqlite3_stmt *stmt = 0;

    state->sql = "DELETE FROM %s WHERE %s = ?;";

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, state->sql, state->info, state->info_text);
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);

    for (size_t i = 0; i != a_vec_len(in); ++i)
    {
        const m_key_s *key = A_VEC_PTR(in, i);
        if (m_key_text(key))
        {
            sqlite3_reset(stmt);
            sqlite3_bind_text(stmt, 1, m_key_text(key), (int)strlen(m_key_text(key)), SQLITE_STATIC);
            sqlite3_step(stmt);
        }
    }

    return sqlite3_finalize(stmt);
}
