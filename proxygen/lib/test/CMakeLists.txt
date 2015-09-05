#
# Copyright (C) 2015, Yeolar
#

set(PROXYGEN_LIB_TEST_LIB_SRCS
    TestAsyncTransport.cpp
)
add_library(proxygen_lib_test STATIC
    ${PROXYGEN_LIB_TEST_LIB_SRCS})
target_link_libraries(proxygen_lib_test proxygen_static)

