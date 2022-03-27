/*!
 @file app.c
 @brief application
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#include "app.h"

#include "m_generate.h"
#include "clipboard.h"

#include <assert.h>

#define TEXT_RED       CONSOLE_TEXT_RED
#define TEXT_GREEN     CONSOLE_TEXT_GREEN
#define TEXT_TURQUOISE CONSOLE_TEXT_TURQUOISE
#define TEXT_WHITE     CONSOLE_TEXT_WHITE
#define TEXT_DEFAULT   CONSOLE_TEXT_DEFAULT

static void app_log3(const char *obj, unsigned int color,
                     const char *state, const char *info)
{
    app_log(3, TEXT_WHITE, obj, color, state, TEXT_TURQUOISE, info);
}

static void app_print_word(size_t index, const char *word)
{
    char buf[1 << 4];
    sprintf(buf, "%04zu", index);
    app_log(2, TEXT_TURQUOISE, buf, TEXT_DEFAULT, word);
}

static void app_print_key(size_t index, const m_key_s *key)
{
    char *text = key->text;
#if defined(_WIN32)
    code_utf8_to(&text, key->text);
#endif /* _WIN32 */
    char buf_i[1 << 4];
    sprintf(buf_i, "%04zu", index);
    char buf_t[1 << 2];
    sprintf(buf_t, "%u", key->type);
    char buf_z[1 << 2];
    sprintf(buf_z, "%02u", key->size);
    app_log(6, TEXT_TURQUOISE, buf_i,
            TEXT_GREEN, buf_z,
            TEXT_GREEN, buf_t,
            TEXT_DEFAULT, text,
            TEXT_GREEN, key->blob,
            TEXT_DEFAULT, key->url);
#if defined(_WIN32)
    free(text);
#endif /* _WIN32 */
}

void app_log(unsigned int n, ...)
{
    va_list ap;
    va_start(ap, n);
    console_vprint(" ", n, ap);
    printf("\n");
    va_end(ap);
}

int app_exec(const m_key_s *key, const char *word)
{
    assert(key);
    assert(word);
    if ((key->type == M_KEY_CUSTOM) && (key->blob == 0))
    {
        app_log(2, TEXT_RED, s_missing, TEXT_TURQUOISE, "blob");
        return ~0;
    }

    char *out = 0;
    if (m_generate(key, word, &out))
    {
        if ((out == 0) || (strlen(out) == 0))
        {
            app_log(2, TEXT_RED, s_missing, TEXT_TURQUOISE, "p");
        }
        else if ((key->text == 0) || (strlen(key->text) == 0))
        {
            app_log(2, TEXT_RED, s_missing, TEXT_TURQUOISE, "k");
        }
        else
        {
            app_log(2, TEXT_RED, s_failure, TEXT_TURQUOISE, "exec");
        }
        return ~0;
    }

#if defined(_WIN32)
    clipboard_sets(out);
    char *text = 0;
    code_utf8_to(&text, key->text);
    app_log(2, TEXT_GREEN, "ok", TEXT_TURQUOISE, text);
    free(text);
#else
    app_log(2, TEXT_TURQUOISE, out, TEXT_DEFAULT, key->text);
#endif /* _WIN32 */

    free(out);

    return 0;
}

#define STATUS_ZERO 0
#define STATUS_INIT (1 << 0)
#define STATUS_DONE (1 << 1)
#define STATUS_MODP (1 << 8)
#define STATUS_MODK (1 << 9)

#define STATUS_SET(stat, mask)    ((stat) |= (mask))
#define STATUS_CLR(stat, mask)    ((stat) &= ~(mask))
#define STATUS_IS_SET(stat, mask) (((stat) & (mask)) == (mask))
#define STATUS_IS_CLR(stat, mask) (((stat) & (mask)) != (mask))

