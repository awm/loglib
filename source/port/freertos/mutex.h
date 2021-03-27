/**
 * @file        port/freertos/mutex.h
 * @copyright   2021 Andrew MacIsaac
 * @remark
 *      SPDX-License-Identifier: BSD-2-Clause
 *
 * @brief       Port implementation for FreeRTOS platforms.
 *              Logging functions may be called both before and after the scheduler is started.
 */
#ifndef PORT_FREERTOS_MUTEX_H_
#define PORT_FREERTOS_MUTEX_H_

#include "ll_internal.h"

#if (HAVE_FREERTOS_STATIC_SEMAPHORE || HAVE_FREERTOS_SEMAPHORE) && \
    HAVE_FREERTOS_XTASKGETSCHEDULERSTATE
#    include <FreeRTOS.h>
#    include <task.h>

#    include <assert.h>

#   ifndef LL_IN_ISR
#       error No hook defined to determine if execution is currently in an ISR!
#   endif

/// Static initializer for a mutex variable.
#   define LL_STATIC_MUTEX_INIT { false }

/**
 * Lock a FreeRTOS mutex.  FreeRTOS does not have a static initializer for mutex objects, so we
 * perform one-time initialization the first time the mutex is locked.
 */
LL_DECLARE_INLINE void _ll_lock_mutex
(
    ll_mutex *mutex ///< Mutex instance.
)
{
#   if HAVE_FREERTOS_STATIC_SEMAPHORE
    SemaphoreHandle_t sem = &mutex->mutex;
#   elif HAVE_FREERTOS_SEMAPHORE
    SemaphoreHandle_t sem = mutex->mutex;
#   endif

    // Don't lock if we are in an ISR.  This could result in interlaced output for nested
    // interrupts, but trying to enforce log ordering in this scenario is fraught with peril and may
    // result in deadlocks or poor interrupt performance.  If logging from an ISR is essential, use
    // a dedicated ISR logger for each level and buffer the entries, dumping them when the ISR
    // completes.
    if (!LL_IN_ISR())
    {
        if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
        {
            vTaskSuspendAll();
            if (!mutex->initialized)
            {
#   if HAVE_FREERTOS_STATIC_SEMAPHORE
                sem = xSemaphoreCreateMutexStatic(&mutex->mutex);
#   elif HAVE_FREERTOS_SEMAPHORE
                sem = xSemaphoreCreateMutex();
#   endif
                mutex->initialized = true;
            }
            xTaskResumeAll();

            assert(sem != NULL);
            xSemaphoreTake(sem, portMAX_DELAY);
        }
    }
}

/**
 * Unlock a FreeRTOS mutex.
 */
LL_DECLARE_INLINE void _ll_unlock_mutex
(
    ll_mutex *mutex ///< Mutex instance.
)
{
#       if HAVE_FREERTOS_STATIC_SEMAPHORE
    SemaphoreHandle_t sem = &mutex->mutex;
#       elif HAVE_FREERTOS_SEMAPHORE
    SemaphoreHandle_t sem = mutex->mutex;
#       endif

    if (!LL_IN_ISR())
    {
        if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
        {
            xSemaphoreGive(sem);
        }
    }
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
#   define LL_UNLOCK(m) _ll_unlock_mutex(m)

#endif /* end (HAVE_FREERTOS_STATIC_SEMAPHORE || HAVE_FREERTOS_SEMAPHORE) &&
              HAVE_FREERTOS_XTASKGETSCHEDULERSTATE */

#endif /* end PORT_FREERTOS_MUTEX_H_ */
