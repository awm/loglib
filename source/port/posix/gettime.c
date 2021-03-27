/**
 * @file        port/posix/gettime.c
 * @copyright   2021 Andrew MacIsaac
 * @remark
 *      SPDX-License-Identifier: BSD-2-Clause
 *
 * @brief       Port implementation to obtain time on POSIX platforms.
 */
#include "ll_internal.h"

#if LL_TIMESTAMP
#   include "gettime.h"

#   if HAVE_GETTIMEOFDAY

/// Shared implementation if _ll_get_time is not inlined.
LL_DEFINE_INLINE void _ll_get_time(time_t *secondsp, unsigned long *microsecondsp);

#   endif /* end HAVE_GETTIMEOFDAY */
#endif /* end LL_TIMESTAMP */
