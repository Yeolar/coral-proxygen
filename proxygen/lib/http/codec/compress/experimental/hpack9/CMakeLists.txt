#
# Copyright (C) 2015, Yeolar
#

set(PROXYGEN_LIB_HTTP_CODEC_COMPRESS_EXPERIMENTAL_HPACK9_SRCS
    HPACKCodec.cpp
    HPACKDecoder.cpp
    HPACKEncoder.cpp
    Huffman.cpp
    StaticHeaderTable.cpp
)
add_library(proxygen_lib_http_codec_compress_experimental_hpack9 OBJECT
    ${PROXYGEN_LIB_HTTP_CODEC_COMPRESS_EXPERIMENTAL_HPACK9_SRCS})


install(FILES
    HPACKCodec.h
    HPACKConstants.h
    HPACKContextImpl.h
    HPACKDecoder.h
    HPACKEncoder.h
    Huffman.h
    StaticHeaderTable.h
    DESTINATION include/proxygen/lib/http/codec/compress/experimental/hpack9
)

