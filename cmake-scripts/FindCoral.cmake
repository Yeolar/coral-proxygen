#
# Copyright (C) 2015, Yeolar
#
# - Try to find coral
# This will define
# CORAL_FOUND
# CORAL_INCLUDE_DIR
# CORAL_LIBRARIES

cmake_minimum_required(VERSION 2.8.8)

include(FindPackageHandleStandardArgs)

find_library(CORAL_LIBRARY coral PATHS ${CORAL_LIBRARYDIR})
find_path(CORAL_INCLUDE_DIR "coral/String.h" PATHS ${CORAL_INCLUDEDIR})

set(CORAL_LIBRARIES ${CORAL_LIBRARY})

find_package_handle_standard_args(Coral
    REQUIRED_ARGS CORAL_INCLUDE_DIR CORAL_LIBRARIES)
