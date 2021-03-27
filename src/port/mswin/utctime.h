/**
 * @file        port/mswin/utctime.h
 * @copyright   2021 Andrew MacIsaac
 * @remark
 *      SPDX-License-Identifier: BSD-2-Clause
 *
 * @brief       UTC time conversion port implementation for Windows platforms.
 */
#ifndef PORT_MSWIN_UTCTIME_H_
#define PORT_MSWIN_UTCTIME_H_

#include "ll_internal.h"

#if HAVE_GMTIME_S
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
#   define LL_CONVERT_TIME(seconds, ptm) (gmtime_s((ptm), &(seconds)) == 0 ? 0 : -1)
#endif /* end HAVE_GMTIME_S */

#endif /* end PORT_MSWIN_UTCTIME_H_ */
