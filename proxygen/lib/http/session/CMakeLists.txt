#
# Copyright (C) 2015, Yeolar
#

set(PROXYGEN_LIB_HTTP_SESSION_SRCS
    ByteEvents.cpp
    ByteEventTracker.cpp
    CodecErrorResponseHandler.cpp
    HTTP2PriorityQueue.cpp
    HTTPDirectResponseHandler.cpp
    HTTPDownstreamSession.cpp
    HTTPErrorPage.cpp
    HTTPEvent.cpp
    HTTPSessionAcceptor.cpp
    HTTPSession.cpp
    HTTPTransaction.cpp
    HTTPTransactionEgressSM.cpp
    HTTPTransactionIngressSM.cpp
    HTTPUpstreamSession.cpp
    SimpleController.cpp
    TransportFilter.cpp
)
add_library(proxygen_lib_http_session OBJECT ${PROXYGEN_LIB_HTTP_SESSION_SRCS})


install(FILES
    AckLatencyEvent.h
    ByteEvents.h
    ByteEventTracker.h
    CodecErrorResponseHandler.h
    HTTP2PriorityQueue.h
    HTTPDirectResponseHandler.h
    HTTPDownstreamSession.h
    HTTPErrorPage.h
    HTTPEvent.h
    HTTPSessionAcceptor.h
    HTTPSessionController.h
    HTTPSession.h
    HTTPSessionStats.h
    HTTPTransactionEgressSM.h
    HTTPTransaction.h
    HTTPTransactionIngressSM.h
    HTTPUpstreamSession.h
    SimpleController.h
    TransportFilter.h
    TTLBAStats.h
    DESTINATION include/proxygen/lib/http/session
)

