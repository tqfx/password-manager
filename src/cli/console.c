/*!
 @file console.c
 @brief console
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/
#include "console.h"

#include <stdarg.h>
#include <stdlib.h>

#if defined(_WIN32)
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 5105)
#endif /* _MSC_VER */
#include <windows.h>
#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */
#endif /* _WIN32 */

static struct
{
    unsigned int ft;
    unsigned int bg;
} console_text = {
    .ft = 7,
    .bg = 0,
};

void console_text_reset(void)
{
    console_text.ft = 7;
    console_text.bg = 0;
#if defined(_WIN32)
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, CONSOLE_TEXT_DEFAULT);
#else
    printf("\033[0m");
#endif /* _WIN32 */
}

unsigned int console_text_set(unsigned int ft, unsigned int bg)
{
    static unsigned int map[] = {
    /* clang-format off */
#if defined(_WIN32)
        0, 4, 6, 2, 3, 1, 5, 7,
#else
        0, 1, 3, 2, 6, 4, 5, 7,
#endif /* _WIN32 */
    };
    /* clang-format on */
    unsigned int ret = (bg << 4) + ft;
    ft = map[ft % (sizeof(map) / sizeof(*map))];
    bg = map[bg % (sizeof(map) / sizeof(*map))];
#if defined(_WIN32)
    if (console_text.ft != ft || console_text.bg != bg)
    {
        WORD x = (WORD)((bg << 4) + ft);
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handle, x);
        console_text.ft = ft;
        console_text.bg = bg;
    }
#else
    if (console_text.ft != ft)
    {
        printf("\033[3%um", ft);
        console_text.ft = ft;
    }
    if (console_text.bg != bg)
    {
        printf("\033[4%um", bg);
        console_text.bg = bg;
    }
#endif /* _WIN32 */
    return ret;
}

int console_vprint(const char *sep, unsigned int n, va_list ap)
{
    int ret = 0;
    while (n--)
    {
        unsigned int x = va_arg(ap, unsigned int);
        console_text_set(x, x >> 4);
        const char *str = va_arg(ap, const char *);
        if (str)
        {
            ret += printf("%s", str);
            if (sep && n)
            {
                ret += printf("%s", sep);
            }
        }
    }
    console_text_reset();
    return ret;
}

int console_printf(unsigned int n, ...)
{
    int ret = 0;
    va_list ap;
    va_start(ap, n);
    while (n--)
    {
        unsigned int x = va_arg(ap, unsigned int);
        console_text_set(x, x >> 4);
        const char *str = va_arg(ap, const char *);
        if (str)
        {
            ret += printf("%s", str);
        }
    }
    console_text_reset();
    va_end(ap);
    return ret;
}

int console_print(const char *sep, unsigned int n, ...)
{
    va_list ap;
    va_start(ap, n);
    int ret = console_vprint(sep, n, ap);
    va_end(ap);
    return ret;
}

int console_println(const char *sep, unsigned int n, ...)
{
    va_list ap;
    va_start(ap, n);
    int ret = console_vprint(sep, n, ap) + 1;
    putchar('\n');
    va_end(ap);
    return ret;
}

int console_aprint(const char *sep, unsigned int n, unsigned int *x, const char **s)
{
    int ret = 0;
    while (n--)
    {
        console_text_set(*x, *x >> 4);
        if (*s)
        {
            ret += printf("%s", *s);
            if (sep && n)
            {
                ret += printf("%s", sep);
            }
        }
        ++x;
        ++s;
    }
    console_text_reset();
    return ret;
}

int console_aprintln(const char *sep, unsigned int n, unsigned int *x, const char **s)
{
    int ret = console_aprint(sep, n, x, s) + 1;
    putchar('\n');
    return ret;
}
