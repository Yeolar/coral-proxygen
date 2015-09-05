#
# Copyright (C) 2015, Yeolar
#

set(PROXYGEN_LIB_HTTP_CODEC_COMPRESS_TEST_LIB_SRCS
    HTTPArchive.cpp
    TestUtil.cpp
)
add_library(proxygen_lib_http_codec_compress_test STATIC
    ${PROXYGEN_LIB_HTTP_CODEC_COMPRESS_TEST_LIB_SRCS})
target_link_libraries(proxygen_lib_http_codec_compress_test proxygen_static)


set(PROXYGEN_LIB_HTTP_CODEC_COMPRESS_TEST_SRCS
    HeaderPieceTests.cpp
    HeaderTableTests.cpp
    HPACKBufferTests.cpp
    HPACKCodecTests.cpp
    HPACKContextTests.cpp
    HPACKHeaderTests.cpp
    HuffmanTests.cpp
    LoggingTests.cpp
    RFCExamplesTests.cpp
)

foreach(test_src ${PROXYGEN_LIB_HTTP_CODEC_COMPRESS_TEST_SRCS})
    get_filename_component(test_name ${test_src} NAME_WE)
    set(test proxygen_lib_http_codec_compress_${test_name})
    add_executable(${test} ${test_src})
    target_link_libraries(${test}
        ${GTEST_BOTH_LIBRARIES} ${GMOCK_LIBRARIES}
        proxygen_static proxygen_lib_http_codec_compress_test)
    add_test(${test} ${test} CONFIGURATIONS Debug)
endforeach()

