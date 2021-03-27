/**
 * @file        port.h
 * @copyright   2021 Andrew MacIsaac
 * @remark
 *      SPDX-License-Identifier: BSD-2-Clause
 *
 * @brief       Port selection (if applicable), for the current target environment.
 */
#ifndef PORT_H_
#define PORT_H_

#include "ll_internal.h"

/**
 * @section mutex Mutex Ports
 */
#if LL_THREADING
#   if !defined(LL_LOCK) && !defined(LL_UNLOCK) && !defined(LL_STATIC_MUTEX_INIT)
#      include "port/freertos/mutex.h"
#   endif
#   if !defined(LL_LOCK) && !defined(LL_UNLOCK) && !defined(LL_STATIC_MUTEX_INIT)
#      include "port/mswin/mutex.h"
#   endif
#   if !defined(LL_LOCK) && !defined(LL_UNLOCK) && !defined(LL_STATIC_MUTEX_INIT)
#      include "port/posix/mutex.h"
#   endif
#   if !defined(LL_LOCK) && !defined(LL_UNLOCK) && !defined(LL_STATIC_MUTEX_INIT)
#       error No mutex implementation provided, and no compatible existing port found!
#   endif
#endif /* end LL_THREADING */

/**
 * @section gettime Time Retrieval Ports
 */
#if LL_TIMESTAMP
#   if LL_LOCALTIME
#       ifndef LL_CONVERT_TIME
#          include "port/mswin/localtime.h"
#       endif
#       ifndef LL_CONVERT_TIME
#          include "port/posix/localtime.h"
#       endif
#       ifndef LL_CONVERT_TIME
#          include "port/stdc/localtime.h"
#       endif
#   else /* !LL_LOCALTIME */
#       ifndef LL_CONVERT_TIME
#          include "port/mswin/utctime.h"
#       endif
#       ifndef LL_CONVERT_TIME
#          include "port/posix/utctime.h"
#       endif
#       ifndef LL_CONVERT_TIME
#          include "port/stdc/utctime.h"
#       endif
#   endif /* end !LL_LOCALTIME */
#   ifndef LL_CONVERT_TIME
#       error No time conversion implementation provided, and no compatible existing port found!
#   endif

#   ifndef LL_GET_TIME
#      include "port/mswin/gettime.h"
#   endif
#   ifndef LL_GET_TIME
#      include "port/posix/gettime.h"
#   endif
#   ifndef LL_GET_TIME
#      include "port/stdc/gettime.h"
#   endif
#   ifndef LL_GET_TIME
#       error No implementation provided to obtain time, and no compatible existing port found!
#   endif
#endif /* end LL_TIMESTAMP */

#endif /* end PORT_H_ */
