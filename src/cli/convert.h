/*!
 @file convert.h
 @brief code conversion
 @copyright Copyright (C) 2020 tqfx. All rights reserved.
*/

#pragma once
#ifndef __CONVERT_H__
#define __CONVERT_H__

#include <stdlib.h>

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 @brief source charset transcoding destination charset
 @note Note the support for C language localization
 @param[out] out: pointer of buffer variable
 @param[in] out_charset: charset of destination string
 @param[in] in: source string
 @param[in] in_charset: charset of source string
 @return the execution state of the function
  @retval -1 failure
  @retval 0 success
*/
#if defined(_WIN32)
int code_convert(char **out, unsigned int out_charset,
                 const void *in, unsigned int in_charset);
#else
int code_convert(char **out, const char *out_charset,
                 const void *in, const char *in_charset);
#endif /* _WIN32 */

/*!
 @brief code transcoding utf-8
 @note Note the support for C language localization
 @param[out] out: pointer of buffer variable
 @param[in] in: source string
 @return the execution state of the function
  @retval -1 failure
  @retval 0 success
*/
int code_to_utf8(char **out, const void *in);

/*!
 @brief utf-8 transcoding code
 @note Note the support for C language localization
 @param[out] out: pointer of buffer variable
 @param[in] in: source string
 @return the execution state of the function
  @retval -1 failure
  @retval 0 success
*/
int code_utf8_to(char **out, const void *in);

/*!
 @brief utf-8 transcoding gbk
 @note Note the support for C language localization
 @param[out] out: pointer of buffer variable
 @param[in] in: source string
 @return the execution state of the function
  @retval -1 failure
  @retval 0 success
*/
int utf8_gbk(char **out, const void *in);

/*!
 @brief gbk transcoding utf-8
 @note Note the support for C language localization
 @param[out] out: pointer of buffer variable
 @param[in] in: source string
 @return the execution state of the function
  @retval -1 failure
  @retval 0 success
*/
int gbk_utf8(char **out, const void *in);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CONVERT_H__ */
