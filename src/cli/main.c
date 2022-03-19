/*!
 @file main.c
 @brief main program
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#include "app.h"

#include <getopt.h>

#define OPTION_ADD   (1 << 0)
#define OPTION_DEL   (1 << 1)
#define OPTION_SHOW  (1 << 2)
#define OPTION_INDEX (1 << 3)

#define OPTION_SET(stat, mask)    ((stat) |= (mask))
#define OPTION_CLR(stat, mask)    ((stat) &= ~(mask))
#define OPTION_IS_SET(stat, mask) (((stat) & (mask)) == (mask))

#pragma pack(push, 4)
static struct
{
    char *file;
    char *data;
    m_key_s key[1];
    m_word_s word[1];
    m_info_s info[1];
    int option;
} state[1] = {
    {
        .file = 0,
        .data = 0,
        .option = 0,
    },
};
#pragma pack(pop)

static void main_help(const char *args)
{
    static const char help[] = "option: --import > -d[i] > -a > -s[i] > -i\n\
  -a --add       option for add\n\
  -d --del       option for del\n\
  -s --show      option for show\n\
  -i --index     option for index\n\
  -k --key       string(\"*\"->\"\")\n\
  -t --type      0:email 1:digit 2:custom\n\
  -b --blob      string\n\
  -u --url       url\n\
  -l --length    1 ~ 32\n\
  -p --password  string(\"*\"->\"\")\n\
  -f --filename  filename\n\
     --import    filename\n\
Copyright (C) 2020 tqfx. All rights reserved.";
    printf("%s\n%s\n", args, help);
}

static int main_app(char *args)
{
#if defined(_WIN32)
    for (size_t i = 0; i != a_vec_len(state->info); ++i)
    {
        m_key_s *key = A_VEC_PTR(state->info, i);
        if (m_key_text(key))
        {
            char *text;
            code_to_utf8(&text, key->text);
            free(key->text);
            key->text = text;
        }
    }
#endif /* _WIN32 */
    if (state->file == 0)
    {
#if defined(_WIN32)
        if (strstr(args, ".exe"))
        {
            args[strlen(args) - 4] = 0;
        }
#endif /* _WIN32 */
        a_str_s str[1] = {A_STR_INITS()};
        a_str_printf(str, "%s%s", args, ".db");
        state->file = a_str_done(str);
    }

    app_init(state->file);

    if (state->data)
    {
        app_import(state->data);
        free(state->data);
        state->data = 0;
    }
    else if (OPTION_IS_SET(state->option, OPTION_DEL))
    {
        OPTION_CLR(state->option, OPTION_DEL);
        if (OPTION_IS_SET(state->option, OPTION_INDEX))
        {
            OPTION_CLR(state->option, OPTION_INDEX);
            app_del_word_idx(state->word);
            app_del_info_idx(state->info);
        }
        else
        {
            app_del_word(state->word);
            app_del_info(state->info);
        }
    }
    else if (OPTION_IS_SET(state->option, OPTION_ADD))
    {
        OPTION_CLR(state->option, OPTION_ADD);
        app_add_word(state->word);
        app_add_info(state->info);
    }
    else if (OPTION_IS_SET(state->option, OPTION_SHOW | OPTION_INDEX))
    {
        OPTION_CLR(state->option, OPTION_SHOW | OPTION_INDEX);
        app_show_word_idx(state->word);
        app_show_info_idx(state->info);
    }
    else if ((state->option & (OPTION_SHOW | OPTION_INDEX)) == OPTION_SHOW)
    {
        OPTION_CLR(state->option, OPTION_SHOW);
        for (size_t i = 0; i != a_vec_len(state->word); ++i)
        {
            a_str_s *str = A_VEC_PTR(state->word, i);
            app_show_word(a_str_val(str));
        }
        for (size_t i = 0; i != a_vec_len(state->info); ++i)
        {
            m_key_s *key = A_VEC_PTR(state->info, i);
            app_show_info(m_key_text(key));
        }
    }
    else if (a_vec_len(state->info) && OPTION_IS_SET(state->option, OPTION_INDEX))
    {
        unsigned int ip = 0;
        unsigned int ik = 0;
        OPTION_CLR(state->option, OPTION_INDEX);
        for (size_t i = 0; i != a_vec_len(state->info); ++i)
        {
            m_key_s *key = A_VEC_PTR(state->info, i);
            sscanf(m_key_text(key), "%u", &ik);
            if (a_vec_len(state->word))
            {
                a_str_s *str = A_VEC_PTR(state->word, a_vec_len(state->word) - 1);
                sscanf(a_str_val(str), "%u", &ip);
            }
            app_generate_idx(ip, ik);
        }
    }
    else if (a_vec_len(state->info))
    {
        for (size_t i = 0; i != a_vec_len(state->info); ++i)
        {
            m_key_s *key = A_VEC_PTR(state->info, i);
            if (a_vec_len(state->word))
            {
                a_str_s *str = A_VEC_PTR(state->word, a_vec_len(state->word) - 1);
                app_exec(key, a_str_val(str));
            }
            else
            {
                app_generate_key(key);
            }
        }
    }
    else
    {
        main_help(args);
    }

    return app_done();
}

