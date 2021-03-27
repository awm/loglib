/**
 * @file        port/mswin/mutex.c
 * @copyright   2021 Andrew MacIsaac
 * @remark
 *      SPDX-License-Identifier: BSD-2-Clause
 *
 * @brief       Mutex port implementation for Windows platforms.
 */
#include "mutex.h"

#if LL_THREADING
#   if HAVE_MSWIN_INIT_ONCE && HAVE_MSWIN_CRITICAL_SECTION

/// Shared implementation if _ll_lock_mutex is not inlined.
LL_DEFINE_INLINE void _ll_lock_mutex(ll_mutex *mutex);

/// Perform one-time mutex initialization on Windows.
BOOL _ll_init_mutex(PINIT_ONCE init_once, PVOID param, PVOID *ctx)
{
    ll_mutex *mutex = (ll_mutex *) param;
    assert(mutex != NULL);

    LL_UNUSED(init_once);
    LL_UNUSED(ctx);

    InitializeCriticalSection(&mutex->critical_section);
    return TRUE;
}

#   endif /* end HAVE_MSWIN_INIT_ONCE && HAVE_MSWIN_CRITICAL_SECTION */
#endif /* end LL_THREADING */
