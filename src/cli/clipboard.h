/*!
 @file clipboard.h
 @brief clipboard
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#pragma once
#ifndef __CLIPBOARD_H__
#define __CLIPBOARD_H__

#include <stdlib.h>

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 @brief Set data to clipboard
 @param[in] pdata points to data
 @param[in] nbyte length of data
 @return the execution state of the function
  @retval -1 failure
  @retval 0 success
*/
int clipboard_set(const void *pdata, size_t nbyte);

/*!
 @brief Set string to clipboard
 @param[in] in points to string
 @return the execution state of the function
  @retval -1 failure
  @retval 0 success
*/
int clipboard_sets(const void *in);

/*!
 @brief Get data of clipboard
 @param[out] out points to data address
 @return the execution state of the function
  @retval -1 failure
  @retval 0 success
*/
int clipboard_get(char **out);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CLIPBOARD_H__ */
