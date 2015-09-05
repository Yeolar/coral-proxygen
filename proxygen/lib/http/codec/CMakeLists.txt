#
# Copyright (C) 2015, Yeolar
#

set(PROXYGEN_LIB_HTTP_CODEC_SRCS
    CodecProtocol.cpp
    ErrorCode.cpp
    FlowControlFilter.cpp
    HTTP1xCodec.cpp
    HTTPChecks.cpp
    HTTPCodecFilter.cpp
    HTTPCodecPrinter.cpp
    HTTPParallelCodec.cpp
    HTTPSettings.cpp
    SettingsId.cpp
    SPDYCodec.cpp
    SPDYConstants.cpp
    SPDYUtil.cpp
    TransportDirection.cpp
)
add_library(proxygen_lib_http_codec OBJECT ${PROXYGEN_LIB_HTTP_CODEC_SRCS})


install(FILES
    CodecDictionaries.h
    CodecProtocol.h
    ErrorCode.h
    FlowControlFilter.h
    HTTP1xCodec.h
    HTTPChecks.h
    HTTPCodecFilter.h
    HTTPCodec.h
    HTTPCodecPrinter.h
    HTTPParallelCodec.h
    HTTPSettings.h
    SettingsId.h
    SPDYCodec.h
    SPDYConstants.h
    SPDYUtil.h
    SPDYVersion.h
    SPDYVersionSettings.h
    TransportDirection.h
    DESTINATION include/proxygen/lib/http/codec
)

