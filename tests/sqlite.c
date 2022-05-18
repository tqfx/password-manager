/*!
 @file sqlite.c
 @brief sqlite manager testing
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "m_sqlite.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    const char *fname = "t_sqlite.db";

    if (argc > 1)
    {
        fname = argv[argc - 1];
    }

    sqlite3 *db;
    int ret = sqlite3_open(fname, &db);
    if (ret)
    {
        fprintf(stderr, "%s\n", sqlite3_errmsg(db));
        exit(EXIT_FAILURE);
    }

    m_sqlite_init(db);
    m_word_s *word = m_word_new();
    m_info_s *info = m_info_new();
    m_sqlite_out_word(db, word);
    m_sqlite_out_info(db, info);

    printf("0x%zX 0x%zX\n", a_vec_len(word), a_vec_len(info));

    size_t n = a_vec_len(word) + 0x10;

    for (size_t i = a_vec_len(word); i != n; ++i)
    {
        a_str_s *str = m_word_push(word);
        if (str == 0)
        {
            continue;
        }
        a_str_printf(str, "%zu", i);
        m_key_s *key = m_info_push(info);
        if (key == 0)
        {
            continue;
        }
        m_key_set_text(key, a_str_val(str));
    }

    m_sqlite_begin(db);
    m_sqlite_add_word(db, word);
    m_sqlite_add_info(db, info);
    m_sqlite_commit(db);

    m_word_die(word);
    m_info_die(info);

    return sqlite3_close(db);
}
