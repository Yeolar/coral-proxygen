#
# Copyright (C) 2015, Yeolar
#

# Setup compiler
if(APPLE)
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
else()
    # required GCC >= 4.8
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
endif()

# Define project
project(proxygen)

set(VERSION_MAJOR 13)
set(VERSION_MINOR 0)
set(VERSION_MICRO 0)
set(VERSION_PATCH 0)

cmake_minimum_required(VERSION 2.8.8)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake-scripts")

# http://www.cmake.org/Wiki/CMake_RPATH_handling#Mac_OS_X_and_the_RPATH
if(APPLE)
    set(CMAKE_MACOSX_RPATH ON)
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES
        "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
    if("${isSystemDir}" STREQUAL "-1")
        set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
    endif()
endif()

# Required packages
find_package(Folly REQUIRED)
set(PROXYGEN_HAVE_FOLLY ${FOLLY_FOUND})
find_package(Wangle REQUIRED)
set(PROXYGEN_HAVE_WANGLE ${WANGLE_FOUND})
find_package(Boost REQUIRED COMPONENTS system thread)
set(PROXYGEN_HAVE_BOOST ${Boost_FOUND})
find_package(DoubleConversion REQUIRED)
set(PROXYGEN_HAVE_LIBDOUBLE_CONVERSION ${DOUBLE_CONVERSION_FOUND})
find_package(GFlags REQUIRED)
set(PROXYGEN_HAVE_LIBGFLAGS ${GFLAGS_FOUND})
find_package(Glog REQUIRED)
set(PROXYGEN_HAVE_LIBGLOG ${GLOG_FOUND})
find_package(OpenSSL REQUIRED)
set(PROXYGEN_HAVE_LIBSSL ${OPENSSL_FOUND})
find_package(ZLIB REQUIRED)
set(PROXYGEN_HAVE_LIBZ ${ZLIB_FOUND})

# Optional packages
find_package(GTest)
set(PROXYGEN_HAVE_LIBGTEST ${GTEST_FOUND})
find_package(GMock)
set(PROXYGEN_HAVE_LIBGMOCK ${GMOCK_FOUND})

# Setup environment
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC")

# Link libraries
link_libraries(
    ${FOLLY_LIBRARIES}
    ${WANGLE_LIBRARIES}
    ${Boost_LIBRARIES}
    ${DOUBLE_CONVERSION_LIBRARY}
    ${GFLAGS_LIBRARIES}
    ${GLOG_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${ZLIB_LIBRARIES}
)

# Check and setup configurations
include(CheckCXXSourceCompiles REQUIRED)

if(GFLAGS_FOUND)
    set(CMAKE_REQUIRED_FLAGS "-std=c++11")
    set(CMAKE_REQUIRED_LIBRARIES "gflags")

    check_cxx_source_compiles("
        #include <gflags/gflags.h>
        void foo() { google::GetArgv(); }
        int main() { return 0; }"
        PROXYGEN_UNUSUAL_GFLAGS_NAMESPACE)

    if(PROXYGEN_UNUSUAL_GFLAGS_NAMESPACE)
        set(PROXYGEN_GFLAGS_NAMESPACE "google")
    else()
        set(PROXYGEN_GFLAGS_NAMESPACE "gflags")
    endif()
endif()

# Uninstall
configure_file(
    "${PROJECT_SOURCE_DIR}/cmake-scripts/cmake_uninstall.cmake.in"
    "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY
)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND}
        -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)


if(NOT EXISTS ${PROJECT_SOURCE_DIR}/proxygen/lib/http/HTTPCommonHeaders.cpp)
  message(STATUS "Generate HTTPCommonHeaders.{h,cpp}")
  set(ENV{HEADERS_LIST}
      ${PROJECT_SOURCE_DIR}/proxygen/lib/http/HTTPCommonHeaders.txt)
  set(ENV{FBCODE_DIR} ${PROJECT_SOURCE_DIR})
  set(ENV{INSTALL_DIR} ${PROJECT_SOURCE_DIR}/proxygen/lib/http)

  execute_process(
    COMMAND ${PROJECT_SOURCE_DIR}/proxygen/lib/http/gen_HTTPCommonHeaders.h.sh
    COMMAND ${PROJECT_SOURCE_DIR}/proxygen/lib/http/gen_HTTPCommonHeaders.cpp.sh
  )
endif()


