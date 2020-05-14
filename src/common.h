/**
 *  @file common.h
 *
 *  Common functions used by the logger implementations.
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "ll_internal.h"

#define LOCK(l)
#define UNLOCK(l)

/**
 *  Get the the threshold level below which a log's messages should be displayed.
 *
 *  @return The level below which to display messages.
 */
enum ll_level get_threshold
(
    const struct ll_log *log    ///< Log handle.
);

/**
 *  Get the path of the log, that is the names of this log and its ancestors as a single string.
 *
 *  @return The number of bytes written or that would be written to the buffer given enough space,
 *          or a negative value on error.
 */
int get_path
(
    const struct ll_log *log,       ///< [in]  Log handle.
    char                *buffer,    ///< [out] Buffer to write path to.
    size_t               size       ///< [in]  Buffer size in bytes.
);

/**
 *  Get the list of targets to which a given log writes.
 *
 *  @return List of log targets, or NULL if there are none.
 */
struct ll_target *get_targets
(
    struct ll_log    *log,  ///< [in]   Log handle.  May be NULL.
    struct ll_log   **owner ///< [out]  Log handle that actually owns the returned targets.  The
                            ///<        returned owner will be locked and must be unlocked by the
                            ///<        caller.
);

/**
 *  Display an error from the logging system itself.  The message will be written to stderr.
 */
void post_error
(
    const char      *error,     ///< Error message.
    const char      *source,    ///< Source file of the log statement where the error occured.  May
                                ///< be NULL.
    unsigned int     line       ///< Line number of the log statement where the error ocurred.
);

#endif /* end COMMON_H_ */