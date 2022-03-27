/*!
 @file m_generate.c
 @brief generate manager
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#include "m_generate.h"

#include "a_convert.h"
#include "a_hmac.h"

#include <assert.h>
#include <string.h>
#include <ctype.h>

__STATIC_INLINE
int xdigit(int x)
{
    return a_xdigit(x);
}

static char *hmac(const void *key, size_t keysiz,
                  const void *msg, size_t msgsiz,
                  void *out)
{
    assert(!keysiz || key);
    assert(!msgsiz || msg);

    a_hmac_s ctx[1];

    a_hmac_init(ctx, &a_hash_md5, key, keysiz);
    a_hmac_process(ctx, msg, msgsiz);
    a_hmac_done(ctx, ctx->buf);

    return (char *)a_digest_lower(ctx->buf, ctx->outsiz, out);
}

int m_generate1(const m_key_s *key, const char *word, char **out)
{
    assert(out);
    assert(key);
    assert(word);

    if ((key->type == M_KEY_CUSTOM) && (key->blob == 0))
    {
        return -3;
    }
    if (key->text == 0)
    {
        return -2;
    }
    unsigned int lword = (unsigned int)strlen(word);
    unsigned int ltext = (unsigned int)strlen(key->text);
    if ((key->size == 0) || (lword == 0) || (ltext == 0))
    {
        return -1;
    }

    char buf[(M_KEY_OUTSIZ << 1) + 1];
    unsigned char tmp[M_KEY_OUTSIZ + 1];
    hmac(key->text, ltext, word, lword, tmp);
    hmac("kise", 4, tmp, M_KEY_OUTSIZ, buf + M_KEY_OUTSIZ * 0);
    hmac("snow", 4, tmp, M_KEY_OUTSIZ, buf + M_KEY_OUTSIZ * 1);

    unsigned char count = 0;
    unsigned char cnt[10] = {0};
    unsigned int length = key->size < M_KEY_OUTSIZ ? key->size : M_KEY_OUTSIZ;
    *out = (char *)calloc(length + 1, sizeof(char));
    for (unsigned int i = 0; i != length; ++i)
    {
        int x = xdigit((buf + M_KEY_OUTSIZ * 0)[i]) +
                xdigit((buf + M_KEY_OUTSIZ * 1)[i]);
        tmp[i] = (unsigned char)x;

        switch (key->type)
        {
        case M_KEY_EMAIL:
        case M_KEY_CUSTOM:
        {
            (*out)[i] = (buf + M_KEY_OUTSIZ * 1)[i];
            if (!isdigit((int)(*out)[i]))
            {
                if (strchr("sunlovesnow1990090127xykab", buf[i]))
                {
                    (*out)[i] = (char)toupper((*out)[i]);
                }
            }
        }
        break;

        case M_KEY_DIGIT:
        {
            x %= 10;

            int m = x;
            while (cnt[x] > count)
            {
                if (++x == 10)
                {
                    x = 0;
                }
                if (x == m)
                {
                    ++count;
                }
            }
            ++cnt[x];

            (*out)[i] = (char)('0' + x);
        }
        break;

        default:
            break;
        }
    }

    if (key->type != M_KEY_DIGIT)
    {
        if (isdigit((int)(*out)[0]))
        {
            (*out)[0] = 'K';
        }
        if (key->type == M_KEY_CUSTOM)
        {
            unsigned int lblob = (unsigned int)strlen(key->blob);
            for (unsigned int i = 0; i != lblob; ++i)
            {
                (*out)[tmp[i % M_KEY_OUTSIZ] % length] = key->blob[i];
            }
        }
    }

    return 0;
}

#pragma pack(push, 4)
static struct
{
    /* points to rule data */
    const char *s0;
    const char *s1;
    const char *s2;
    const char *s3;
    /* length for rule data */
    unsigned int l0;
    unsigned int l1;
    unsigned int l2;
    unsigned int l3;
    /* character table */
    const char ch[60];
} state[1] = {
    {
        .s0 = 0,
        .s1 = 0,
        .s2 = 0,
        .s3 = 0,
        .l0 = 0,
        .l1 = 0,
        .l2 = 0,
        .l3 = 0,
        .ch = {
            /* clang-format off */
            'a', 'A', 'b', 'B', 'c', 'C', 'd', 'D', 'e', 'E', 'f', 'F', 'g', 'G', 'h',
            'H', 'i', 'j', 'J', 'k', 'K', 'l', 'L', 'm', 'M', '0', '1', '2', '3', '4',
            '5', '6', '7', '8', '9', 'n', 'N', 'o', 'p', 'P', 'q', 'Q', 'r', 'R', 's',
            'S', 't', 'T', 'u', 'U', 'v', 'V', 'w', 'W', 'x', 'X', 'y', 'Y', 'z', 'Z',
            /* clang-format on */
        },
    },
};
#pragma pack(pop)

