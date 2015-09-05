#
# Copyright (C) 2015, Yeolar
#

set(PROXYGEN_LIB_SERVICES_SRCS
    RequestWorker.cpp
    Service.cpp
    WorkerThread.cpp
)
add_library(proxygen_lib_services OBJECT ${PROXYGEN_LIB_SERVICES_SRCS})


install(FILES
    AcceptorConfiguration.h
    HTTPAcceptor.h
    RequestWorker.h
    ServiceConfiguration.h
    Service.h
    ServiceWorker.h
    WorkerThread.h
    DESTINATION include/proxygen/lib/services
)

