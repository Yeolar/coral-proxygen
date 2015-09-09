#
# Copyright (C) 2015, Yeolar
#
# - Try to find folly
# This will define
# FOLLY_FOUND
# FOLLY_INCLUDE_DIR
# FOLLY_LIBRARIES

cmake_minimum_required(VERSION 2.8.8)

include(FindPackageHandleStandardArgs)

find_library(FOLLY_LIBRARY folly PATHS ${FOLLY_LIBRARYDIR})
find_path(FOLLY_INCLUDE_DIR "folly/String.h" PATHS ${FOLLY_INCLUDEDIR})

set(FOLLY_LIBRARIES ${FOLLY_LIBRARY})

find_package_handle_standard_args(Folly
    REQUIRED_ARGS FOLLY_INCLUDE_DIR FOLLY_LIBRARIES)