void m_generate_setrule(const void *s0, const void *s1, const void *s2, const void *s3)
{
    /* Set the rules */
    state->s0 = (const char *)s0;
    state->s1 = (const char *)s1;
    state->s2 = (const char *)s2;
    state->s3 = (const char *)s3;
    /* Set the rule length */
    state->l0 = s0 ? (unsigned int)strlen(state->s0) : 0;
    state->l1 = s1 ? (unsigned int)strlen(state->s1) : 0;
    state->l2 = s2 ? (unsigned int)strlen(state->s2) : 0;
    state->l3 = s3 ? (unsigned int)strlen(state->s3) : 0;
}

int m_generate2(const m_key_s *key, const char *word, char **out)
{
    assert(out);
    assert(key);
    assert(word);

    if ((key->type == M_KEY_CUSTOM) && (key->blob == 0))
    {
        return -3;
    }
    if (key->text == 0)
    {
        return -2;
    }
    unsigned int lword = (unsigned int)strlen(word);
    unsigned int ltext = (unsigned int)strlen(key->text);
    if ((key->size == 0) || (lword == 0) || (ltext == 0))
    {
        return -1;
    }

    char buf[(M_KEY_OUTSIZ << 2) + 1];
    unsigned char tmp[M_KEY_OUTSIZ + 1];
    hmac(key->text, ltext, word, lword, tmp);
    hmac(state->s0, state->l0, tmp, M_KEY_OUTSIZ, buf + M_KEY_OUTSIZ * 0);
    hmac(state->s1, state->l1, tmp, M_KEY_OUTSIZ, buf + M_KEY_OUTSIZ * 1);
    hmac(state->s2, state->l2, tmp, M_KEY_OUTSIZ, buf + M_KEY_OUTSIZ * 2);
    hmac(state->s3, state->l3, tmp, M_KEY_OUTSIZ, buf + M_KEY_OUTSIZ * 3);

    unsigned char count = 0;
    unsigned char cnt[sizeof(state->ch)] = {0};
    unsigned int length = key->size < M_KEY_OUTSIZ ? key->size : M_KEY_OUTSIZ;
    *out = (char *)calloc(length + 1, sizeof(char));
    for (unsigned int i = 0; i != length; ++i)
    {
        int x = xdigit((buf + M_KEY_OUTSIZ * 0)[i]) +
                xdigit((buf + M_KEY_OUTSIZ * 1)[i]) +
                xdigit((buf + M_KEY_OUTSIZ * 2)[i]) +
                xdigit((buf + M_KEY_OUTSIZ * 3)[i]);
        tmp[i] = (unsigned char)x;

        switch (key->type)
        {
        case M_KEY_EMAIL:
        case M_KEY_CUSTOM:
        {
            while (cnt[x])
            {
                if (++x == sizeof(cnt))
                {
                    x = 0;
                }
            }
            ++cnt[x];

            (*out)[i] = state->ch[x];
        }
        break;

        case M_KEY_DIGIT:
        {
            x %= 10;

            int m = x;
            while (cnt[x] > count)
            {
                if (++x == 10)
                {
                    x = 0;
                }
                if (x == m)
                {
                    ++count;
                }
            }
            ++cnt[x];

            (*out)[i] = (char)('0' + x);
        }
        break;

        default:
            break;
        }
    }

    if (key->type == M_KEY_CUSTOM)
    {
        unsigned int lblob = (unsigned int)strlen(key->blob);
        for (unsigned int i = 0; i != lblob; ++i)
        {
            (*out)[tmp[i % M_KEY_OUTSIZ] % length] = key->blob[i];
        }
    }

    return 0;
}