if(NOT EXISTS ${PROJECT_SOURCE_DIR}/proxygen/lib/utils/TraceEventType.cpp OR
   NOT EXISTS ${PROJECT_SOURCE_DIR}/proxygen/lib/utils/TraceFieldType.cpp)
  message(STATUS "Generate TraceEventType.{h,cpp} TraceFieldType.{h,cpp}")
  execute_process(
    COMMAND ${PROJECT_SOURCE_DIR}/proxygen/lib/utils/gen_trace_event_constants.py
            --install_dir=${PROJECT_SOURCE_DIR}/proxygen/lib/utils
            --input_files=${PROJECT_SOURCE_DIR}/proxygen/lib/utils/TraceEventType.txt,${PROJECT_SOURCE_DIR}/proxygen/lib/utils/TraceFieldType.txt
            --output_type=cpp
            --output_scope=proxygen
            --header_path=proxygen/lib/utils
  )
endif()


include_directories(${PROJECT_SOURCE_DIR})

# Work on all sub directories
add_subdirectory(proxygen/external/http_parser)
add_subdirectory(proxygen/httpserver)
add_subdirectory(proxygen/httpserver/filters)
add_subdirectory(proxygen/httpserver/samples/echo)
add_subdirectory(proxygen/lib/http)
add_subdirectory(proxygen/lib/http/codec)
add_subdirectory(proxygen/lib/http/codec/experimental)
add_subdirectory(proxygen/lib/http/codec/compress)
add_subdirectory(proxygen/lib/http/codec/compress/experimental/hpack9)
add_subdirectory(proxygen/lib/http/session)
add_subdirectory(proxygen/lib/services)
add_subdirectory(proxygen/lib/ssl)
add_subdirectory(proxygen/lib/utils)

# Link libraries
add_library(proxygen_static STATIC
    $<TARGET_OBJECTS:proxygen_external_httpparser>
    $<TARGET_OBJECTS:proxygen_httpserver>
    $<TARGET_OBJECTS:proxygen_lib_http>
    $<TARGET_OBJECTS:proxygen_lib_http_codec>
    $<TARGET_OBJECTS:proxygen_lib_http_codec_experimental>
    $<TARGET_OBJECTS:proxygen_lib_http_codec_compress>
    $<TARGET_OBJECTS:proxygen_lib_http_codec_compress_experimental_hpack9>
    $<TARGET_OBJECTS:proxygen_lib_http_session>
    $<TARGET_OBJECTS:proxygen_lib_services>
    $<TARGET_OBJECTS:proxygen_lib_utils>
)
set_target_properties(proxygen_static PROPERTIES OUTPUT_NAME proxygen)
add_library(proxygen_shared SHARED
    $<TARGET_OBJECTS:proxygen_external_httpparser>
    $<TARGET_OBJECTS:proxygen_httpserver>
    $<TARGET_OBJECTS:proxygen_lib_http>
    $<TARGET_OBJECTS:proxygen_lib_http_codec>
    $<TARGET_OBJECTS:proxygen_lib_http_codec_experimental>
    $<TARGET_OBJECTS:proxygen_lib_http_codec_compress>
    $<TARGET_OBJECTS:proxygen_lib_http_codec_compress_experimental_hpack9>
    $<TARGET_OBJECTS:proxygen_lib_http_session>
    $<TARGET_OBJECTS:proxygen_lib_services>
    $<TARGET_OBJECTS:proxygen_lib_utils>
)
set_target_properties(proxygen_shared PROPERTIES OUTPUT_NAME proxygen)

install(TARGETS proxygen_static
    DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_WRITE
                GROUP_READ
                WORLD_READ
)
install(TARGETS proxygen_shared
    DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                GROUP_READ GROUP_EXECUTE
                WORLD_READ WORLD_EXECUTE
)

# Test
if(GTEST_FOUND AND GMOCK_FOUND)
    enable_testing()
    add_subdirectory(proxygen/httpserver/test)
    add_subdirectory(proxygen/httpserver/filters/test)
    add_subdirectory(proxygen/lib/test)
    add_subdirectory(proxygen/lib/http/test)
    add_subdirectory(proxygen/lib/http/codec/test)
    #add_subdirectory(proxygen/lib/http/codec/experimental/test)
    add_subdirectory(proxygen/lib/http/codec/compress/test)
    add_subdirectory(proxygen/lib/http/codec/compress/experimental/hpack9/test)
    add_subdirectory(proxygen/lib/http/session/test)
    add_subdirectory(proxygen/lib/services/test)
    add_subdirectory(proxygen/lib/utils/test)
endif()

