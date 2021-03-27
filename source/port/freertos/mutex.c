/**
 * @file        port/freertos/mutex.c
 * @copyright   2021 Andrew MacIsaac
 * @remark
 *      SPDX-License-Identifier: BSD-2-Clause
 *
 * @brief       Port implementation for FreeRTOS platforms.
 */
#include "ll_internal.h"

#if LL_THREADING
#      include "mutex.h"

#   if (HAVE_FREERTOS_STATIC_SEMAPHORE) || defined(HAVE_FREERTOS_SEMAPHORE) && \
       HAVE_FREERTOS_XTASKGETSCHEDULERSTATE

/// Shared implementation if _ll_lock_mutex is not inlined.
LL_DEFINE_INLINE void _ll_lock_mutex(ll_mutex *mutex);

/// Shared implementation if _ll_unlock_mutex is not inlined.
LL_DEFINE_INLINE void _ll_unlock_mutex(ll_mutex *mutex);

#   endif /* end (HAVE_FREERTOS_STATIC_SEMAPHORE || HAVE_FREERTOS_SEMAPHORE) &&
                 HAVE_FREERTOS_XTASKGETSCHEDULERSTATE */
#endif /* end LL_THREADING */
