/**
 * @file        port/stdc/gettime.h
 * @copyright   2021 Andrew MacIsaac
 * @remark
 *      SPDX-License-Identifier: BSD-2-Clause
 *
 * @brief       Port implementation to obtain time using only standard C library calls.
 *              This port is limited to a resolution of one second.
 */
#ifndef PORT_STDC_GETTIME_H_
#define PORT_STDC_GETTIME_H_

#include "ll_internal.h"

#if !LL_THREADING
#   include <time.h>

/**
 * Retrieve the current system time.
 *
 * @param[out]  secondsp        Seconds since the Epoch.
 * @param[out]  microsecondsp   Fraction of the current second, in microseconds.
 */
#   define LL_GET_TIME(secondsp, microsecondsp) \
    do                                          \
    {                                           \
        *secondsp = time(NULL);                 \
        *microsecondsp = 0;                     \
    } while (0)

#endif /* end !LL_THREADING */

#endif /* end PORT_STDC_GETTIME_H_ */
