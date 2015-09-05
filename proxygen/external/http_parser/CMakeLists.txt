#
# Copyright (C) 2015, Yeolar
#

set(PROXYGEN_EXTERNAL_HTTPPARSER_SRCS
    http_parser_cpp.cpp
)
add_library(proxygen_external_httpparser OBJECT ${PROXYGEN_EXTERNAL_HTTPPARSER_SRCS})


install(FILES
    http_parser.h
    DESTINATION include/proxygen/external/http_parser
)

