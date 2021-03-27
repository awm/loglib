#
# @file        CMakeLists.txt
# @copyright   2021 Andrew MacIsaac
# @remark
#      SPDX-License-Identifier: BSD-2-Clause
#
# @brief       Build instructions for loglib library.
#
include(CheckSymbolExists)

# Test for platform features.
check_symbol_exists(InitOnceExecuteOnce         "Windows.h"                 HAVE_MSWIN_INIT_ONCE)
check_symbol_exists(InitializeCriticalSection   "Windows.h"                 HAVE_MSWIN_CRITICAL_SECTION)
check_symbol_exists(PTHREAD_MUTEX_INITIALIZER   "pthread.h"                 HAVE_PTHREAD_MUTEX)
check_symbol_exists(_ftime_s                    "sys/types.h;sys/timeb.h"   HAVE__FTIME_S)
check_symbol_exists(gettimeofday                "sys/time.h"                HAVE_GETTIMEOFDAY)
check_symbol_exists(gmtime_r                    "time.h"                    HAVE_GMTIME_R)
check_symbol_exists(gmtime_s                    "time.h"                    HAVE_GMTIME_S)
check_symbol_exists(localtime_r                 "time.h"                    HAVE_LOCALTIME_R)
check_symbol_exists(localtime_s                 "time.h"                    HAVE_LOCALTIME_S)
check_symbol_exists(xSemaphoreCreateMutex       "FreeRTOS.h;semphr.h"       HAVE_FREERTOS_SEMAPHORE)
check_symbol_exists(xSemaphoreCreateMutexStatic "FreeRTOS.h;semphr.h"       HAVE_FREERTOS_STATIC_SEMAPHORE)
check_symbol_exists(xTaskGetSchedulerState      "FreeRTOS.h;task.h"         HAVE_FREERTOS_XTASKGETSCHEDULERSTATE)

configure_file(ll_features.h.in include/ll_features.h NEWLINE_STYLE UNIX)

# Set include paths for library.
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

# Define static loglib library.
set(
    SRCS
    port/freertos/mutex.c
    port/mswin/gettime.c
    port/mswin/mutex.c
    port/posix/gettime.c

    common.c
    log.c
)
add_library(log STATIC ${SRCS})
