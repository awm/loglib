#
# @file        CMakeLists.txt
# @copyright   2021 Andrew MacIsaac
# @remark
#      SPDX-License-Identifier: BSD-2-Clause
#
# @brief       Root build definition file for loglib.
#
cmake_minimum_required(VERSION 3.5)
project(loglib LANGUAGES C)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Enable unit testing framework.
include(CTest)

# Configure basic C compiler settings.
set(CMAKE_C_STANDARD 99)
set(GNU_LIKE GNU Clang AppleClang)
if (CMAKE_C_COMPILER_ID IN_LIST GNU_LIKE)
    add_compile_options(-Wall -Wextra -Werror)
elseif (MSVC)
    add_compile_options(/W4 /WX)
endif()

# Add subdirectories.
add_subdirectory(source)
# add_subdirectory(documentation)
# if (BUILD_TESTING)
#     add_subdirectory(test)
# endif()
