#
# Copyright (C) 2015, Yeolar
#

set(PROXYGEN_LIB_HTTP_CODEC_TEST_LIB_SRCS
    TestUtils.cpp
)
add_library(proxygen_lib_http_codec_test STATIC
    ${PROXYGEN_LIB_HTTP_CODEC_TEST_LIB_SRCS})
target_link_libraries(proxygen_lib_http_codec_test proxygen_static)


set(PROXYGEN_LIB_HTTP_CODEC_TEST_SRCS
    FilterTests.cpp
    HTTP1xCodecTest.cpp
    SPDYCodecTest.cpp
)

foreach(test_src ${PROXYGEN_LIB_HTTP_CODEC_TEST_SRCS})
    get_filename_component(test_name ${test_src} NAME_WE)
    set(test proxygen_lib_http_codec_${test_name})
    add_executable(${test} ${test_src})
    target_link_libraries(${test}
        ${GTEST_BOTH_LIBRARIES} ${GMOCK_LIBRARIES}
        proxygen_static proxygen_lib_http_codec_test)
    add_test(${test} ${test} CONFIGURATIONS Debug)
endforeach()

