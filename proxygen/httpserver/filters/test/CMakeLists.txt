#
# Copyright (C) 2015, Yeolar
#

set(PROXYGEN_HTTPSERVER_FILTERS_TEST_SRCS
    ZlibServerFilterTest.cpp
)

foreach(test_src ${PROXYGEN_HTTPSERVER_FILTERS_TEST_SRCS})
    get_filename_component(test_name ${test_src} NAME_WE)
    set(test proxygen_httpserver_filters_${test_name})
    add_executable(${test} ${test_src})
    target_link_libraries(${test}
        ${GTEST_BOTH_LIBRARIES} ${GMOCK_LIBRARIES} proxygen_static)
    add_test(${test} ${test} CONFIGURATIONS Debug)
endforeach()

