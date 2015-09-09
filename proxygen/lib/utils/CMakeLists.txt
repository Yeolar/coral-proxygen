#
# Copyright (C) 2015, Yeolar
#

set(PROXYGEN_LIB_UTILS_SRCS
    AsyncTimeoutSet.cpp
    ChromeUtils.cpp
    CryptUtil.cpp
    Exception.cpp
    HTTPTime.cpp
    Logging.cpp
    ParseURL.cpp
    RendezvousHash.cpp
    TraceEvent.cpp
    TraceEventContext.cpp
    TraceEventType.cpp
    TraceFieldType.cpp
    ZlibStreamCompressor.cpp
    ZlibStreamDecompressor.cpp
)
add_library(proxygen_lib_utils OBJECT ${PROXYGEN_LIB_UTILS_SRCS})


install(FILES
    AsyncTimeoutSet.h
    ChromeUtils.h
    CobHelper.h
    ConsistentHash.h
    CryptUtil.h
    DestructorCheck.h
    Exception.h
    FilterChain.h
    HTTPTime.h
    Logging.h
    NullTraceEventObserver.h
    ParseURL.h
    RendezvousHash.h
    Result.h
    StateMachine.h
    TestUtils.h
    Time.h
    TraceEvent.h
    TraceEventContext.h
    TraceEventObserver.h
    TraceEventType.h
    TraceFieldType.h
    UnionBasedStatic.h
    UtilInl.h
    ZlibStreamCompressor.h
    ZlibStreamDecompressor.h
    DESTINATION include/proxygen/lib/utils
)

