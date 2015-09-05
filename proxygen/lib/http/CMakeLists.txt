#
# Copyright (C) 2015, Yeolar
#

if(NOT EXISTS ${PROJECT_SOURCE_DIR}/proxygen/lib/http/HTTPCommonHeaders.h)
    set(ENV{HEADERS_LIST} ${PROJECT_SOURCE_DIR}/proxygen/lib/http/HTTPCommonHeaders.txt)
    set(ENV{FBCODE_DIR} ${PROJECT_SOURCE_DIR})
    set(ENV{INSTALL_DIR} ${PROJECT_SOURCE_DIR}/proxygen/lib/http)

    execute_process(
        COMMAND ${PROJECT_SOURCE_DIR}/proxygen/lib/http/gen_HTTPCommonHeaders.h.sh
        COMMAND ${PROJECT_SOURCE_DIR}/proxygen/lib/http/gen_HTTPCommonHeaders.cpp.sh
    )
endif()


set(PROXYGEN_LIB_HTTP_SRCS
    HTTPCommonHeaders.cpp
    HTTPConnector.cpp
    HTTPConstants.cpp
    HTTPException.cpp
    HTTPHeaders.cpp
    HTTPMessage.cpp
    HTTPMethod.cpp
    ProxygenErrorEnum.cpp
    RFC2616.cpp
    Window.cpp
)
add_library(proxygen_lib_http OBJECT ${PROXYGEN_LIB_HTTP_SRCS})


install(FILES
    HTTPCommonHeaders.h
    HTTPConnector.h
    HTTPConstants.h
    HTTPException.h
    HTTPHeaders.h
    HTTPHeaderSize.h
    HTTPMessageFilters.h
    HTTPMessage.h
    HTTPMethod.h
    ProxygenErrorEnum.h
    RFC2616.h
    Window.h
    DESTINATION include/proxygen/lib/http
)

