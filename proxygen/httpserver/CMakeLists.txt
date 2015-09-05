#
# Copyright (C) 2015, Yeolar
#

set(PROXYGEN_HTTPSERVER_SRCS
    HTTPServerAcceptor.cpp
    HTTPServer.cpp
    RequestHandlerAdaptor.cpp
    SignalHandler.cpp
)
add_library(proxygen_httpserver OBJECT ${PROXYGEN_HTTPSERVER_SRCS})


install(FILES
    Filters.h
    HTTPServerAcceptor.h
    HTTPServer.h
    HTTPServerOptions.h
    Mocks.h
    RequestHandlerAdaptor.h
    RequestHandlerFactory.h
    RequestHandler.h
    ResponseBuilder.h
    ResponseHandler.h
    ScopedHTTPServer.h
    SignalHandler.h
    DESTINATION include/proxygen/httpserver
)

