/**
 * @file        ll_config.h
 * @copyright   2021 Andrew MacIsaac
 * @remark
 *      SPDX-License-Identifier: BSD-2-Clause
 *
 * @brief       Default loglib compile time feature configuration.
 *              This can be overridden by defining LL_CONFIG to be the path to an alternate header.
 */
#ifndef LL_CONFIG_H
#define LL_CONFIG_H

#include "ll_features.h"

#include <stdint.h>

/// Type to hold a static hash value.
typedef uint32_t ll_hash_t;

/// Available log levels.  The value LL_LEVEL_INHERIT must be defined and be the last value.
enum ll_level
{
    LL_LEVEL_FATAL,     ///< Fatal error message.  Typically used immediately before abnormal
                        ///< program termination.
    LL_LEVEL_ERROR,     ///< Error condition.
    LL_LEVEL_WARN,      ///< Warning of an undesirable state.
    LL_LEVEL_INFO,      ///< Informational message.
    LL_LEVEL_DEBUG,     ///< Message to assist in debugging.
    LL_LEVEL_TRACE,     ///< Detailed and verbose message to trace a particular aspect of program
                        ///< state.

    LL_LEVEL_INHERIT    ///< Used to indicate that a log instance should inherit its level from its
                        ///< parent.
};

/**
 * Convert an enum ll_level value into a string suitable for display in a log message.  This must
 * support all listed enum values.  No value less than 0 or greater than or equal to
 * LL_LEVEL_INHERIT will be passed to this macro, allowing the mapping to be efficiently
 * implemented using an array.
 *
 * @param   level   Level value.
 *
 * @return  Constant string representation of the log level.  To maintain the best formatting, it is
 *          recommended that this be four or fewer characters.
 */
#define LL_LEVEL_NAME(level)    (_ll_log_level_name[level])

/// Default mapping of level value to name.
extern const char *_ll_log_level_name[LL_LEVEL_INHERIT];

/// Ensure the default mapping is enabled.
#define LL_DEFAULT_LEVEL_MAPPING 1

/// Log statements with a level greater than this will be omitted entirely and cannot be turned on
/// or off dynamically.
#define LL_STATIC_MAX_LEVEL LL_LEVEL_DEBUG

/// Maximum size of message buffer, in bytes.  Must be greater than or equal to 8.
#define LL_MAX_MESSAGE_SIZE 1024

/**
 * Allocate message buffer.  In this case, allocate on the stack.  The character buffer "name" must
 * be created by this macro.
 *
 * @param   name    Buffer variable name.
 * @param   size    Size of the requested buffer, in bytes.  This will always be a constant value.
 */
#define LL_ALLOCATE_BUFFER(name, size) char name[size]

/**
 * Release the message buffer.  In this case, because the buffer is allocated on the stack, no
 * action is required.
 *
 * @param   name    Buffer variable name.
 */
#define LL_RELEASE_BUFFER(name)

/// Enable file:line information in standard log messages.
#define LL_LOCATION      1

/// Enable time stamps in standard log messages.
#define LL_TIMESTAMP     1

/// Use local time rather than UTC for time stamps.
#define LL_LOCALTIME     0

/// Enable support for customised log messages (TBD).
#define LL_CUSTOM_FORMAT 0

/// Enable compact binary log messages.
#define LL_COMPACT       0

/// Enable threading support.
#define LL_THREADING     1

/**
 * @section mutex   Mutex Definitions
 *                  When threading support is enabled, the mutex type must be publically defined for
 *                  use in static log instances.  This section includes the necessary definitions.
 */
#if LL_THREADING
#   if !LL_MUTEX_IMPLEMENTED
#       if HAVE_FREERTOS_STATIC_SEMAPHORE
#           include <FreeRTOS.h>
#           include <semphr.h>

/// FreeRTOS mutex structure.
typedef struct _ll_freertos_mutex
{
    volatile int        init_state; ///< Initialization state.
    StaticSemaphore_t   mutex;      ///< Mutex variable.
} ll_mutex;

#       elif HAVE_FREERTOS_SEMAPHORE
#           include <FreeRTOS.h>
#           include <semphr.h>

/// FreeRTOS mutex structure.
typedef struct _ll_freertos_mutex
{
    volatile int    init_state; ///< Initialization state.
    Semaphore_t     mutex;      ///< Mutex variable.
} ll_mutex;

#       elif HAVE_MSWIN_INIT_ONCE && HAVE_MSWIN_CRITICAL_SECTION
#           define _WIN32_WINNT 0x0600
#           define WIN32_LEAN_AND_MEAN
#           include <Windows.h>
#           undef WIN32_LEAN_AND_MEAN
#           undef _WIN32_WINNT

/// Windows mutex structure.
typedef struct _ll_mswin_mutex
{
    INIT_ONCE           init_block;         ///< Init block to support on-demand one-time
                                            ///< initialization.
    CRITICAL_SECTION    critical_section;   ///< Critical section to provide mutex functionality.
} ll_mutex;

#       elif HAVE_PTHREAD_MUTEX
#           include <pthread.h>

/// Mutex type.
typedef pthread_mutex ll_mutex;

#       endif /* end HAVE_PTHREAD_MUTEX */
#   endif /* end !LL_MUTEX_IMPLEMENTED */
#endif /* end LL_THREADING */

#endif /* end LL_CONFIG_H */
