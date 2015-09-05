#
# Copyright (C) 2015, Yeolar
#

set(PROXYGEN_LIB_UTILS_TEST_SRCS
    AsyncTimeoutSetTest.cpp
    CryptUtilTest.cpp
    GenericFilterTest.cpp
    HTTPTimeTest.cpp
    LoggingTests.cpp
    ParseURLTest.cpp
    RendezvousHashTest.cpp
    ResultTest.cpp
    UtilTest.cpp
    ZlibTests.cpp
)

foreach(test_src ${PROXYGEN_LIB_UTILS_TEST_SRCS})
    get_filename_component(test_name ${test_src} NAME_WE)
    set(test proxygen_lib_utils_${test_name})
    add_executable(${test} ${test_src})
    target_link_libraries(${test}
        ${GTEST_BOTH_LIBRARIES} ${GMOCK_LIBRARIES} proxygen_static)
    add_test(${test} ${test} CONFIGURATIONS Debug)
endforeach()


set(PROXYGEN_LIB_UTILS_BENCHMARK_SRCS
    ResultBenchmark.cpp
)

foreach(bench_src ${PROXYGEN_LIB_UTILS_BENCHMARK_SRCS})
    get_filename_component(bench_name ${bench_src} NAME_WE)
    set(bench proxygen_lib_utils_${bench_name})
    add_executable(${bench} ${bench_src})
    target_link_libraries(${bench} ${GTEST_BOTH_LIBRARIES} proxygen_static)
endforeach()

