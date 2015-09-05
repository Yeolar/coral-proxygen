#
# Copyright (C) 2015, Yeolar
#

add_library(proxygen_external_httpparser OBJECT http_parser_cpp.cpp)


install(FILES
    http_parser.h
    DESTINATION include/proxygen/external/http_parser
)


add_executable(proxygen_external_httpparser_test test.c http_parser.c)
add_test(proxygen_external_httpparser_test
    proxygen_external_httpparser_test
    CONFIGURATIONS Debug)