#pragma pack(push, 4)
static struct
{
    sqlite3 *db;
    const char *fname;
    m_word_s word[1];
    m_info_s info[1];
    int status;
} state[1] = {
    {
        .db = 0,
        .fname = 0,
        .status = STATUS_ZERO,
    },
};
#pragma pack(pop)

int app_init(const char *fname)
{
    assert(fname);
    if (STATUS_IS_SET(state->status, STATUS_INIT))
    {
        return ~0;
    }

    sqlite3_initialize();
    STATUS_CLR(state->status, STATUS_DONE);

    int ret = sqlite3_open(fname, &state->db);
    if (ret != SQLITE_OK)
    {
        fprintf(stderr, "%s\n", sqlite3_errmsg(state->db));
        exit(EXIT_FAILURE);
    }

    state->fname = fname;
    m_sqlite_init(state->db);
    STATUS_SET(state->status, STATUS_INIT);

    m_word_ctor(state->word);
    m_info_ctor(state->info);

    m_sqlite_out_word(state->db, state->word);
    m_sqlite_out_info(state->db, state->info);

    return ret;
}

int app_done(void)
{
    if (STATUS_IS_CLR(state->status, STATUS_INIT))
    {
        return ~0;
    }

    if (STATUS_IS_SET(state->status, STATUS_MODP))
    {
        m_sqlite_drop_word(state->db);
        m_sqlite_create_word(state->db);
        m_sqlite_add_word(state->db, state->word);
        STATUS_CLR(state->status, STATUS_MODP);
    }
    if (STATUS_IS_SET(state->status, STATUS_MODK))
    {
        m_sqlite_drop_info(state->db);
        m_sqlite_create_info(state->db);
        m_sqlite_add_info(state->db, state->info);
        STATUS_CLR(state->status, STATUS_MODK);
    }

    m_sqlite_done(state->db);
    sqlite3_close(state->db);
    STATUS_CLR(state->status, STATUS_INIT);

    m_word_dtor(state->word);
    m_info_dtor(state->info);
    STATUS_SET(state->status, STATUS_DONE);

    return sqlite3_shutdown();
}

void app_show_word(const void *word)
{
    printf("   I P\n");
    const char *s = word ? (const char *)word : "";
    for (size_t i = 0; i != a_vec_len(state->word); ++i)
    {
        a_str_s *str = m_word_at(state->word, i);
        if (a_str_len(str) && strstr(a_str_val(str), s))
        {
            app_print_word(i, a_str_val(str));
        }
    }
}

void app_show_info(const void *key)
{
    printf("   I  L T K\n");
    const char *s = key ? (const char *)key : "";
    for (size_t i = 0; i != a_vec_len(state->info); ++i)
    {
        m_key_s *p = m_info_at(state->info, i);
        if (m_key_text(p) && strstr(m_key_text(p), s))
        {
            app_print_key(i, p);
        }
    }
}

void app_show_word_idx(const m_word_s *word)
{
    assert(word);
    if (a_vec_len(word))
    {
        printf("   I P\n");
    }
    for (size_t i = 0; i != a_vec_len(word); ++i)
    {
        unsigned int idx;
        a_str_s *str = m_word_at(word, i);
        sscanf(a_str_val(str), "%u", &idx);
        if (idx < a_vec_len(word))
        {
            app_print_word(idx, a_str_val(str));
        }
    }
}

void app_show_info_idx(const m_info_s *info)
{
    assert(info);
    if (a_vec_len(info))
    {
        printf("   I  L T K\n");
    }
    for (size_t i = 0; i != a_vec_len(info); ++i)
    {
        unsigned int idx;
        m_key_s *key = m_info_at(info, i);
        sscanf(m_key_text(key), "%u", &idx);
        if (idx < a_vec_len(info))
        {
            app_print_key(idx, key);
        }
    }
}

