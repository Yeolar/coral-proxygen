#
# Copyright (C) 2015, Yeolar
#

set(PROXYGEN_LIB_HTTP_CODEC_EXPERIMENTAL_SRCS
    HTTP2Codec.cpp
    HTTP2Constants.cpp
    HTTP2Framer.cpp
)
add_library(proxygen_lib_http_codec_experimental OBJECT
    ${PROXYGEN_LIB_HTTP_CODEC_EXPERIMENTAL_SRCS})


install(FILES
    HTTP2Codec.h
    HTTP2Constants.h
    HTTP2Framer.h
    HTTPRequestVerifier.h
    DESTINATION include/proxygen/lib/http/codec/experimental
)

