/**
 * @file        port/mswin/gettime.c
 * @copyright   2021 Andrew MacIsaac
 * @remark
 *      SPDX-License-Identifier: BSD-2-Clause
 *
 * @brief       Port implementation to obtain time on Windows platforms.
 */
#include "gettime.h"

#if LL_TIMESTAMP
#   if HAVE__FTIME_S

/// Shared implementation if _ll_get_time is not inlined.
LL_DEFINE_INLINE void _ll_get_time(time_t *secondsp, unsigned long *microsecondsp);

#   endif /* end HAVE__FTIME_S */
#endif /* end LL_TIMESTAMP */
