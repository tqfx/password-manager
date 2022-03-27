/*!
 @file generate.c
 @brief generate manager testing
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#include "m_generate.h"

#include <stdio.h>

int main(void)
{
    static char txt[] = "txt";
    static char text[] = "test";
    static char blob[] = ".";
    m_key_s *ctx = m_key_new();

    char *result = 0;
    m_generate_setrule(0, 0, 0, 0);
    m_key_set_blob(ctx, 0);
    m_key_set_blob(ctx, blob);
    m_key_set_text(ctx, text);
    m_key_set_text(ctx, text);

    {
        m_key_set_type(ctx, M_KEY_EMAIL);
        m_generate1(ctx, txt, &result);
        printf("%s\n", result);
        free(result);
        result = 0;

        m_key_set_type(ctx, M_KEY_DIGIT);
        m_generate1(ctx, txt, &result);
        printf("%s\n", result);
        free(result);
        result = 0;

        m_key_set_type(ctx, M_KEY_CUSTOM);
        m_generate1(ctx, txt, &result);
        printf("%s\n", result);
        free(result);
        result = 0;
    }

    {
        m_key_set_type(ctx, M_KEY_EMAIL);
        m_generate2(ctx, txt, &result);
        printf("%s\n", result);
        free(result);
        result = 0;

        m_key_set_type(ctx, M_KEY_DIGIT);
        m_generate2(ctx, txt, &result);
        printf("%s\n", result);
        free(result);
        result = 0;

        m_key_set_type(ctx, M_KEY_CUSTOM);
        m_generate2(ctx, txt, &result);
        printf("%s\n", result);
        free(result);
        result = 0;
    }

    m_key_die(ctx);
    return 0;
}
