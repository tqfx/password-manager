/*!
 @file path.c
 @brief Path handling
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "path.h"

#if defined(_WIN32)
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 5105)
#endif /* _MSC_VER */
#include <windows.h>
#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */
#else
#include <unistd.h>
#endif /* _WIN32 */

char *path_self(void)
{
    char *self = 0;
    size_t length = 0;
    size_t size = 0x10;
    do
    {
        size <<= 1;
        char *ptr = (char *)realloc(self, size);
        if (ptr == 0)
        {
            break;
        }
        self = ptr;
#if defined(_WIN32)
        length = GetModuleFileName(NULL, self, (DWORD)(size - 1));
#else
        length = (size_t)readlink("/proc/self/exe", self, size - 1);
#endif /* _WIN32 */
    } while (length >= size - 1);
    if (self)
    {
        self[length] = 0;
    }
    return self;
}
