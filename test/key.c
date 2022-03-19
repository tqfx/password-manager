/*!
 @file key.c
 @brief key manager testing
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#include "m_key.h"

#include <stdio.h>

int main(void)
{
    m_key_s *ctx = m_key_new();

    m_key_set_size(ctx, 0xFF);
    m_key_set_text(ctx, "test");
    m_key_set_text(ctx, "test");
    m_key_set_type(ctx, M_KEY_CUSTOM);
    m_key_set_blob(ctx, "ABCD");
    m_key_set_blob(ctx, "ABCD");
    m_key_set_url(ctx, "www.example.com");
    m_key_set_url(ctx, "www.example.com");

    printf("url: %s\n", m_key_url(ctx));
    printf("text: %s\n", m_key_text(ctx));
    printf("blob: %s\n", m_key_blob(ctx));
    printf("type: %u\n", m_key_type(ctx));
    printf("size: %u\n", m_key_size(ctx));

    m_key_delete(ctx);
    return 0;
}
