#include "ll_log.h"

#include "common.h"

#include <assert.h>
#include <stdio.h>

/// Local implementation if _ll_log is not inlined.
extern inline void _ll_log
(
    struct ll_log   *log,
    enum ll_level    level,
    const char      *source,
    unsigned int     line,
    const char      *format,
    ...
);

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
    int                  n;
    size_t               space = LL_MAX_MESSAGE_SIZE;
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

    // Message format is the following.  Square brackets indicate optional portions of the message.
    // [YYYY-MM-DD HH:MM:SS.mmm ]LEVL [file:line ]logger.name: Formatted log message

    // First, write the timestamp into the buffer, if so configured.
#if LL_TIMESTAMP
    {
        struct tm       tm;
        unsigned long   milliseconds;

        // Obtain the current system time.
        LL_GET_TIME(&seconds, &microseconds);
        if (LL_CONVERT_TIME(seconds, &tm) == NULL)
        {
            err = "Local time overflow!";
            goto end;
        }
        milliseconds = microseconds / 1000U;

        // Write the first part of the time stamp (down to seconds).
        n = strftime(buffer, space, "%F %T", &tm);
        if (n == 0)
        {
            err = "No space for time stamp!";
            goto end;
        }
        space -= n;

        // Write the fractional seconds.
        n = snprintf(buffer + n, space, ".%03u ", milliseconds);
        if (n < 5 || n >= space)
        {
            err = "No space for time stamp milliseconds!";
            goto end;
        }
        space -= n;
    }
#endif /* end LL_TIMESTAMP */

    // Print the log level next.
    n = LL_MAX_MESSAGE_SIZE - space;
    n = snprintf(buffer + n, space, "%5s ", LL_LEVEL_NAME(level));
    if (n < 5 || n >= (int) space)
    {
        err = "No space for log level!";
        goto end;
    }
    space -= n;

#if LL_LOCATION
    // Print the file and line number, if so configured.
    n = LL_MAX_MESSAGE_SIZE - space;
    n = snprintf(buffer + n, space, "%s:%u ", source, line);
    if (n < 3 || n >= (int) space)
    {
        err = "No space for location info!";
        goto end;
    }
    space -= n;
#endif /* end LL_LOCATION */

    // Print the logger path into the buffer.
    n = LL_MAX_MESSAGE_SIZE - space;
    n = get_path(log, buffer + n, space);
    if (n < 0 || n >= (int) space)
    {
        err = "Log path too long!";
        goto end;
    }
    space -= n;

    // Next, print the rest of the preamble.
    n = LL_MAX_MESSAGE_SIZE - space;
    n = snprintf(buffer + n, space, ": ");
    if (n >= (int) space)
    {
        err = "Separator too long!";
        goto end;
    }
    space -= n;

    // Write the formatted message.
    n = LL_MAX_MESSAGE_SIZE - space;
    n = vsnprintf(buffer + n, space, format, args);
    if (n >= (int) space)
    {
        err = "Message too long!";
        goto end;
    }
    space -= n;

#if LL_APPEND_NEWLINE
    // Print the newline at the end of the message, if so configured.
    n = LL_MAX_MESSAGE_SIZE - space;
    n = snprintf(buffer + n, space, "\n");
    if (n < 1 || n >= space)
    {
        err = "No space for newline!";
        goto end;
    }
#endif /* end LL_LOCATION */

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