int app_add_word(const m_word_s *word)
{
    assert(word);
    int ret = ~0;
    for (size_t i = 0; i != a_vec_len(word); ++i)
    {
        a_str_s *str = m_word_at(word, i);
        if ((a_str_len(str) == 0) || (strlen(a_str_val(str)) == 0))
        {
            app_log3(state->fname, TEXT_RED, s_missing, "p");
            break;
        }
        ret = m_word_add(state->word, str);
        if (ret == 0)
        {
            STATUS_SET(state->status, STATUS_MODP);
            app_log3(state->fname, TEXT_GREEN, s_success, a_str_val(str));
        }
        else
        {
            app_log3(state->fname, TEXT_RED, s_failure, a_str_val(str));
        }
    }
    return ret;
}

int app_add_info(const m_info_s *info)
{
    assert(info);
    int ret = ~0;
    for (size_t i = 0; i != a_vec_len(info); ++i)
    {
        m_key_s *key = m_info_at(info, i);
        if ((m_key_type(key) == M_KEY_CUSTOM) && (m_key_blob(key) == 0))
        {
            app_log3(state->fname, TEXT_RED, s_missing, "blob");
            break;
        }
        if ((m_key_text(key) == 0) || (strlen(m_key_text(key)) == 0))
        {
            app_log3(state->fname, TEXT_RED, s_missing, "k");
            break;
        }
        ret = m_info_add(state->info, key);
        if (ret == 0)
        {
            STATUS_SET(state->status, STATUS_MODK);
            app_generate_key(key);
        }
        else
        {
            app_log3(state->fname, TEXT_RED, s_failure, m_key_text(key));
        }
    }
    return ret;
}

int app_del_word(const m_word_s *word)
{
    assert(word);
    int ret = ~0;
    for (size_t i = 0; i != a_vec_len(word); ++i)
    {
        a_str_s *str = m_word_at(word, i);
        ret = m_word_del(state->word, str);
        if (ret == 0)
        {
            STATUS_SET(state->status, STATUS_MODP);
            app_log3(state->fname, TEXT_GREEN, s_success, a_str_val(str));
        }
        else
        {
            app_log3(state->fname, TEXT_RED, s_failure, a_str_val(str));
        }
    }
    return ret;
}

int app_del_info(const m_info_s *info)
{
    assert(info);
    int ret = ~0;
    for (size_t i = 0; i != a_vec_len(info); ++i)
    {
        m_key_s *key = m_info_at(info, i);
        ret = m_info_del(state->info, key);
        if (ret == 0)
        {
            STATUS_SET(state->status, STATUS_MODK);
            app_log3(state->fname, TEXT_GREEN, s_success, m_key_text(key));
        }
        else
        {
            app_log3(state->fname, TEXT_RED, s_failure, m_key_text(key));
        }
    }
    return ret;
}

int app_del_word_idx(const m_word_s *word)
{
    assert(word);
    int ret = ~0;
    for (size_t i = 0; i != a_vec_len(word); ++i)
    {
        unsigned int x = 0;
        a_str_s *str = m_word_at(word, i);
        sscanf(a_str_val(str), "%u", &x);
        if (x < a_vec_len(state->word) &&
            (str = m_word_at(state->word, x), a_str_len(str)))
        {
            STATUS_SET(state->status, STATUS_MODP);
            app_print_word(x, a_str_val(str));
            a_str_dtor(str);
            ret = 0;
        }
    }
    return ret;
}

int app_del_info_idx(const m_info_s *info)
{
    assert(info);
    int ret = ~0;
    for (size_t i = 0; i != a_vec_len(info); ++i)
    {
        unsigned int x = 0;
        m_key_s *key = m_info_at(info, i);
        sscanf(m_key_text(key), "%u", &x);
        if (x < a_vec_len(state->info) &&
            (key = m_info_at(state->info, x), m_key_text(key)))
        {
            STATUS_SET(state->status, STATUS_MODK);
            app_print_key(x, key);
            m_key_dtor(key);
            ret = 0;
        }
    }
    return ret;
}

