/**
 * @file        log.c
 * @copyright   2021 Andrew MacIsaac
 * @remark
 *      SPDX-License-Identifier: BSD-2-Clause
 *
 * @brief       Log function implementation for full (non-compact) log statements.
 */
#include "ll_log.h"

#include "common.h"

#include <assert.h>
#include <stdio.h>

/**
 * Calculate offset of next free character in buffer.
 *
 * @param   space   Space remaining in the buffer.
 *
 * @return Offset of free space.
 */
#define OFFSET(space) (LL_MAX_MESSAGE_SIZE - (space))

/// Local implementation if _ll_log is not inlined.
LL_DEFINE_INLINE void _ll_log
(
    struct ll_log   *log,
    enum ll_level    level,
    const char      *source,
    unsigned int     line,
    const char      *format,
    ...
);

#if LL_TIMESTAMP
static const char *write_timestamp
(
    char             *buffer,
    time_t           *seconds,
    unsigned long    *microseconds,
    size_t           *space
)
{
    int              n;
    struct tm        tm;
    unsigned long    milliseconds;

    // Obtain the current system time.
    LL_GET_TIME(seconds, microseconds);
    if (LL_CONVERT_TIME(*seconds, &tm) < 0)
    {
        return "Time conversion overflow!";
    }
    milliseconds = *microseconds / 1000U;

    // Write the first part of the time stamp (down to seconds).
    n = (int) strftime(buffer, *space, "%F %T", &tm);
    if (n == 0)
    {
        return "No space for time stamp!";
    }
    *space -= n;

    // Write the fractional seconds.
    n = snprintf(buffer + n, *space, ".%03u ", milliseconds);
    if (n < 5 || n >= *space)
    {
        return "No space for time stamp milliseconds!";
    }
    *space -= n;

    return NULL;
}
#endif /* end LL_TIMESTAMP */

#if !LL_CUSTOM_FORMAT
static const char *standard_format
(
    struct ll_log   *log,
    char            *buffer,
    time_t          *seconds,
    unsigned long   *microseconds,
    const char      *source,
    unsigned int     line,
    enum ll_level    level,
    const char      *format,
    va_list          args
)
{
    const char  *err = NULL;
    int          n;
    size_t       space = LL_MAX_MESSAGE_SIZE;

    // Message format is the following.  Square brackets indicate optional portions of the message.
    // [YYYY-MM-DD HH:MM:SS.mmm ]LEVL [file:line ]logger.name: Formatted log message

    // First, write the timestamp into the buffer, if so configured.
#if LL_TIMESTAMP
    err = write_timestamp(buffer, seconds, microseconds, &space);
    if (err != NULL)
    {
        return err;
    }
#endif /* end LL_TIMESTAMP */

    // Print the log level next.
    n = snprintf(buffer + OFFSET(space), space, "%5s ", LL_LEVEL_NAME(level));
    if (n < 5 || n >= (int) space)
    {
        return "No space for log level!";
    }
    space -= n;

#if LL_LOCATION
    // Print the file and line number, if so configured.
    n = snprintf(buffer + OFFSET(space), space, "%s:%u ", source, line);
    if (n < 3 || n >= (int) space)
    {
        return "No space for location info!";
    }
    space -= n;
#endif /* end LL_LOCATION */

    // Print the logger path into the buffer.
    n = get_path(log, buffer + OFFSET(space), space);
    if (n < 0 || n >= (int) space)
    {
        return "Log path too long!";
    }
    space -= n;

    // Next, print the rest of the preamble.
    n = snprintf(buffer + OFFSET(space), space, ": ");
    if (n >= (int) space)
    {
        return "Separator too long!";
    }
    space -= n;

    // Write the formatted message.
    n = vsnprintf(buffer + OFFSET(space), space, format, args);
    if (n >= (int) space)
    {
        return "Message too long!";
    }
    space -= n;

    return NULL;
}
#endif /* end LL_STDFORMAT */

/// Unconditionally log a message using a variable argument list.
void _ll_logv
(
    struct ll_log   *log,
    enum ll_level    level,
    const char      *source,
    unsigned int     line,
    const char      *format,
    va_list          args
)
{
    const char          *err = NULL;
    struct ll_log       *target_owner = NULL;
    struct ll_target    *target;
    time_t               seconds = 0;
    unsigned long        microseconds = 0;

    assert(log != NULL);
    if (level > get_threshold(log))
    {
        // Current level prohibits logging this message, so just return.
        return;
    }
    assert(format != NULL);
    assert(source != NULL);

    LL_ALLOCATE_BUFFER(buffer, LL_MAX_MESSAGE_SIZE);

#if LL_CUSTOM_FORMAT
#   error Custom format specifiers are not currently supported!
#else /* !LL_CUSTOM_FORMAT */
    err = standard_format(log, buffer, &seconds, &microseconds, source, line, level, format, args);
    if (err != NULL)
    {
        goto end;
    }
#endif /* end !LL_CUSTOM_FORMAT */

    // Pass the message buffer to the targets to write out.
    target = get_targets(log, &target_owner);
    while (target != NULL)
    {
        target->send(target, level, seconds, microseconds, buffer);
        target = target->next;
    }

end:
    if (target_owner != NULL)
    {
        UNLOCK(target_owner);
    }
    LL_RELEASE_BUFFER(buffer);

    if (err != NULL)
    {
        post_error(err, source, line);
    }
}
