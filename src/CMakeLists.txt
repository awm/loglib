project(loglib VERSION 0.1.0 LANGUAGES C)

include(CheckIncludeFile)
include(CheckSymbolExists)
include(CTest)

set(GNU_LIKE GNU Clang AppleClang)
if (CMAKE_C_COMPILER_ID IN_LIST GNU_LIKE)
    add_compile_options(-Wall -Wextra -Werror)
elseif (MSVC)
    add_compile_options(/W4 /WX)
endif()

# Test for platform features.
check_symbol_exists(snprintf  stdio.h HAVE_SNPRINTF)
check_symbol_exists(vsnprintf stdio.h HAVE_VSNPRINTF)

# configure_file(platform.h.in platform.h UNIX)

include_directories(
    ${CMAKE_SOURCE_DIR}/inc
)

set(
    SRCS
    log.c
    common.c
)

# if (not HAVE_SNPRINTF)

add_library(log STATIC ${SRCS})

# if (BUILD_TESTING)
#     add_subdirectory(${CMAKE_SOURCE_DIR}/test/unit ${CMAKE_BINARY_DIR}/test/unit)
# endif()