int app_generate_idx(unsigned int word, unsigned int key)
{
    int ret = ~0;

    if (a_vec_len(state->word) == 0)
    {
        app_log3(state->fname, TEXT_RED, s_missing, "p");
        goto label_exit;
    }
    else if (word >= a_vec_len(state->word))
    {
        app_log3(state->fname, TEXT_RED, s_failure, "p");
        goto label_exit;
    }

    if (a_vec_len(state->info) == 0)
    {
        app_log3(state->fname, TEXT_RED, s_missing, "k");
        goto label_exit;
    }
    else if (key >= a_vec_len(state->info))
    {
        app_log3(state->fname, TEXT_RED, s_failure, "k");
        goto label_exit;
    }

    if (word)
    {
        a_str_s str[1];
        a_str_s *ptr = m_word_ptr(state->word);
        a_str_move(str, ptr + word);
        a_str_move(ptr + word, ptr);
        a_str_move(ptr, str);
        STATUS_SET(state->status, STATUS_MODP);
    }

    a_str_s *str = m_word_ptr(state->word);
    m_key_s *ptr = m_info_ptr(state->info);
    ret = app_exec(ptr + key, a_str_val(str));

label_exit:
    return ret;
}

int app_generate_key(const m_key_s *key)
{
    assert(key);
    int ret = ~0;
    if (a_vec_len(state->word))
    {
        ret = app_exec(key, a_str_val(m_word_ptr(state->word)));
    }
    else
    {
        app_log(2, TEXT_RED, s_missing, TEXT_TURQUOISE, "p");
    }
    return ret;
}

#include "m_json.h"
#include "m_io.h"

static int app_import_(m_info_s *info, const char *in)
{
    assert(in);
    assert(info);
    int ret = ~0;
    cJSON *json = 0;
    ret = m_json_load(&json, in);
    if (json)
    {
        ret = m_json_export_info(json, info);
        cJSON_Delete(json);
        return ret;
    }
    sqlite3 *db = 0;
    ret = sqlite3_open(in, &db);
    if (ret == SQLITE_OK)
    {
        return m_sqlite_out_info(db, info);
    }
    fprintf(stderr, "%s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return ret;
}

static int app_export_(m_info_s *info, const char *out)
{
    assert(out);
    assert(info);
    if (strstr(out, ".db"))
    {
        sqlite3 *db = 0;
        int ret = sqlite3_open(out, &db);
        if (ret == SQLITE_OK)
        {
            m_sqlite_create_info(db);
            m_sqlite_begin(db);
            m_sqlite_add_info(db, info);
            m_sqlite_commit(db);
        }
        sqlite3_close(db);
        return ret;
    }
    else
    {
        cJSON *json = 0;
        m_json_import_info(&json, info);
        char *str = cJSON_PrintUnformatted(json);
        if (str)
        {
            m_io_write(out, str, strlen(str));
            free(str);
        }
        cJSON_Delete(json);
        return 0;
    }
}

int app_convert(const char *in, const char *out)
{
    assert(in);
    assert(out);
    int ret = ~0;
    m_info_s *info = m_info_new();
    ret = app_import_(info, in);
    if (ret)
    {
        goto label_exit;
    }
    ret = app_export_(info, out);
    if (ret)
    {
        goto label_exit;
    }
label_exit:
    m_info_die(info);
    return ret;
}

int app_import(const char *fname)
{
    assert(fname);
    m_info_s *info = m_info_new();
    int ret = app_import_(info, fname);
    if (a_vec_len(info) && ret == 0)
    {
        STATUS_SET(state->status, STATUS_MODK);
        for (size_t i = 0; i != a_vec_len(info); ++i)
        {
            m_info_add(state->info, m_info_at(info, i));
        }
        app_log3(state->fname, TEXT_GREEN, s_success, fname);
    }
    else
    {
        app_log3(state->fname, TEXT_RED, s_failure, fname);
    }
    m_info_die(info);
    return ret;
}

int app_export(const char *fname)
{
    assert(fname);
    return app_export_(state->info, fname);
}
