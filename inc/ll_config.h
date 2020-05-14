/**
 *  @file ll_config.h
 *
 *  Default loglib compile time feature configuration.
 *  This can be overridden by defining LL_CONFIG to be the path to an alternate header.
 */

#ifndef LL_CONFIG_H
#define LL_CONFIG_H

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
 *  Convert an enum ll_level value into a string suitable for display in a log message.  This must
 *  support all listed enum values.  No value less than 0 or greater than or equal to
 *  LL_LEVEL_INHERIT will be passed to this macro, allowing the mapping to be efficiently
 *  implemented using an array.
 *
 *  @param level    Level value.
 *
 *  @return Constant string representation if the log level.  To maintain the best formatting, it is
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

/// Maximum size of message buffer, in bytes.
#define LL_MAX_MESSAGE_SIZE 1024

/**
 *  Allocate message buffer.  In this case, allocate on the stack.  The character buffer "name" must
 *  be created by this macro.
 *
 *  @param  name    Buffer variable name.
 *  @param  size    Size of the requested buffer, in bytes.  This will always be a constant value.
 */
#define LL_ALLOCATE_BUFFER(name, size)  char name[size]

/**
 *  Release the message buffer.  In this case, because the buffer is allocated on the stack, no
 *  action is required.
 *
 *  @param  name Buffer variable name.
 */
#define LL_RELEASE_BUFFER(name)

/// Enable file:line information in log messages.
#define LL_LOCATION 1

#endif /* end LL_CONFIG_H */
