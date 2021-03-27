/**
 * @file        port/posix/mutex.h
 * @copyright   2021 Andrew MacIsaac
 * @remark
 *      SPDX-License-Identifier: BSD-2-Clause
 *
 * @brief       Mutex port implementation for POSIX platforms.
 */
#ifndef PORT_POSIX_MUTEX_H_
#define PORT_POSIX_MUTEX_H_

#include "ll_internal.h"

#if HAVE_PTHREAD_MUTEX
#   include <pthread.h>

/**
 * Static initializer for a mutex variable.
 */
#   define LL_STATIC_MUTEX_INIT     PTHREAD_MUTEX_INITIALIZER

/**
 * Lock a mutex.
 *
 * @param   m   Mutex instance pointer.
 */
#   define LL_LOCK(m)               pthread_mutex_lock(m)

/**
 * Unlock a mutex.
 *
 * @param   m   Mutex instance pointer.
 */
#   define LL_UNLOCK(m)             pthread_mutex_unlock(m)

#endif /* end HAVE_PTHREAD_MUTEX */

#endif /* end PORT_POSIX_MUTEX_H_ */
