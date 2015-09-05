#
# Copyright (C) 2015, Yeolar
#

set(PROXYGEN_LIB_HTTP_CODEC_COMPRESS_EXPERIMENTAL_HPACK9_TEST_SRCS
    HPACKBufferTests.cpp
    HPACKCodecTests.cpp
    HPACKContextTests.cpp
    HuffmanTests.cpp
    RFCExamplesTests.cpp
)

foreach(test_src ${PROXYGEN_LIB_HTTP_CODEC_COMPRESS_EXPERIMENTAL_HPACK9_TEST_SRCS})
    get_filename_component(test_name ${test_src} NAME_WE)
    set(test proxygen_lib_http_codec_compress_experimental_hpack9_${test_name})
    add_executable(${test} ${test_src})
    target_link_libraries(${test}
        ${GTEST_BOTH_LIBRARIES} ${GMOCK_LIBRARIES}
        proxygen_static proxygen_lib_http_codec_compress_test)
    add_test(${test} ${test} CONFIGURATIONS Debug)
endforeach()

