/**
 * @file        port/mswin/localtime.h
 * @copyright   2021 Andrew MacIsaac
 * @remark
 *      SPDX-License-Identifier: BSD-2-Clause
 *
 * @brief       Local time conversion port implementation for Windows platforms.
 */
#ifndef PORT_MSWIN_LOCALTIME_H_
#define PORT_MSWIN_LOCALTIME_H_

#include "ll_internal.h"

#if HAVE_LOCALTIME_S
#   include <time.h>

/**
 *  Time stamp conversion.
 *
 * @param[in]   seconds Time to convert, in seconds since the Epoch.
 * @param[out]  ptm     Returned tm structure containing the converted time.
 *
 * @retval  0   Success.
 * @retval  <0  Conversion error.
 */
#   define LL_CONVERT_TIME(seconds, ptm) (localtime_s((ptm), &(seconds)) == 0 ? 0 : -1)
#endif /* end HAVE_LOCALTIME_S */

#endif /* end PORT_MSWIN_LOCALTIME_H_ */
