/**
 * @file        ll_internal.h
 * @copyright   2021 Andrew MacIsaac
 * @remark
 *      SPDX-License-Identifier: BSD-2-Clause
 *
 * @brief       Private loglib API.
 *              These functions and macros are used to compose the public API and should not be
 *              called directly.
 */
#ifndef LL_INTERNAL_H
#define LL_INTERNAL_H

// Use default configuration unless overridden.
#ifdef LL_CONFIG
#   include LL_CONFIG
#else
#   include "ll_config.h"
#endif

#include <stdarg.h>
#include <time.h>

/// Mark a parameter as unused.
#define LL_UNUSED(x) ((void) (x))

/**
 * @def LL_DECLARE_INLINE
 *
 * Declare an inline function in a header.
 */
#ifndef LL_DECLARE_INLINE
#   if __GNUC_GNU_INLINE__
#       define LL_DECLARE_INLINE extern inline
#   elif __GNUC__ || __clang__
#       define LL_DECLARE_INLINE inline
#   else
#       define LL_DECLARE_INLINE static inline
#   endif
#endif /* end !defined(LL_DECLARE_INLINE) */

/**
 * @def LL_DEFINE_INLINE
 *
 * Emit a function definition for a declared inline function, if the compiler decides not to inline
 * it.
 */
#ifndef LL_DEFINE_INLINE
#   if __GNUC_GNU_INLINE__
#       define LL_DEFINE_INLINE inline
#   elif __GNUC__ || __clang__
#       define LL_DEFINE_INLINE extern inline
#   else
#       define LL_DEFINE_INLINE static inline
#   endif
#endif /* end !defined(LL_DEFINE_INLINE) */


// Forward reference.
struct ll_target;

/**
 * Send a log message to a log target.
 */
typedef void (*ll_send_func)
(
    struct ll_target    *target,        ///< Target instance.
    enum ll_level        level,         ///< Message level.
    time_t               seconds,       ///< Time stamp in seconds.
    unsigned long        microseconds,  ///< Time stamp fraction of a second in microseconds.
    const char          *message        ///< Message text.
);

/**
 * Log target object.  Handles sending log output to a particular sink.
 */
struct ll_target
{
    struct ll_target    *next;  ///< Next target instance.
    ll_send_func         send;  ///< Function to write out log message.
};

/**
 *  Logger object.  Represents a log to which messages can be written.
 */
struct ll_log
{
    struct ll_log       *parent;    ///< Parent log instance.
    struct ll_log       *next;      ///< Next sibling log instance.
    struct ll_log       *children;  ///< Child log instances.

    const char          *name;      ///< Name of this log instance.
    const char          *prefix;    ///< Prefix to prepend to each log message.  Not currently
                                    ///< implemented.
    enum ll_level        level;     ///< Threshold below which to pass log messages.
    struct ll_target    *targets;   ///< Target(s) to write log messages to.

#if LL_THREADING
    ll_mutex             mutex;     ///< Mutex used to serialize log accesses.
#endif
};

/**
 * Unconditionally log a message using a variable argument list.
 */
void _ll_logv
(
    struct ll_log   *log,       ///< Log handle.
    enum ll_level    level,     ///< Level of log message.
    const char      *source,    ///< Source file of log statement.
    unsigned int     line,      ///< Source line number of log statement.
    const char      *format,    ///< Message format string.  This must be a string literal.
    va_list          args       ///< Positional parameters of format string.
);

/**
 * Unconditionally log a message using positional parameters.
 */
LL_DECLARE_INLINE void _ll_log
(
    struct ll_log   *log,       ///< Log handle.
    enum ll_level    level,     ///< Level of log message.
    const char      *source,    ///< Source file of log statement.
    unsigned int     line,      ///< Source line number of log statement.
    const char      *format,    ///< Message format string.  This must be a string literal.
    ...                         ///< Positional parameters of format string.
)
{
    va_list args;
    va_start(args, format);
    _ll_logv(log, level, source, line, format, args);
    va_end(args);
}

/**
 * Unconditionally log a compact message using a variable argument list.
 *
 * The file and line information is omitted, as it is collected by the log extractor tool during
 * preprocessing.
 */
void _ll_clogv
(
    struct ll_log   *log,       ///< Log handle.
    enum ll_level    level,     ///< Level of log message.
    const char      *source,    ///< Source file of log statement.
    unsigned int     line,      ///< Source line number of log statement.
    ll_hash_t        hash,      ///< Compile-time hash of format string.
    va_list          args       ///< Positional parameters of format string.
);

/**
 * Unconditionally log a compact message using positional parameters.
 *
 * The file and line information is omitted, as it is collected by the log extractor tool during
 * preprocessing.
 */
LL_DECLARE_INLINE void _ll_clog
(
    struct ll_log   *log,       ///< Log handle.
    enum ll_level    level,     ///< Level of log message.
    const char      *source,    ///< Source file of log statement.
    unsigned int     line,      ///< Source line number of log statement.
    ll_hash_t        hash,      ///< Compile-time hash of format string.
    ...                         ///< Positional parameters of format string.
)
{
    va_list args;
    va_start(args, hash);
    _ll_clogv(log, level, source, line, hash, args);
    va_end(args);
}

/**
 * @def _LL_LOG(log, level, format, ...)
 * Unconditionally log a message using a variable argument list.
 *
 * This macro expands to the appropriate log function invocation based on the log compaction
 * setting and format parameters.
 *
 * The implementation here uses a compile-time trick to determine if there are any variable
 * arguments provided, by detecting if the variable argument list is an empty string when
 * stringized.  Even a single empty string parameter will be converted to "\"\"", so an entirely
 * empty string can be used to make the appropriate choice of call at compile time.  This avoids
 * having to detect which compiler is in use and avoids the whole question of dangling commas, ##,
 * and __VA_OPT__.
 *
 * @param  log     Log handle.
 * @param  level   Level of log message.
 * @param  format  Message format string.  This must be a string literal.
 * @param  args    Positional parameters of format string.
 */

/**
 * @def _LL_LOGV(log, level, format, args)
 * Unconditionally log a message using positional parameters.
 *
 * This macro expands to the appropriate log function invocation based on the log compaction
 * setting and format parameters.
 *
 * @param  log     Log handle.
 * @param  level   Level of log message.
 * @param  format  Message format string.  This must be a string literal.
 * @param  ...     Positional parameters of format string.
 */

#if LL_COMPACT
#   define __LL_LOG(log, level, format, ...)                                    \
        ((#__VA_ARGS__[0] == '\0')                                          ?   \
            _ll_clog((log), (level), __FILE__, __LINE__, LL_HASH(format))   :   \
            _ll_clog((log), (level), __FILE__, __LINE__, LL_HASH(format), __VA_ARGS__))
#   define __LL_LOGV(log, level, format, args) \
        _ll_clogv((log), (level), __FILE__, __LINE__, LL_HASH(format), (args))
#else /* !LL_COMPACT */
#   define __LL_LOG(log, level, format, ...)                            \
        ((#__VA_ARGS__[0] == '\0')                                  ?   \
            _ll_log((log), (level), __FILE__, __LINE__, (format))   :   \
            _ll_log((log), (level), __FILE__, __LINE__, (format), __VA_ARGS__))
#   define __LL_LOGV(log, level, format, args) \
        _ll_logv((log), (level), __FILE__, __LINE__, (format), (args))
#endif /* end !LL_COMPACT */

#endif /* end LL_INTERNAL_H */
