/**
 * @file        common.c
 * @copyright   2021 Andrew MacIsaac
 * @remark
 *      SPDX-License-Identifier: BSD-2-Clause
 *
 * @brief       Common functions used by the logger implementations.
 */
#include "common.h"

#include <assert.h>
#include <stdio.h>

#if LL_DEFAULT_LEVEL_MAPPING
/// Provide the log level name mapping when using the default configuration.
const char *_ll_log_level_name[LL_LEVEL_INHERIT] =
{
    "FATAL",
    "ERROR",
    "Warn",
    "Info",
    "debug",
    "trace"
};
#endif /* end LL_DEFAULT_LEVEL_MAPPING */

/// Get the the threshold level below which a log's messages should be displayed.
enum ll_level get_threshold(const struct ll_log *log)
{
    assert(log != NULL);

    while (log->level == LL_LEVEL_INHERIT)
    {
        if (log->parent != NULL)
        {
            log = log->parent;
        }
    }

    return log->level;
}

/// Get the path of the log, that is the names of this log and its ancestors as a single string.
int get_path(const struct ll_log *log, char *buffer, size_t size)
{
    int length = 0;
    const char *dot = "";

    assert(buffer != NULL);
    assert(log != NULL);
    assert(log->name != NULL);
    assert(size > 0);

    if (log->parent != NULL)
    {
        length = get_path(log->parent, buffer, size);
        if (length < 0)
        {
            return length;
        }

        size -= length;
        dot = ".";
    }

    return snprintf(buffer + length, size, "%s%s", dot, log->name);
}

/// Get the list of targets to which a given log writes.
struct ll_target *get_targets(struct ll_log *log, struct ll_log **owner)
{
    assert(owner != NULL);
    *owner = NULL;

    if (log == NULL)
    {
        return NULL;
    }

    LOCK(log);
    if (log->targets != NULL)
    {
        // Leave owner locked when we return so that target list cannot change.
        *owner = log;
        return log->targets;
    }
    UNLOCK(log);

    return get_targets(log->parent, owner);
}

///  Display an error from the logging system itself.
void post_error(const char *error, const char *source, unsigned int line)
{
    assert(error != NULL);

    if (source == NULL)
    {
        source = "<unknown>";
    }
    fprintf(stderr, "\n[loglib error] %s:%u %s\n", source, line, error);
}
