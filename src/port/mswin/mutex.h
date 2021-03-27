/**
 * @file        port/mswin/mutex.h
 * @copyright   2021 Andrew MacIsaac
 * @remark
 *      SPDX-License-Identifier: BSD-2-Clause
 *
 * @brief       Mutex port implementation for Windows platforms.
 */
#ifndef PORT_MSWIN_MUTEX_H_
#define PORT_MSWIN_MUTEX_H_

#include "ll_internal.h"

#if HAVE_MSWIN_INIT_ONCE && HAVE_MSWIN_CRITICAL_SECTION
#   define _WIN32_WINNT 0x0600
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#   undef WIN32_LEAN_AND_MEAN
#   undef _WIN32_WINNT

#    include <assert.h>

/**
 * Static initializer for a mutex variable.
 */
#   define LL_STATIC_MUTEX_INIT { INIT_ONCE_STATIC_INIT }

/**
 * Perform one-time mutex initialization on Windows.
 *
 * @return Always TRUE.
 */
BOOL _ll_init_mutex
(
    PINIT_ONCE   init_block,    ///< One-time init block.
    PVOID        param,         ///< Pointer to mutex to initialize.
    PVOID       *ctx            ///< Unused context pointer.
);

/**
 * Lock a mutex (Windows critical section).  Windows does not have a static initializer for critical
 * section objects, so InitOnceExecuteOnce is used to achieve one-time initialization the first time
 * the mutex is locked.
 */
LL_DECLARE_INLINE void _ll_lock_mutex
(
    ll_mutex *mutex ///< Mutex instance pointer.
)
{
    assert(mutex != NULL);
    InitOnceExecuteOnce(&mutex->init_block, &_ll_init_mutex, mutex, NULL);
    EnterCriticalSection(&mutex->critical_section);
}

/**
 * Lock a mutex.
 *
 * @param   m   Mutex instance pointer.
 */
#   define LL_LOCK(m)   _ll_lock_mutex(m)

/**
 * Unlock a mutex.
 *
 * @param   m   Mutex instance pointer.
 */
#   define LL_UNLOCK(m) LeaveCriticalSection(&(m)->critical_section)

#endif /* end HAVE_MSWIN_INIT_ONCE && HAVE_MSWIN_CRITICAL_SECTION */

#endif /* end PORT_MSWIN_MUTEX_H_ */
