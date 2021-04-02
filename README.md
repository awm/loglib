# loglib

[![License][license-img]][license-link]
![Quality][quality-img]
[![CMake][cmake-img]][cmake-link]
[![Static Analysis][static-analysis-img]][static-analysis-link]
[![Coverity][coverity-img]][coverity-link]

[license-img]:   https://img.shields.io/github/license/awm/loglib "License"
[license-link]:  https://github.com/awm/loglib/blob/main/LICENSE.txt
[quality-img]:   https://img.shields.io/badge/quality-unstable-red "Quality"
[cmake-img]:     https://github.com/awm/loglib/actions/workflows/cmake.yml/badge.svg "CMake Status"
[cmake-link]:    https://github.com/awm/loglib/actions/workflows/cmake.yml?query=branch%3Amain
[coverity-img]:  https://img.shields.io/coverity/scan/22867 "Coverity Scan"
[coverity-link]: https://scan.coverity.com/projects/awm-loglib
[static-analysis-img]:  https://github.com/awm/loglib/actions/workflows/static-analysis.yml/badge.svg "Static Analysis Status"
[static-analysis-link]: https://github.com/awm/loglib/actions/workflows/static-analysis.yml?query=branch%3Amain

A configurable, embeddable logging framework.

## Initial Target Features

 * Written in C99.
 * No dynamic memory allocation or heap use.
 * Support for UTF-8.
 * Support for printf-style format specifiers.
 * Optional thread safety.
 * Custom log levels.
 * Compile-time log level selection.
 * Optional run-time log level selection.
 * Optional log prefix.
 * Log tree hierarchy.
 * Log extraction/offloading.
 * Configurable log targets.

## Future Features

 * Support for internationalization.
 * Optional time stamp, file, function, line number format specifiers.
 * Support for custom format specifiers.
