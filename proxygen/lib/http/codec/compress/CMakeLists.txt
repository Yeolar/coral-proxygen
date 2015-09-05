#
# Copyright (C) 2015, Yeolar
#

set(PROXYGEN_LIB_HTTP_CODEC_COMPRESS_SRCS
    GzipHeaderCodec.cpp
    HeaderTable.cpp
    HPACKCodec.cpp
    HPACKContext.cpp
    HPACKDecodeBuffer.cpp
    HPACKDecoder.cpp
    HPACKEncodeBuffer.cpp
    HPACKEncoder.cpp
    HPACKHeader.cpp
    Huffman.cpp
    Logging.cpp
    StaticHeaderTable.cpp
)
add_library(proxygen_lib_http_codec_compress OBJECT ${PROXYGEN_LIB_HTTP_CODEC_COMPRESS_SRCS})


install(FILES
    GzipHeaderCodec.h
    HeaderCodec.h
    Header.h
    HeaderPiece.h
    HeaderTable.h
    HPACKCodec.h
    HPACKConstants.h
    HPACKContext.h
    HPACKDecodeBuffer.h
    HPACKDecoder.h
    HPACKEncodeBuffer.h
    HPACKEncoder.h
    HPACKHeader.h
    Huffman.h
    Logging.h
    StaticHeaderTable.h
    DESTINATION include/proxygen/lib/http/codec/compress
)

