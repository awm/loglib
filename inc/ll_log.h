/**
 *  @file ll_log.h
 *
 *  Public loglib API.
 *  These functions an macros are the supported interface for using the loglib API.
 */

#ifndef LL_LOG_H
#define LL_LOG_H

#include "ll_internal.h"

#include <stdlib.h>

/// Write to parent log's target.
#define LL_INHERIT_TARGET   NULL

/**
 *  Initialiser for a log structure.
 *
 *  @param  name    Log name string.
 *  @param  prefix  Prefix to prepend to each log message.  Not currently implemented.  Set to NULL
 *                  for the default.
 *  @param  level   Default log threshold.
 *  @param  parent  Parent log instance.  Set to NULL if there is no parent.
 *  @param  targets Linked list of log targets to write messages to.  Set to LL_INHERIT_TARGET to
 *                  use the parent's targets.
 *
 *  Example:
 *  @code
 *  static struct ll_log MyLog = LL_LOG_INIT("mylog", NULL, LL_LEVEL_INFO, NULL, &PrintfTarget);
 *  @endcode
 */
#define LL_LOG_INIT(name, prefix, level, parent, targets) \
    { (parent), NULL, NULL, (name), (prefix), (level), (targets) }

/**
 *  Write a message to a log at the specified level, using positional parameters.
 *  If the log level is greater than the configured static maximum then no function call will be
 *  emitted in the code.
 *
 *  @param  log     Pointer to log instance.
 *  @param  level   Level at which to log the message.
 *  @param  format  Message format string.
 *  @param  ...     Positional parameters of format string.
 */
#define LL_LOG(log, level, format, ...) \
    (((level) <= LL_STATIC_MAX_LEVEL) ? _LL_LOG((log), (level), (format), __VA_ARGS__) : (void) 0)

/**
 *  Write a message to a log at the specified level, using a variable argument list.
 *  If the log level is greater than the configured static maximum then no function call will be
 *  emitted in the code.
 *
 *  @param  log     Pointer to log instance.
 *  @param  level   Level at which to log the message.
 *  @param  format  Message format string.
 *  @param  args    Positional parameters of format string.
 */
#define LL_LOGV(log, level, format, args) \
    (((level) <= LL_STATIC_MAX_LEVEL) ? _LL_LOGV((log), (level), (format), (args)) : (void) 0)

#endif /* end LL_LOG_H */
