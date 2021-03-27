/**
 * @file        port/stc/localtime.h
 * @copyright   2021 Andrew MacIsaac
 * @remark
 *      SPDX-License-Identifier: BSD-2-Clause
 *
 * @brief       Local time conversion port implementation using only standard C library calls.
 *              This port is limited to single-threaded environments as it uses library calls which
 *              are not thread-safe.
 */
#ifndef PORT_STDC_LOCALTIME_H_
#define PORT_STDC_LOCALTIME_H_

#include "ll_internal.h"

#if !LL_THREADING
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
#   define LL_CONVERT_TIME(seconds, ptm) ((*(ptm) = *localtime(&(seconds))), 0)
#endif /* end !LL_THREADING */

#endif /* end PORT_STDC_LOCALTIME_H_ */
