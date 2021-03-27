/**
 * @file        port/posix/gettime.h
 * @copyright   2021 Andrew MacIsaac
 * @remark
 *      SPDX-License-Identifier: BSD-2-Clause
 *
 * @brief       Port implementation to obtain time on POSIX platforms.
 */
#ifndef PORT_POSIX_GETTIME_H_
#define PORT_POSIX_GETTIME_H_

#include "ll_internal.h"

#if HAVE_GETTIMEOFDAY
#   include <sys/time.h>

/**
 * Retrieve the current system time using POSIX gettimeofday().
 */
LL_DECLARE_INLINE void _ll_get_time
(
    time_t          *secondsp,      ///< [out]  Seconds since the Epoch.
    unsigned long   *microsecondsp  ///< [out]  Fraction of the current second, in microseconds.
)
{
    struct timeval now;

    gettimeofday(&now, NULL);
    *secondsp       = (time_t) now.tv_sec;
    *microsecondsp  = (unsigned long) now.tv_usec;
}

/**
 * Retrieve the current system time.
 *
 * @param[out]  secondsp        Seconds since the Epoch.
 * @param[out]  microsecondsp   Fraction of the current second, in microseconds.
 */
#   define LL_GET_TIME(secondsp, microsecondsp) _ll_get_time((secondsp), (microsecondsp))

#endif /* end HAVE_GETTIMEOFDAY */

#endif /* end PORT_POSIX_GETTIME_H_ */