__STATIC_INLINE
char *text_optarg(char *arg)
{
    return (*arg != '*') ? arg : (arg + 1);
}

int main(int argc, char *argv[])
{
    int opt = 0;
    const char *shortopts = "adhsik:u:b:t:l:p:f:";
    struct option longopts[] = {
        {"add", no_argument, 0, 'a'},
        {"del", no_argument, 0, 'd'},
        {"help", no_argument, 0, 'h'},
        {"show", no_argument, 0, 's'},
        {"index", no_argument, 0, 'i'},
        {"key", required_argument, 0, 'k'},
        {"url", required_argument, 0, 'u'},
        {"blob", required_argument, 0, 'b'},
        {"type", required_argument, 0, 't'},
        {"import", required_argument, 0, 1},
        {"length", required_argument, 0, 'l'},
        {"password", required_argument, 0, 'p'},
        {"filename", required_argument, 0, 'f'},
        {0, 0, 0, 0},
    };

    if (argc == 1)
    {
        main_help(*argv);
        exit(EXIT_SUCCESS);
    }

#if defined(_WIN32)
    if (app_windows_check())
    {
        exit(EXIT_FAILURE);
    }
#endif /* _WIN32 */

    /* init */
    {
        m_key_ctor(state->key);
        m_word_ctor(state->word);
        m_info_ctor(state->info);
    }

    while ((opt = getopt_long(argc, argv, shortopts, longopts, &optind), opt) != -1)
    {
        a_str_s ctx[1] = {A_STR_INITS()};
        switch (opt)
        {
        case 'h':
        {
            main_help(*argv);
            exit(EXIT_SUCCESS);
        }
        break;
        case 'a':
        {
            OPTION_SET(state->option, OPTION_ADD);
        }
        break;
        case 'd':
        {
            OPTION_SET(state->option, OPTION_DEL);
        }
        break;
        case 's':
        {
            OPTION_SET(state->option, OPTION_SHOW);
        }
        break;
        case 'i':
        {
            OPTION_SET(state->option, OPTION_INDEX);
        }
        break;
        case 'k':
        {
            if ((m_key_type(state->key) != M_KEY_CUSTOM) && m_key_blob(state->key))
            {
                m_key_blob(state->key) = 0;
            }
            m_key_text(state->key) = text_optarg(optarg);
            if (m_info_add(state->info, state->key))
            {
                fprintf(stderr, "%s + %s!\n", m_key_text(state->key), s_failure);
            }
            m_key_text(state->key) = 0;
            m_key_blob(state->key) = 0;
            m_key_url(state->key) = 0;
        }
        break;
        case 'u':
        {
            m_key_url(state->key) = optarg;
        }
        break;
        case 'b':
        {
            m_key_blob(state->key) = optarg;
        }
        break;
        case 't':
        {
            sscanf(optarg, "%u", &m_key_type(state->key));
            if (m_key_type(state->key) > M_KEY_CUSTOM)
            {
                m_key_type(state->key) = M_KEY_EMAIL;
            }
        }
        break;
        case 'l':
        {
            sscanf(optarg, "%u", &m_key_size(state->key));
            if (m_key_size(state->key) > M_KEY_OUTSIZ)
            {
                m_key_size(state->key) = M_KEY_OUTSIZ;
            }
        }
        break;
        case 'p':
        {
            a_str_s *str = m_word_push(state->word);
            a_str_puts(str, text_optarg(optarg));
        }
        break;
        case 'f':
        {
            if (state->file)
            {
                free(state->file);
            }
            a_str_puts(ctx, optarg);
            state->file = a_str_done(ctx);
        }
        break;
        case 1:
        {
            if (state->data)
            {
                free(state->data);
            }
            a_str_puts(ctx, optarg);
            state->data = a_str_done(ctx);
        }
        break;
        default:
            break;
        }
    }

    if (a_vec_len(state->info))
    {
        m_key_s *key = A_VEC_PTR(state->info, a_vec_len(state->info) - 1);
        m_key_set_size(key, m_key_size(state->key));
        m_key_set_type(key, m_key_type(state->key));
        if (m_key_blob(state->key))
        {
            if (m_key_type(state->key) == M_KEY_CUSTOM)
            {
                m_key_set_blob(key, m_key_blob(state->key));
            }
            m_key_blob(state->key) = 0;
        }
        if (m_key_url(state->key))
        {
            m_key_set_url(key, m_key_url(state->key));
            m_key_url(state->key) = 0;
        }
    }

    opt = main_app(*argv);

    /* done */
    {
        m_word_dtor(state->word);
        m_info_dtor(state->info);
        m_key_dtor(state->key);
        free(state->file);
        state->file = 0;
    }

    return opt;
}
